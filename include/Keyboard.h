//
// Created by juicylucy on 1/2/23.
//

#ifndef RAYCASTER_KEYBOARD_H
#define RAYCASTER_KEYBOARD_H
#include <SDL2/SDL.h>
#include <unordered_map>
class Keyboard {
public:
    Keyboard();
    bool& operator[](SDL_KeyCode code);
    void Press(SDL_KeyCode code);
    void Release(SDL_KeyCode code);
private:
    std::unordered_map<SDL_KeyCode,bool> keys;
};


#endif //RAYCASTER_KEYBOARD_H
