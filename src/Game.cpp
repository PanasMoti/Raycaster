//
// Created by juicylucy on 1/2/23.
//

#include "Game.h"
#include <algorithm>

Game::Game(const char *t, int w, int h) : renderWindow(nullptr), player(nullptr)
{
    renderWindow = new RenderWindow(t, w, h);
    player = new Player();
    clock = new Clock();
    wall_test = img::Image("assets/wall_.png");
    floor_test = img::Image("assets/floor_.png");
    // ceil_test = img::Image("assets/ceil_test.png");
    sprite_tex[0] = img::Image("assets/Sprites/barrel.png");
    sprite_tex[1] = img::Image("assets/Sprites/pillar.png");
    sprite_tex[2] = img::Image("assets/Sprites/greenlight.png");
    for (int i = 0; i < w; i++)
        ZBuffer.push_back(0);
    render_dist = 3;
    sprites = {
        new img::Sprite(20.5, 11.5, sprite_tex[2]), // green light in front of playerstart
        // green lights in every room
        new img::Sprite(18.5, 4.5, sprite_tex[2]),
        new img::Sprite(10.0, 4.5, sprite_tex[2]),
        new img::Sprite(10.0, 12.5, sprite_tex[2]),
        new img::Sprite(3.5, 6.5, sprite_tex[2]),
        new img::Sprite(3.5, 20.5, sprite_tex[2]),
        new img::Sprite(3.5, 14.5, sprite_tex[2]),
        new img::Sprite(14.5, 20.5, sprite_tex[2]),

        // row of pillars in front of wall: fisheye test
        new img::Sprite(18.5, 10.5, sprite_tex[1]),
        new img::Sprite(18.5, 11.5, sprite_tex[1]),
        new img::Sprite(18.5, 12.5, sprite_tex[1]),

        // some barrels accross the map
        new img::Sprite(21.5, 1.5, sprite_tex[0]),
        new img::Sprite(15.5, 1.5, sprite_tex[0]),
        new img::Sprite(16.0, 1.8, sprite_tex[0]),
        new img::Sprite(16.2, 1.2, sprite_tex[0]),
        new img::Sprite(3.5, 2.5, sprite_tex[0]),
        new img::Sprite(9.5, 15.5, sprite_tex[0]),
        new img::Sprite(10.0, 15.1, sprite_tex[0]),
        new img::Sprite(10.5, 15.8, sprite_tex[0]),
    };
}

Game::~Game()
{
    delete renderWindow;
    delete clock;
    delete player;
    for (int i = 0; i < numSprites; i++)
    {
        delete sprites[i];
    }
}

void Game::MainLoop()
{
    while (!renderWindow->ShouldClose)
    {
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent))
        {
            if (sdlEvent.type == SDL_QUIT)
                renderWindow->ShouldClose = true;
            if (sdlEvent.type == SDL_KEYDOWN)
                this->KeyDown(sdlEvent);
            if (sdlEvent.type == SDL_KEYUP)
                this->KeyUp(sdlEvent);
        }
        clock->Update();
        renderWindow->clean_screen(img::Color("#000000"));
        this->Update(clock->DeltaTime());
        this->Draw3D();
        renderWindow->display();
    }
}

