//
// Created by juicylucy on 1/1/23.
//

#include "Image.h"
#include <iostream>

namespace img {

    Image::Image(const std::string &pathToImage) {
        std::vector<unsigned char> data;
        this->width = 0; this->height = 0;
        unsigned error = lodepng::decode(data,width,height,pathToImage);
        if(error) {
            std::cerr << "decoder error" << error << ":" << lodepng_error_text(error) << '\n';
            std::cerr << "the file in question : " << pathToImage << "\n";
            this->NotFound();
        }
        else {
            for(int i = 0; i < data.size(); i+=4) {
                this->pixels.emplace_back(data[i],data[i+1],data[i+2]);
            }
        }
    }

    unsigned Image::getWidth() const {
        return this->width;
    }

    unsigned Image::getHeight() const {
        return height;
    }

    Color &Image::operator[](std::size_t i) {
        return this->pixels[i];
    }

    Color &Image::operator()(std::size_t x, std::size_t y) {
        return this->pixels[x + y*this->width];
    }

    void Image::NotFound() {
        this->width = 4;
        this->height = 4;
        Color c1("#000001");
        Color c2("#ff00ff");
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                if((i+j)%2) {
                    pixels.push_back(c1);
                }
                else {
                    pixels.push_back(c2);
                }
            }
        }

    }
} // img