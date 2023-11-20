#pragma once

#include "Key.h"
#include "image/bitmap_image.h"
#include "../image/pixel.h"

class FES {

public:
    typedef Key::key_type key_type;
    typedef std::array<BitmapImage::BitmapPixel, 16> row_type;
    typedef std::array<std::array<row_type, 3>, 3> block_type;

public:

    static block_type convert_key_to_block(key_type key) {

        block_type block;
        row_type row = convert_key_to_row(key);
        for (int i = 0; i < block.size(); i++) {
            for (int j = 0; j < block.at(i).size(); j++) {
                block.at(i).at(j) = row;
            }
        }

        return block;
    }

    static row_type convert_key_to_row(key_type key) {

        row_type row;
        int j = 0;
        for (auto i = key_type::value_type(0); i < key.size(); i ++) {

            Pixel pixel = Pixel(key[i], key[i + 1], key[i +2]);
            row[j] = pixel;
            j++;
            i = i + 2; //weil wir immer 3 Elemente pro Pixel abarbeiten und KP warum in dem for nicht i +3 ging ?!?
        }

        return row;
    }

    //3c
    static row_type combine_rows(row_type current, row_type previous) {
        row_type row;
        for(int i = 0; i < current.size(); i++) {
            BitmapImage::BitmapPixel pixel = BitmapImage::BitmapPixel((current.at(i).get_red_channel() ^ previous.at(i).get_red_channel()),
                                (current.at(i).get_green_channel() ^ previous.at(i).get_green_channel()),
                                (current.at(i).get_blue_channel() ^ previous.at(i).get_blue_channel()));
            // combine_Pixel(current.at(i), previous.at(i));
            row.at(i) = pixel;
        }

        return row;
    }

    /*
    static Pixel<std::uint8_t> combine_Pixel(Pixel<std::uint8_t> current, Pixel<std::uint8_t> previous) { //woher soll ich wissen, was für einen Typ die channel haben ??
        return Pixel((current.get_red_channel() ^ previous.get_red_channel()),
                            (current.get_green_channel() ^ previous.get_green_channel()),
                            (current.get_blue_channel() ^ previous.get_blue_channel()));
    }
     */

    //3d FEHLT NOCH!!! NOCH KEINE RANDBEHANDLUNG
    static block_type encrypt_block(block_type current, block_type previous) {

        block_type encryptedBlock = block_type();
        row_type allSoFarCombinedRows;

		// actual dimensions
		int blockWidth = (current.at(0).at(2) != 0) ? 3 : ((current.at(0).at(1) != 0) ? 2 : 1);
		int blockHeight = (current.at(2).at(0) != 0) ? 3 : ((current.at(1).at(0) != 0) ? 2 : 1);

		if (blockWidth >= blockHeight) {
			
			for (int y = 0; y < blockHeight; y++) {
				for (int x = 0; x < blockWidth; x++) {
					encryptedBlock.at(y).at(x) = combine_rows(current.at(y).at(x), previous.at(y).at(x));
					for (int h = 0; h <= y; h++) for (int w = 0; w < x; w++) encrypt_block.at(y).at(x) = combine_rows(current.at(y).at(x), encryptedBlock.at(h).at(w));
				}
			}
			
		} else {
			
			for (int x = 0; x < blockWidth; x++) {
				for (int y = 0; y < blockHeight; y++) {
					encryptedBlock.at(y).at(x) = combine_rows(current.at(y).at(x), previous.at(y).at(x));
					for (int w = 0; w <= x; w++) for (int h = 0; h < y; h++) encrypt_block.at(y).at(x) = combine_rows(current.at(y).at(x), encryptedBlock.at(h).at(w));
				}
			}
			
		}

        return encryptedBlock;
    }


    //3e FEHLT NOCH!!!! Zur Not einfach nur den Methoden Kopf und ein NULL zurückgeben

    static BitmapImage encrypt(BitmapImage image, key_type key) {

        BitmapImage encryptedImage = BitmapImage(image.get_height(), image.get_width());
        block_type keyBlock = convert_key_to_block(key);

        for(int i = 0; i < image.get_height(); i + 3) {



        }

        return encryptedImage;
    }




};
