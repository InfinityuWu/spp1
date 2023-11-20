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
    [[nodiscard]] std::array<BitmapImage::BitmapPixel, 16> get_row(index_type y, index_type x) const {
        std::array<BitmapImage::BitmapPixel, 16> row;
        if ((0 > y || height < y) ||
            (0 > x ||  width < x) ||
            (x % 16 != 0))
        {
            throw std::exception{};
        }
        for (int i = 0; i < 16; i++){
            row[i] = get_pixel(y, x+i);
        }

        return row;
    }

    void set_pixel(index_type y, index_type x, const BitmapPixel& new_pixel) {//konstante referenz
        if ((0 > y || height < y) ||
            (0 > x ||  width < x)) {
            throw std::exception{};
        }

        pictureVec.at(y).at(x) = new_pixel; //pixel wird kopiert
    }

    void set_row(index_type y, index_type x, std::array<BitmapImage::BitmapPixel, 16> row) const {
        if ((0 > y || height < y) ||
            (0 > x ||  width < x) ||
            (x % 16 != 0))
        {
            throw std::exception{};
        }
        for (int i = 0; i < 16; i++){
            set_pixel(y, x+i, row[i]);
        }
    }

    void set_block(index_type y, index_type x, std::array<std::array<row_type, 3>, 3> block) const {
        if ((0 > y || height < y) ||
            (0 > x ||  width < x) ||
            (x % 48 != 0 || y % 3 != 0))
        {
            throw std::exception{};
        }
        // actual dimensions
        int blockWidth = (current.at(0).at(2) != 0) ? 3 : ((current.at(0).at(1) != 0) ? 2 : 1);
        int blockHeight = (current.at(2).at(0) != 0) ? 3 : ((current.at(1).at(0) != 0) ? 2 : 1);

        for (int ix = 0; ix < blockWidth; ix++){
            for(int iy = 0; iy < blockHeight; iy++){
                set_row(y + iy, x + ix, block[iy][ix]);
            }
        }
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
