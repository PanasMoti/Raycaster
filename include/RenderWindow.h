//
// Created by juicylucy on 1/1/23.
//

#ifndef RAYCASTER_RENDERWINDOW_H
#define RAYCASTER_RENDERWINDOW_H
#include <SDL2/SDL.h>
#include <iostream>
#include "Color.h"
#include <GL/glut.h>
#define res 3

#define RESOLUTION_WIDTH 160*res
#define RESOLUTION_HEIGHT 120*res

class RenderWindow {
public:
    RenderWindow(const char* t,int w,int h);
    ~RenderWindow();
    void clean_screen(const img::Color& bg);
    void display();
    void set_pixel(int x,int y,const img::Color& c);
    bool ShouldClose;

    [[maybe_unused]] void reloadBuffer();
    [[nodiscard]] int Width() const;
    [[nodiscard]] int Height() const;
    void setWindowTitle(const std::string &ptitle);
    void lockBuffer();
    void unlockBuffer();
private:
    SDL_Renderer* ren;
    SDL_Window* win;
    SDL_Surface* buffer;
    SDL_GLContext context;
    std::string title;
    int height,width;
};


#endif //RAYCASTER_RENDERWINDOW_H
