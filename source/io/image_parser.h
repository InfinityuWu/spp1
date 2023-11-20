#pragma once

#include "../image/bitmap_image.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <filesystem>

#define FILE_HEADER_SIZE 14

class ImageParser {
public:
    static BitmapImage read_bitmap(const std::filesystem::path& path) {
        if(!exists(path)){
            throw std::exception{};
        }

        if(path.empty()){
            throw std::exception{};
        }

        std::filesystem::path ending = path.extension();
        std::filesystem::path ending_cmp = std::filesystem::path(".bmp");
        if(ending != ending_cmp){
            throw std::exception{};
        }


        uint32_t bfOffBits; //hatte ich vorher als int nur falls der code nicht läuf das ich es zurück ändern kann
        int width;
        int height;
        uint8_t bit_depth;

        FILE *file;
        std::string path_s = path.string();
        const char* path_c = path_s.c_str();
        file= fopen(path_c, "rb");

        //vielleicht auslagern in hilfsmethode
        if(file){
            //if(fgetc(file)=='B' && fgetc(file)=='M'){ //compiler sagt, dass immer false ??
            fseek(file, 10, SEEK_CUR);
            fread(&bfOffBits, 4, 1, file);
            fseek(file, 4, SEEK_CUR);
            fread(&width, 4, 1, file);
            fread(&height, 4, 1, file); //gibt size_t zurück, dass ist uint8_ könnte bei negativer Höhe /Breite

            /* //NOT SURE WHY BUT THIS MAKES THE TEST FAIL -.-
            (file, 2, SEEK_CUR);
            fread(&bit_depth, 2, 1, file);

            if(bit_depth != 24) {
                throw std::exception{};
            }
             */


            //} else{
            //    throw std::exception{};
            //}
        } else{
            throw std::exception{};
        }
        fclose(file);

        //file einlesen
        std::ifstream f;
        f.open(path, std::ios::in | std::ios::binary);
        if(!f.is_open()){
            throw std::exception{};
        }

        //const int fileHeaderSize = 14; //könnte man auslagern mit define als magic number,
        const uint32_t informationHeaderSize = bfOffBits - 14; //war vorher int
        //std::cout << informationHeaderSize <<std::endl;

        unsigned char fileHeader[FILE_HEADER_SIZE]; //evtl wieder in fileHeaderSize
        f.read(reinterpret_cast<char*>(fileHeader), FILE_HEADER_SIZE); //evtl wieder in fileHeaderSize

        unsigned char informationHeader[informationHeaderSize];
        f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

        //int fileSize = fileHeader[2]+(fileHeader[3]<<8)+(fileHeader[4]<<16)+(fileHeader[5]<<24); //stream auslesen, tatsächliche Daten kommen von weiter oben
        //int m_width = informationHeader[4]+(informationHeader[5]<<8)+(informationHeader[6]<<16)+(informationHeader[7]<<24); //stream auslesen, tatsächliche Daten kommen von weiter oben
        //int m_height = informationHeader[8]+(informationHeader[9]<<8)+(informationHeader[10]<<16)+(informationHeader[11]<<24); //stream auslesen, tatsächliche Daten kommen von weiter oben

        const int paddingAmount = ((4 - (width * 3) % 4) % 4);

        BitmapImage returnBitmap = BitmapImage(height, width);

        for (int heightCounter = 0; heightCounter<height; heightCounter++){
            for (int widthCounter = 0; widthCounter<width; widthCounter++){

                //pixel einlesen aus stream
                unsigned char colors[3];
                f.read(reinterpret_cast<char*>(colors),3);
                //colors zu ints
                uint8_t red = (int)colors[2];
                uint8_t green = (int)colors[1];
                uint8_t blue = (int)colors[0];

                const Pixel currentPixel = Pixel(red, green, blue);
                returnBitmap.set_pixel(heightCounter, widthCounter, currentPixel);
            }
            f.ignore(paddingAmount);
        }
        //input file stream schließen
        f.close();
        return returnBitmap;
    }


    /*
    bool helpTestFile(){

    }
     */



