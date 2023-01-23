#include "Sprite.h"

namespace img {
    Sprite::Sprite(Image& tex) : texture(tex) {
        pos = {0.0f,0.0f};
        Sprite::counter++;
    }
    Sprite::Sprite(float x,float y,Image& tex) : texture(tex) {
        pos = {x,y};
        Sprite::counter++;
    }
    Sprite::Sprite(float2 ppos,Image& tex) : texture(tex) {
        pos = ppos;
        Sprite::counter++;
    }
    Sprite::~Sprite() {
        Sprite::counter--;
    }
}