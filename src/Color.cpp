//
// Created by juicylucy on 12/30/22.
//

#include "Color.h"
#include <sstream>
namespace img {

    Color::Color(uint8_t r, uint8_t g, uint8_t b) {
        this->r = r; this->g = g; this->b = b;
    }

    Color::Color(int c) {
        auto c1 = static_cast<uint8_t>(c);
        this->r = c1; this->g = c1; this->b = c1;
    }

    Color::Color(const std::string &hex_code) {
        this->r = 0; this->g = 0; this->b = 0;
        const char* format = (hex_code.find('#') != std::string::npos) ? "#%02hhx%02hhx%02hhx" : "%02hhx%02hhx%02hhx";
        sscanf(hex_code.c_str(), format, &this->r, &this->g, &this->b);
    }

    Color::operator bool() const {
        return (this->r != TRANSPARENT_R || this->g != TRANSPARENT_G || this->b != TRANSPARENT_B);
    }

    Color Color::operator*(float p) const {
        auto nr = (uint8_t)((float)this->r*p);
        auto ng = (uint8_t)((float)this->g*p);
        auto nb = (uint8_t)((float)this->b*p);
        return {nr,ng,nb};
    }

    Color Color::operator/(float p) const {
        auto nr = (uint8_t)((float)this->r/p);
        auto ng = (uint8_t)((float)this->g/p);
        auto nb = (uint8_t)((float)this->b/p);
        return {nr,ng,nb};
    }

    Color Color::grayScale() const {

        auto nr = ((float)this->r*Color::GrayR);
        auto ng = ((float)this->g*Color::GrayG);
        auto nb = ((float)this->b*Color::GrayB);
        float avg = (nr+ng+nb)/3.0f;
        return Color(static_cast<uint8_t>(avg));
    }

    Color Color::operator!() const {
        return {static_cast<uint8_t>(255-this->r),static_cast<uint8_t>(255-this->g),static_cast<uint8_t>(255-this->b)};
    }

    uint8_t &Color::operator[](std::size_t i) {
        i %= 3; //overflow protection
        return (!i)? this->r :
               ((i==1)? this->g : this->b);
    }

    Color::operator uint32_t() const {
        return (this->r << 16 | this->g << 8 | this->b << 0);
    }





}// img
