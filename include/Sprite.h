#ifndef _RAYCASTER_SPRITE_H_
#define _RAYCASTER_SPRITE_H_

#include "Image.h"
#include "imports/linalg.h"
using namespace linalg::aliases;
namespace img 
{
    struct Sprite
    {
        float2 pos;
        Image& texture; // reference to an image that will be a texture
        Sprite() = default;
        Sprite(float,float,Image&);
        Sprite(float2,Image&);
        Sprite(Image&);
        // Sprite(const Sprite& );
        static inline int counter = 0;
        ~Sprite();
    };
}


#endif // _RAYCASTER_SPRITE_H