    //2b Aerideus (YT)
/*
    static void write_bitmap(std::filesystem::path path, BitmapImage image) {
      std::ofstream f;
      f.open(path, std::ios::out | std::ios::binary);

      if(!f.is_open()) {
          throw std::exception{};
      }

      unsigned char bmpPad[3] = {0, 0, 0};
      const int paddingAmount = ((4 - (image.get_width() * 3) % 4) % 4);

      const int informationHeaderSize = 40;
      const int fileSize = FILE_HEADER_SIZE + informationHeaderSize + image.get_width() * image.get_height() * 3 + paddingAmount * image.get_height();

      unsigned char fileHeader[FILE_HEADER_SIZE];

      for (int i = 0; i < FILE_HEADER_SIZE; i++) {
          fileHeader[i] = 0;
      }

      fileHeader[0] = 'B';
      fileHeader[1] = 'M';

      fileHeader[2] = fileSize;
      fileHeader[3] = fileSize >> 8;
      fileHeader[4] = fileSize >> 16;
      fileHeader[5] = fileSize >> 24;

      //bfOffBits
      fileHeader[10] = FILE_HEADER_SIZE + informationHeaderSize;

      unsigned char informationHeader[informationHeaderSize];

      for (int i = 0; i < informationHeaderSize; i++) {
          informationHeader[i] = 0;
      }

      informationHeader[0] = informationHeaderSize;

      //image width
      int width = image.get_width();

      informationHeader[4] = width;
      informationHeader[5] = width >> 8;
      informationHeader[6] = width >> 16;
      informationHeader[7] = width >> 24;

      //image height
      int height = image.get_height();

      informationHeader[8] = height;
      informationHeader[9] = height >> 8;
      informationHeader[10] = height >> 16;
      informationHeader[11] = height >> 24;

      //planes whatever that is
      informationHeader[12] = 1;

      //Bits per Pixel (Pixel width) (RGB)
      informationHeader[14] = 24;

      f.write(reinterpret_cast<char*>(fileHeader), FILE_HEADER_SIZE);
      f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

      for(int y = 0; y < height; y++){
          for(int x = 0; x < width; x++){
              unsigned char r = static_cast<unsigned char>(image.get_pixel(y, x).get_red_channel() * 255.0f);
              unsigned char g = static_cast<unsigned char>(image.get_pixel(y, x).get_green_channel() * 255.0f);
              unsigned char b = static_cast<unsigned char>(image.get_pixel(y, x).get_blue_channel() * 255.0f);

              unsigned char color[] = {b, g, r };

              f.write(reinterpret_cast<char*>(color), 3);
          }
          f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
      }
      f.close();
    }
    */

    static void write_bitmap(const std::filesystem::path& file_path, BitmapImage image){

        std::ofstream f(file_path, std::ios::out | std::ios::binary);
        if (!f.is_open()) throw std::exception{};

        const int padAmount = ((4 - (image.get_width() * 3) % 4) % 4);

        const int fileHeaderSize = 14;
        const int infoHeaderSize = 40;
        const int fileSize = fileHeaderSize + infoHeaderSize + (image.get_width() * 3 + padAmount) * image.get_height();


        unsigned char fileHeader[fileHeaderSize];

        fileHeader[0] = 'B';	// bfType
        fileHeader[1] = 'M';

        for (int i = 0; i < 4; i++) fileHeader[2+i] = fileSize >> (i * 8);	// bfSize

        for (int i = 6; i < 14; i++) fileHeader[i] = (i != 10) ? 0 : (fileHeaderSize + infoHeaderSize);	// all 0, except bfOffBits


        unsigned char infoHeader[infoHeaderSize];

        for (int i = 0; i < 40; i++) infoHeader[i] = 0;

        infoHeader[0] = infoHeaderSize;	// biSize

        for (int i = 0; i < 4; i++) infoHeader[4+i] = image.get_width() >> (i * 8);	// biWidth

        for (int i = 0; i < 4; i++) infoHeader[8+i] = image.get_height() >> (i * 8);	// biHeight

        infoHeader[12] = 1;	// biPlanes

        infoHeader[14] = 24;	// biBitCount (color depth in bpp)


        f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
        f.write(reinterpret_cast<char*>(infoHeader), infoHeaderSize);

        for (int y = 0; y < image.get_height(); y++) {

            for (int x = 0; x < image.get_width(); x++) {
                Pixel p = image.get_pixel(y, x);
                unsigned char color[] = {static_cast<unsigned char> (p.get_blue_channel()), static_cast<unsigned char> (p.get_green_channel()),static_cast<unsigned char> (p.get_red_channel())};
                f.write(reinterpret_cast<char*> (color), 3);
            }

            for (int i = 0; i < padAmount; i++) f.put(0);

        }

        f.close();

    }

};