#pragma once


#include "pixel.h"
#include <vector>

class BitmapImage {


public:
    typedef Pixel<std::uint8_t> BitmapPixel; //is this dependent type??? what does dependent type even mean dude?!?
    typedef int index_type;

    BitmapImage(int argument_height, int argument_width) {
        if ((1 > argument_height ||  8192 < argument_height) ||
            (1 > argument_width ||  8192 < argument_width)) {
            throw std::exception{};
        }

        height = argument_height;
        width = argument_width;

        for (int i=0; i<height; i++){
            std::vector<BitmapPixel> vec(width);
            pictureVec.push_back(vec);
        }
    }

    [[nodiscard]] int get_height() const {
        return height;
    }

    [[nodiscard]] int get_width() const {
        return width;
    }

    [[nodiscard]] BitmapPixel get_pixel(index_type y, index_type x) const {
        if ((0 > y || height < y) ||
            (0 > x ||  width < x)) {
            throw std::exception{};
        }

        return pictureVec.at(y).at(x);
    }

    void set_pixel(index_type y, index_type x, const BitmapPixel& new_pixel) {//konstante referenz
        if ((0 > y || height < y) ||
            (0 > x ||  width < x)) {
            throw std::exception{};
        }

        pictureVec.at(y).at(x) = new_pixel; //pixel wird kopiert
    }

    [[nodiscard]] BitmapImage transpose() const {
        BitmapImage transposedImage(width, height);
        for (index_type y = 0; y < height; y++) {
            for (index_type x = 0; x < width; x++) {
                transposedImage.set_pixel(y, x, get_pixel(x, y));		// random java-inspired things :)
            }
        }

        return transposedImage;
    }

private:

    int height, width;
    std::vector<std::vector<BitmapPixel>> pictureVec;

};
