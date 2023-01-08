//
// Created by juicylucy on 12/30/22.
//

#ifndef RAYCASTER_COLOR_H
#define RAYCASTER_COLOR_H


#include <SDL2/SDL_render.h>
#include <cstdint>
#include <ostream>
#define TRANSPARENT_R 163
#define TRANSPARENT_G 160
#define TRANSPARENT_B 0
namespace img {



    /// @brief a struct that represents a 24 bit rgb color
    /// @property r the red channel value   [0-255]
    /// @property g the green channel value [0-255]
    /// @property b the blue channel value  [0-255]
    struct Color {

        constexpr static const float GrayR = 0.2126;
        constexpr static const float GrayG = 0.7152;
        constexpr static const float GrayB = 0.0722;

        uint8_t r,g,b;

        Color() = default;
        Color(uint8_t r///<[in] : red channel value
              ,uint8_t g///<[in] : green channel value
              ,uint8_t b///<[in] : blue channel value
              );
        Color(int c///<[in] : all the channel values are equal to c
        );
        explicit Color(const std::string& hex_code ///<[in]: rgb value as a hex string@example eg: #00FF00 would be red=0 green = 255 blue = 0
        );
        /// @brief bool operator overloading, since we dont have an alpha channel in our struct we will define one very
        /// @brief specific color to be @a "transparent"
        /// @details our @a "transparent" color will be @b #a3a000
        /// @returns @b true if the color is @b not  @a "transparent" \n@b false otherwise
        explicit operator bool() const;

        /// @brief make a color brighter
        /// @details this @b is the @a official way to brighten up a color , @b( @b to @b darken @b it @b use @b the @b (/) @b operator @b)
        /// @param p : some constant that's bigger than 1 to multiply with
        /// @returns Color(r*p,g*p,b*p)
        Color operator*(float p) const;
        /// @brief make a color darker
        /// @details this @b is the @a official way to darken a color , @b( @b to @b brighten @b it @b use @b the @b (*) @b operator @b)
        /// @param p : some constant that's bigger than 1 to divide by
        /// @returns Color(r/p,g/p,b/p)
        Color operator/(float) const;
        /// @brief create a grayScale version of the color with same level of brightness
        /// @returns a color whose red green and blue values are equal to the weighted average of the three channels
        [[nodiscard]] Color grayScale() const;
        /// @brief create a @b negative version of the color by subtracting it from 255
        /// @returns the opposite color => (255-r,255-g,255-b)
        Color operator!() const;
        /// @brief access the red/green/blue values in the style of accessing an array
        /// @param i:the index (0->red,1->green,2->blue)
        /// @details if the input is bigger than 2 we use the @b (%) operator to prevent overflow
        uint8_t& operator[](std::size_t i);


        friend std::ostream& operator<<(std::ostream&,const Color&);


        ///@brief convert the rgb to hex value integer
        explicit operator uint32_t() const;
        ///@brief convert the rgb to hex string
        explicit operator std::string() const;
    };

    static void setRenderColor(SDL_Renderer *sdlRenderer, const img::Color &color) {
        SDL_SetRenderDrawColor(sdlRenderer,color.r,color.g,color.b,255);
    }

} // img

#endif //RAYCASTER_COLOR_H
