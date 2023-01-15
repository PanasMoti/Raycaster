#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glut.h>

#include "Game.h"

int main(int argc,char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    auto* game = new Game("RayCaster v1.0",1280,720);
    game->MainLoop();
    delete game;
    SDL_Quit();
    return 0;
}
