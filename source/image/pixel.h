#pragma once

// DO NOT CHANGE THIS ENUM
enum class ChannelOrder {
	BGR,
	BRG,
	GBR,
	GRB,
	RBG,
	RGB
};


#include <exception>
#include <cstdint>
#include <stdio.h>
#include <vector>
#include <iostream>

template <typename T> 
class Pixel {

public:

	Pixel() {
        red = 0;
        green = 0;
        blue = 0;
	}

	Pixel(T argument_red, T argument_green, T argument_blue) {
		red = argument_red; //oder = set_red_channel(argument_red);
		green = argument_green;
		blue = argument_blue;
	}

	~Pixel(){//destruktor!!!!

	}

	T get_red_channel() const noexcept{
		return red;
	}

	T get_green_channel() const noexcept{
		return green;
	}

    T get_blue_channel() const noexcept{
		return blue;
	}

	//brauchen vielleicht setter
    T set_red_channel (const T redNewValue) noexcept {
        T red_old = Pixel::red;
        Pixel::red = redNewValue;
        return red_old;
    }

    T set_green_channel (const T greenNewValue) noexcept {
        T green_old = Pixel::green;
        Pixel::green = greenNewValue;
        return green_old;
    }

    T set_blue_channel (const T blueNewValue) noexcept {
        T blue_old = Pixel::blue;
        Pixel::blue = blueNewValue;
        return blue_old;
    }


    bool operator== (const Pixel& other) const{
		return red == other.get_red_channel() && green == other.get_green_channel() && blue == other.get_blue_channel();
	}

	static const ChannelOrder channel_order = ChannelOrder::RGB;

private:

	T red, green, blue;

};

