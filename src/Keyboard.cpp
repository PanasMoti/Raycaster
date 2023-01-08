//
// Created by juicylucy on 1/2/23.
//

#include "Keyboard.h"

Keyboard::Keyboard() {
    this->keys;
}

bool &Keyboard::operator[](SDL_KeyCode code) {
    if(keys.find(code) == keys.end()) {
        // if the key doesn't exist in our keyboard we add it and init to false
        keys.insert({code,false});
    }
    return keys[code];
}

void Keyboard::Press(SDL_KeyCode code) {
    if(keys.find(code) == keys.end()) {
        keys.insert({code,false});
    }
    keys[code] = true;
}

void Keyboard::Release(SDL_KeyCode code) {
    if(keys.find(code) == keys.end()) {
        keys.insert({code,false});
    }
    keys[code] = false;
}