void Game::Update(float dt)
{
    float fps = 1000.0f / dt;
    dt *= 0.001f; // converting to seconds
    float moveSpeed = player->speed.x * dt;

    renderWindow->setWindowTitle("fps:" + std::to_string(fps));
    // todo: add collision with walls
    if (keyboard[SDLK_w])
    {
        player->pos = player->pos + player->dir * moveSpeed;
    }
    if (keyboard[SDLK_s])
    {
        player->pos -= player->dir * moveSpeed;
    }
    float rotSpeed = player->speed.y * dt;
    if (keyboard[SDLK_d])
    {
        // both camera direction and camera plane must be rotated
        float oldDirX = player->dir.x;
        player->dir.x = player->dir.x * cosf(-rotSpeed) - player->dir.y * sinf(-rotSpeed);
        player->dir.y = oldDirX * sinf(-rotSpeed) + player->dir.y * cosf(-rotSpeed);
        float oldPlaneX = player->plane.x;
        player->plane.x = player->plane.x * cosf(-rotSpeed) - player->plane.y * sinf(-rotSpeed);
        player->plane.y = oldPlaneX * sinf(-rotSpeed) + player->plane.y * cosf(-rotSpeed);
    }
    if (keyboard[SDLK_a])
    {
        // both camera direction and camera plane must be rotated
        float oldDirX = player->dir.x;
        player->dir.x = player->dir.x * cosf(rotSpeed) - player->dir.y * sinf(rotSpeed);
        player->dir.y = oldDirX * sinf(rotSpeed) + player->dir.y * cosf(rotSpeed);
        float oldPlaneX = player->plane.x;
        player->plane.x = player->plane.x * cosf(rotSpeed) - player->plane.y * sinf(rotSpeed);
        player->plane.y = oldPlaneX * sinf(rotSpeed) + player->plane.y * cosf(rotSpeed);
    }

    if (keyboard[SDLK_ESCAPE])
    {
        printf("closing!\n");
        renderWindow->ShouldClose = true;
        return;
    }
}

void Game::KeyDown(SDL_Event sdlEvent)
{
    keyboard.Press(static_cast<SDL_KeyCode>(sdlEvent.key.keysym.sym));
}

void Game::KeyUp(SDL_Event sdlEvent)
{
    keyboard.Release(static_cast<SDL_KeyCode>(sdlEvent.key.keysym.sym));
}

void Game::Draw3D()
{
    this->Floors();
    this->Rays();
    this->Sprites();
}

void Game::Rays()
{
    const float far_dist = ((mapHeight + mapWidth) / 2) * 0.1 * render_dist;
    int w = RESOLUTION_WIDTH;
    int h = RESOLUTION_HEIGHT;
    for (int x = 0; x < w; x++)
    {
        bool in_render_dist = true;
        float cameraX = 2.0f * (float)x / float(w) - 1;
        float2 rayDir = player->dir + player->plane * cameraX;
        int2 map = {int(player->pos.x), int(player->pos.y)};
        float2 sideDist;

        float2 deltaDist = {
            (rayDir.x == 0) ? 1e30 : std::fabs(1 / rayDir.x),
            (rayDir.y == 0) ? 1e30 : std::fabs(1 / rayDir.y)};
        float perpWallDist;
        //*calculate step and initial sideDist
        int2 step;
        int hit = 0;
        int side;
        if (rayDir.x < 0)
        {
            step.x = -1;
            sideDist.x = (player->pos.x - map.x) * deltaDist.x;
        }
        else
        {
            step.x = 1;
            sideDist.x = (map.x + 1.0f - player->pos.x) * deltaDist.x;
        }
        if (rayDir.y < 0)
        {
            step.y = -1;
            sideDist.y = (player->pos.y - map.y) * deltaDist.y;
        }
        else
        {
            step.y = 1;
            sideDist.y = (map.y + 1.0f - player->pos.y) * deltaDist.y;
        }
        //* DDA
        while (hit == 0)
        {
            //*jump to the next map square, either in x coords or y coords
            if (sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                map.x += step.x;
                side = 0;
            }
            else
            {
                sideDist.y += deltaDist.y;
                map.y += step.y;
                side = 1;
            }
            //* check if ray has hit a wall
            if (worldMap[map.x][map.y] > 0)
                hit = 1;
        }
        //*Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)
        if (side == 0)
            perpWallDist = (sideDist.x - deltaDist.x);
        else
            perpWallDist = (sideDist.y - deltaDist.y);

        in_render_dist = perpWallDist <= far_dist;
        //?Calculate height of line to draw on screen
        int lineHeight = (int)(h / perpWallDist);
        //?calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + h / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + h / 2;
        if (drawEnd >= h)
            drawEnd = h - 1;
        // only render if its in the render distance
        if (in_render_dist)
        {
            int texWidth = (int)wall_test.getWidth();
            int texHeight = (int)wall_test.getHeight();
            float wallX;
            if (side == 0)
                wallX = player->pos.y + perpWallDist * rayDir.y;
            else
                wallX = player->pos.x + perpWallDist * rayDir.x;
            wallX -= floorf(wallX);

            int texX = int(wallX * float(texWidth));
            if (side == 0 && rayDir.x > 0)
                texX = texWidth - texX - 1;
            if (side == 1 && rayDir.y < 0)
                texX = texWidth - texX - 1;

            float stepT = 1.0f * (float)texHeight / (float)lineHeight;
            float texPos = (drawStart - h / 2 + lineHeight / 2) * stepT;

            for (int y = drawStart; y < drawEnd; y++)
            {
                // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
                int texY = (int)texPos & (texHeight - 1);
                texPos += stepT;
                float s = 2.0f / (1 + perpWallDist * perpWallDist);
                s = SDL_clamp(s, 0.0f, 1.0f);
                img::Color color = (wall_test)(texX, texY) * s;
                if (s < 0.15f)
                    color = color.grayScale();
                renderWindow->set_pixel(x, y, color);
            }
        }
        // else {
        //     auto c = img::Color("#c0c0c0");
        //     renderWindow->vertLine(x,drawStart,drawEnd,c);

        // }
        ZBuffer[x] = perpWallDist;
    }
}

