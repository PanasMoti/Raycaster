//
// Created by juicylucy on 1/2/23.
//

#ifndef RAYCASTER_PLAYER_H
#define RAYCASTER_PLAYER_H
#include "imports/linalg.h"
using namespace linalg::aliases;

struct Player {
    float2 pos = {22.0f,11.5f};
    float2 dir = {-1.0f,0.0f};
    float2 plane = {0.0f,0.66f};
    float2 speed = {5.0f,3.0f};
};



#endif //RAYCASTER_PLAYER_H
