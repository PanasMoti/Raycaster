//
// Created by juicylucy on 1/1/23.
//

#ifndef RAYCASTER_IMAGE_H
#define RAYCASTER_IMAGE_H
#include "Color.h"
#include <vector>
#include "imports/lodepng.h"


namespace img {

    //todo : add documentation

    class Image {
    public:
        explicit Image(const std::string&);
        [[nodiscard]] unsigned getWidth() const; [[nodiscard]] unsigned getHeight() const;
        Color& operator[](std::size_t);
        Color& operator()(std::size_t,std::size_t);

    protected:
        void NotFound();
        std::vector<Color> pixels;
        unsigned width,height;

    };

} // img

#endif //RAYCASTER_IMAGE_H