void Game::Floors()
{
    int w = RESOLUTION_WIDTH;
    int h = RESOLUTION_HEIGHT;
    int screenHeight = h;
    int screenWidth = w;
    const float far_dist = ((mapHeight + mapWidth) / 2) * 0.1 * render_dist;
    bool in_render_dist = true;
    //* FLOOR CASTING
    for (int y = h / 2 + 1; y < h; y++)
    {
        //? rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
        float2 rayDir0 = player->dir - player->plane;
        float2 rayDir1 = player->dir + player->plane;
        int p = y - screenHeight / 2;    //? Current y position compared to the center of the screen (the horizon)
        float posZ = 0.5 * screenHeight; //? Vertical position of the camera.

        //? Horizontal distance from the camera to the floor for the current row.
        //? 0.5 is the z position exactly in the middle between floor and ceiling.
        float rowDistance = posZ / p;

        // calculate the real world step vector we have to add for each x (parallel to camera plane)
        // adding step by step avoids multiplications with a weight in the inner loop
        float2 floorStep = rowDistance * (rayDir1 - rayDir0) / screenWidth;

        // real world coordinates of the leftmost column. This will be updated as we step to the right.
        float2 floorP = player->pos + rowDistance * rayDir0;
        in_render_dist = (rowDistance <= far_dist);
        if (in_render_dist)
        {

            for (int x = 0; x < screenWidth; x++)
            {
                int texWidth = floor_test.getWidth();
                int texHeight = floor_test.getHeight();
                //? the cell coord is simply got from the integer parts of floorX and floorY
                int cellX = (int)floorP.x;
                int cellY = (int)floorP.y;
                //? get the texture coordinate from the fractional part
                int tx = (int)(texWidth * (floorP.x - cellX)) & (texWidth - 1);
                int ty = (int)(texHeight * (floorP.y - cellY)) & (texHeight - 1);
                floorP += floorStep;
                //? choose texture and draw the pixel
                img::Color color;
                float s = SDL_clamp(1.0f / (rowDistance * rowDistance + 0.1), 0.0f, 1.0f);
                color = (floor_test)[texWidth * ty + tx];
                color = color * 0.7 * s; //? making the floors darker
                renderWindow->set_pixel(x, y, color);
            }
        }
    }
}

void Game::sortSprites(int amount)
{
    std::vector<std::pair<double, int>> sprites_(amount);
    for (int i = 0; i < amount; i++)
    {
        sprites_[i].first = spriteDistance[i];
        sprites_[i].second = spriteOrder[i];
    }
    std::sort(sprites_.begin(), sprites_.end());
    // restore in reverse order to go from farthest to nearest
    for (int i = 0; i < amount; i++)
    {
        spriteDistance[i] = sprites_[amount - i - 1].first;
        spriteOrder[i] = sprites_[amount - i - 1].second;
    }
}

