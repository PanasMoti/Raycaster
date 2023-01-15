//
// Created by juicylucy on 1/2/23.
//

#include "Game.h"

Game::Game(const char *t, int w, int h) : renderWindow(nullptr), player(nullptr)
{
    renderWindow = new RenderWindow(t, w, h);
    player = new Player();
    clock = new Clock();
    wall_test = new img::Image("assets/wall_.png");
    floor_test = new img::Image("assets/floor_test.png");
    ceil_test = new img::Image("assets/ceil_test.png");
    render_dist = 3;
}

Game::~Game()
{
    delete renderWindow;
    delete clock;
    delete player;
    delete wall_test;
    delete floor_test;
    delete ceil_test;
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
            int texWidth = (int)wall_test->getWidth();
            int texHeight = (int)wall_test->getHeight();
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
                float s = 2.0f/(1+perpWallDist*perpWallDist);
                s = SDL_clamp(s,0.0f,1.0f);
                img::Color color = (*wall_test)(texX, texY) * s;
                if(s < 0.15f) color = color.grayScale();
                renderWindow->set_pixel(x, y, color);
            }
        }
        // else {
        //     auto c = img::Color("#c0c0c0");
        //     renderWindow->vertLine(x,drawStart,drawEnd,c);

        // }
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
                int texWidth = floor_test->getWidth();
                int texHeight = floor_test->getHeight();
                //? the cell coord is simply got from the integer parts of floorX and floorY
                int cellX = (int)floorP.x;
                int cellY = (int)floorP.y;
                //? get the texture coordinate from the fractional part
                int tx = (int)(texWidth * (floorP.x - cellX)) & (texWidth - 1);
                int ty = (int)(texHeight * (floorP.y - cellY)) & (texHeight - 1);
                floorP += floorStep;
                //? choose texture and draw the pixel
                img::Color color;
                float s = SDL_clamp(1.0f/(rowDistance*rowDistance+0.1),0.0f,1.0f);
                color = (*floor_test)[texWidth * ty + tx];
                color = color * 0.7*s; //? making the floors darker
                renderWindow->set_pixel(x, y, color);
            }
        }
    }
}
