//
// Created by juicylucy on 1/1/23.
//

#include "RenderWindow.h"

RenderWindow::RenderWindow(const char *t, int w, int h) : win(nullptr),ren(nullptr),buffer(nullptr){
    this->title = t;
    this->width = w;
    this->height = h;
    win = SDL_CreateWindow(t,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,w,h,SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win,-1, SDL_RENDERER_ACCELERATED);
    buffer = SDL_CreateRGBSurface(0,RESOLUTION_WIDTH,RESOLUTION_HEIGHT,32,0,0,0,0);
    SDL_LockSurface(buffer);
    SDL_memset(buffer->pixels,0,buffer->h*buffer->pitch);
    SDL_UnlockSurface(buffer);
    SDL_SetSurfaceRLE(buffer, true);

    ShouldClose = false;
    context = SDL_GL_CreateContext(win);
    SDL_GL_MakeCurrent(win,context);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "best" );
    SDL_SetHint(SDL_HINT_RENDER_VSYNC,"1");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER,"direct3d");

    Tbuffer = SDL_CreateTexture(ren,buffer->format->format,SDL_TEXTUREACCESS_TARGET,RESOLUTION_WIDTH,RESOLUTION_HEIGHT);
   
}


RenderWindow::~RenderWindow() {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_FreeSurface(buffer);
    SDL_DestroyTexture(Tbuffer);
}

void RenderWindow::updateBuffer() {
    SDL_UpdateTexture(Tbuffer,nullptr,buffer->pixels,buffer->pitch);
    SDL_RenderCopy(ren,Tbuffer,nullptr,nullptr);
}

void RenderWindow::clean_screen(const img::Color &bg) {
    // SDL_RenderClear(ren);
    SDL_LockSurface(buffer);
    SDL_memset4(buffer->pixels,uint32_t(bg),buffer->h*buffer->pitch/4);
    SDL_UnlockSurface(buffer);
}

void RenderWindow::display() {
    updateBuffer();
    SDL_RenderPresent(ren);
}



void RenderWindow::set_pixel(int x, int y, const img::Color &c) {
    if(!bool(c)) return; // if color is "transparent" -> don't render it
    auto pixel = (uint32_t)c;
    auto * const target_pixel = (Uint32 *) ((Uint8 *) buffer->pixels
                                              + y * buffer->pitch
                                              + x * buffer->format->BytesPerPixel);
  *target_pixel = pixel;
}

int RenderWindow::Width() const {
    return this->width;
}

int RenderWindow::Height() const {
    return this->height;
}

[[maybe_unused]] void RenderWindow::reloadBuffer() {
    buffer = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
    SDL_LockSurface(buffer);
    SDL_memset(buffer->pixels,0,buffer->h*buffer->pitch);
    SDL_UnlockSurface(buffer);
}

void RenderWindow::setWindowTitle(const std::string & ptitle) {
    if(this->title == ptitle) return;
    title = ptitle;
    SDL_SetWindowTitle(win,title.c_str());
}
void RenderWindow::set_rect(int x0,int y0,int x1,int y1,const img::Color& c) {
    SDL_Rect r;
    r.x = x0; r.y = y0; r.w = x1-x0; r.h = y1-y0;
    SDL_FillRect(buffer,&r,uint32_t(c));
}

void RenderWindow::vertLine(int x,int y0,int y1,const img::Color& c) {
    SDL_Rect r;
    r.x = x; r.y = y0; r.h = y1-y0; r.w = 1;
    SDL_FillRect(buffer,&r,uint32_t(c));
}
void RenderWindow::lockBuffer() {
    SDL_LockSurface(buffer);

}

void RenderWindow::unlockBuffer() {
    SDL_UnlockSurface(buffer);
}