void Game::Sprites()
{
    const float far_dist = ((mapHeight + mapWidth) / 2) * 0.3 * render_dist;
    bool in_render_dist = true;
    // SPRITE CASTING
    // sort sprites from far to close
    float posX = player->pos.x;
    float posY = player->pos.y;
    float dirX = player->dir.x;
    float dirY = player->dir.y;
    float planeX = player->plane.x;
    float planeY = player->plane.y;
    int w = RESOLUTION_WIDTH;
    int h = RESOLUTION_HEIGHT;
    for (int i = 0; i < numSprites; i++)
    {
        spriteOrder[i] = i;
        spriteDistance[i] = ((posX - sprites[i]->pos.x) * (posX - sprites[i]->pos.x) + (posY - sprites[i]->pos.y) * (posY - sprites[i]->pos.y)); // sqrt not taken, unneeded
    }
    sortSprites(numSprites);

    // after sorting the sprites, do the projection and draw them
    for (int i = 0; i < numSprites; i++)
    {
        // translate sprite position to relative to camera
        float2 spriteP = sprites[spriteOrder[i]]->pos - player->pos;
        in_render_dist = spriteDistance[i] <= far_dist;
        if (in_render_dist)
        {
            // transform sprite with the inverse camera matrix
            //  [ planeX   dirX ] -1                                       [ dirY      -dirX ]
            //  [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
            //  [ planeY   dirY ]                                          [ -planeY  planeX ]

            float invDet = 1.0 / (planeX * dirY - dirX * planeY); // required for correct matrix multiplication

            float transformX = invDet * (dirY * spriteP.x - dirX * spriteP.y);
            float transformY = invDet * (-planeY * spriteP.x + planeX * spriteP.y); // this is actually the depth inside the screen, that what Z is in 3D

            int spriteScreenX = int((w / 2) * (1 + transformX / transformY));

            // calculate height of the sprite on screen
            int spriteHeight = abs(int(h / (transformY))); // using 'transformY' instead of the real distance prevents fisheye
            // calculate lowest and highest pixel to fill in current stripe
            int drawStartY = -spriteHeight / 2 + h / 2;
            if (drawStartY < 0)
                drawStartY = 0;
            int drawEndY = spriteHeight / 2 + h / 2;
            if (drawEndY >= h)
                drawEndY = h - 1;

            // calculate width of the sprite
            int spriteWidth = abs(int(h / (transformY)));
            int drawStartX = -spriteWidth / 2 + spriteScreenX;
            if (drawStartX < 0)
                drawStartX = 0;
            int drawEndX = spriteWidth / 2 + spriteScreenX;
            if (drawEndX >= w)
                drawEndX = w - 1;
            
            float s = 2.0f / (1 + spriteDistance[i] * spriteDistance[i]);
            s = SDL_clamp(s, 0.0f, 1.0f);

            // loop through every vertical stripe of the sprite on screen
            int texWidth = sprites[i]->texture.getWidth();
            int texHeight = sprites[i]->texture.getHeight();
            for (int stripe = drawStartX; stripe < drawEndX; stripe++)
            {
                int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
                // the conditions in the if are:
                // 1) it's in front of camera plane so you don't see things behind you
                // 2) it's on the screen (left)
                // 3) it's on the screen (right)
                // 4) ZBuffer, with perpendicular distance
                if (transformY > 0 && stripe > 0 && stripe < w && transformY < ZBuffer[stripe])
                {
                    for (int y = drawStartY; y < drawEndY; y++) // for every pixel of the current stripe
                    {
                        int d = (y)*256 - h * 128 + spriteHeight * 128; // 256 and 128 factors to avoid floats
                        int texY = ((d * texHeight) / spriteHeight) / 256;
                        img::Color c = sprites[spriteOrder[i]]->texture(texX, texY);
                        renderWindow->set_pixel(stripe, y, c*s);
                    }
                }
            }
        }
    }
}