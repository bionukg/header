//////////////////////////////////ANSI
#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS
// #define fwrite(buffer,size,leng,fp) {fwrite(buffer,size,leng,fp);fw++;}

#ifndef _bionukg_graphics_h
#define _bionukg_graphics_h

#include <iostream>
// #include<Windows.h>
#ifndef MAX_PATH
#define MAX_PATH 260
#endif // !MAX_PATH

// #include"./bionukg_optimize.h"

#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg
	// width*height size bitmap
	template <int width, int height>
	class bitPic
	{
	public:
// data, width in bits should be 8,16,32,so on
// so width in BYTEs should be 1,2,4,so on
#define widthBYTE ((width + 7) / 8)
		uint8_t data[widthBYTE * height];
		bitPic()
		{
			memset(data, 0, sizeof(data));
		}
		// write bit at x,y
		void write(int x, int y, bool bit)
		{
			if (x < 0 || x >= width || y < 0 || y >= height)
				return;
			if (bit)
			{
				data[y * widthBYTE + x / 8] |= (1 << (x % 8));
			}
			else
			{
				data[y * widthBYTE + x / 8] &= ~(1 << (x % 8));
			}
		}
		// write_unsafe bit at x,y, not check if x,y is out of range
		void write_unsafe(int x, int y, bool bit)
		{
			if (bit)
			{
				data[y * widthBYTE + x / 8] |= (1 << (x % 8));
			}
			else
			{
				data[y * widthBYTE + x / 8] &= ~(1 << (x % 8));
			}
		}
		// read bit at x,y
		bool read(int x, int y)
		{
			if (x < 0 || x >= width || y < 0 || y >= height)
				return 0;
			return data[y * widthBYTE + x / 8] & (1 << (x % 8));
		}
		// read_unsafe bit at x,y, not check if x,y is out of range
		bool read_unsafe(int x, int y)
		{
			return ((data[y * widthBYTE + x / 8] >> (x % 8)) & 1) == 1 ? true : false;
		}
#undef widthBYTE
	};

	class text2pic
	{
	public:
		int width;
		int height;
		text2pic(int w, int h)
		{
			width = w;
			height = h;
		}
	};

	// input an ascii char, output a 6x11 bitmap
	class text2pic_7x12
	{
	public:
		bitPic<7, 12> pic;
		text2pic_7x12(){};
		// make: input an ascii char, turn it into a bitmap in piv
		// return 0 if success, return -1 if c is not printable

		void font1(char c, uint8_t bits[12])
		{
			switch (c)
			{
			case ' ':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '!':
				bits[0] = 0b00000000;
				bits[1] = 0b00010000;
				bits[2] = 0b00010000;
				bits[3] = 0b00010000;
				bits[4] = 0b00010000;
				bits[5] = 0b00010000;
				bits[6] = 0b00010000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00010000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '"':
				bits[0] = 0b00010100;
				bits[1] = 0b00010100;
				bits[2] = 0b00101000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '#':
				bits[0] = 0b00000000;
				bits[1] = 0b00010010;
				bits[2] = 0b00010010;
				bits[3] = 0b01111110;
				bits[4] = 0b00010010;
				bits[5] = 0b00100100;
				bits[6] = 0b00100100;
				bits[7] = 0b01111110;
				bits[8] = 0b00100100;
				bits[9] = 0b00100100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '$':
				bits[0] = 0b00001000;
				bits[1] = 0b00011100;
				bits[2] = 0b00101010;
				bits[3] = 0b00101000;
				bits[4] = 0b00011000;
				bits[5] = 0b00001100;
				bits[6] = 0b00001010;
				bits[7] = 0b00101010;
				bits[8] = 0b00101010;
				bits[9] = 0b00011100;
				bits[10] = 0b0001000;
				bits[11] = 0b0000000;
				break;
			case '%':
				bits[0] = 0b00000000;
				bits[1] = 0b00100100;
				bits[2] = 0b01010100;
				bits[3] = 0b01011000;
				bits[4] = 0b01011000;
				bits[5] = 0b00110100;
				bits[6] = 0b00011010;
				bits[7] = 0b00011010;
				bits[8] = 0b00101010;
				bits[9] = 0b00100100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '&':
				bits[0] = 0b00000000;
				bits[1] = 0b00010000;
				bits[2] = 0b00101000;
				bits[3] = 0b00101000;
				bits[4] = 0b00110000;
				bits[5] = 0b00100110;
				bits[6] = 0b01010100;
				bits[7] = 0b01010100;
				bits[8] = 0b01001001;
				bits[9] = 0b00110110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '\'':
				bits[0] = 0b00110000;
				bits[1] = 0b00010000;
				bits[2] = 0b00100000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '(':
				bits[0] = 0b00000010;
				bits[1] = 0b00000100;
				bits[2] = 0b00000100;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00000100;
				bits[9] = 0b00000100;
				bits[10] = 0b0000010;
				bits[11] = 0b0000000;
				break;
			case ')':
				bits[0] = 0b00100000;
				bits[1] = 0b00010000;
				bits[2] = 0b00010000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00010000;
				bits[9] = 0b00010000;
				bits[10] = 0b0100000;
				bits[11] = 0b0000000;
				break;
			case '*':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00001000;
				bits[3] = 0b00101010;
				bits[4] = 0b00011100;
				bits[5] = 0b00011100;
				bits[6] = 0b00101010;
				bits[7] = 0b00001000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '+':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00001000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b01111111;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case ',':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00110000;
				bits[9] = 0b00010000;
				bits[10] = 0b0100000;
				bits[11] = 0b0000000;
				break;
			case '-':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b01111110;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '.':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00110000;
				bits[9] = 0b00110000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '/':
				bits[0] = 0b00000010;
				bits[1] = 0b00000100;
				bits[2] = 0b00000100;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00010000;
				bits[6] = 0b00010000;
				bits[7] = 0b00010000;
				bits[8] = 0b00100000;
				bits[9] = 0b00100000;
				bits[10] = 0b1000000;
				bits[11] = 0b0000000;
				break;
			case '0':
				bits[0] = 0b00000000;
				bits[1] = 0b00011100;
				bits[2] = 0b00100010;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00100010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00011100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '1':
				bits[0] = 0b00000000;
				bits[1] = 0b00001000;
				bits[2] = 0b00011000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00011100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '2':
				bits[0] = 0b00000000;
				bits[1] = 0b00011100;
				bits[2] = 0b00100010;
				bits[3] = 0b00100010;
				bits[4] = 0b00000010;
				bits[5] = 0b00000100;
				bits[6] = 0b00001000;
				bits[7] = 0b00010000;
				bits[8] = 0b00100010;
				bits[9] = 0b00111110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '3':
				bits[0] = 0b00000000;
				bits[1] = 0b00011100;
				bits[2] = 0b00100010;
				bits[3] = 0b00000010;
				bits[4] = 0b00000010;
				bits[5] = 0b00001100;
				bits[6] = 0b00000010;
				bits[7] = 0b00000010;
				bits[8] = 0b00100010;
				bits[9] = 0b00011100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '4':
				bits[0] = 0b00000000;
				bits[1] = 0b00000100;
				bits[2] = 0b00000100;
				bits[3] = 0b00001100;
				bits[4] = 0b00010100;
				bits[5] = 0b00010100;
				bits[6] = 0b00100100;
				bits[7] = 0b00111110;
				bits[8] = 0b00000100;
				bits[9] = 0b00001110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '5':
				bits[0] = 0b00000000;
				bits[1] = 0b00111110;
				bits[2] = 0b00100000;
				bits[3] = 0b00100000;
				bits[4] = 0b00111100;
				bits[5] = 0b00100010;
				bits[6] = 0b00000010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00011100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '6':
				bits[0] = 0b00000000;
				bits[1] = 0b00001100;
				bits[2] = 0b00010010;
				bits[3] = 0b00100000;
				bits[4] = 0b00101100;
				bits[5] = 0b00110010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00011100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '7':
				bits[0] = 0b00000000;
				bits[1] = 0b00111110;
				bits[2] = 0b00100010;
				bits[3] = 0b00000100;
				bits[4] = 0b00000100;
				bits[5] = 0b00000100;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '8':
				bits[0] = 0b00000000;
				bits[1] = 0b00011100;
				bits[2] = 0b00100010;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00011100;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00011100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '9':
				bits[0] = 0b00000000;
				bits[1] = 0b00011100;
				bits[2] = 0b00100010;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00100110;
				bits[6] = 0b00011010;
				bits[7] = 0b00000010;
				bits[8] = 0b00100100;
				bits[9] = 0b00011000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case ':':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00010000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00010000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case ';':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00001000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00001000;
				bits[10] = 0b0001000;
				bits[11] = 0b0000000;
				break;
			case '<':
				bits[0] = 0b00000000;
				bits[1] = 0b00000010;
				bits[2] = 0b00000100;
				bits[3] = 0b00001000;
				bits[4] = 0b00010000;
				bits[5] = 0b00100000;
				bits[6] = 0b00010000;
				bits[7] = 0b00001000;
				bits[8] = 0b00000100;
				bits[9] = 0b00000010;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '=':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b01111110;
				bits[5] = 0b00000000;
				bits[6] = 0b01111110;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '>':
				bits[0] = 0b00000000;
				bits[1] = 0b00100000;
				bits[2] = 0b00010000;
				bits[3] = 0b00001000;
				bits[4] = 0b00000100;
				bits[5] = 0b00000010;
				bits[6] = 0b00000100;
				bits[7] = 0b00001000;
				bits[8] = 0b00010000;
				bits[9] = 0b00100000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '?':
				bits[0] = 0b00000000;
				bits[1] = 0b00011100;
				bits[2] = 0b00100010;
				bits[3] = 0b00100010;
				bits[4] = 0b00000010;
				bits[5] = 0b00000100;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00000000;
				bits[9] = 0b00001000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '@':
				bits[0] = 0b00000000;
				bits[1] = 0b00011110;
				bits[2] = 0b00100001;
				bits[3] = 0b01001101;
				bits[4] = 0b01010101;
				bits[5] = 0b01010101;
				bits[6] = 0b01010101;
				bits[7] = 0b01001110;
				bits[8] = 0b00100001;
				bits[9] = 0b00011110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'A':
				bits[0] = 0b00000000;
				bits[1] = 0b00001000;
				bits[2] = 0b00001000;
				bits[3] = 0b00010100;
				bits[4] = 0b00010100;
				bits[5] = 0b00010100;
				bits[6] = 0b00011100;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b01110111;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'B':
				bits[0] = 0b00000000;
				bits[1] = 0b01111100;
				bits[2] = 0b00100010;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00111100;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b01111100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'C':
				bits[0] = 0b00000000;
				bits[1] = 0b00011110;
				bits[2] = 0b00100010;
				bits[3] = 0b01000000;
				bits[4] = 0b01000000;
				bits[5] = 0b01000000;
				bits[6] = 0b01000000;
				bits[7] = 0b01000000;
				bits[8] = 0b00100010;
				bits[9] = 0b00011100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'D':
				bits[0] = 0b00000000;
				bits[1] = 0b01111000;
				bits[2] = 0b00100100;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00100010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100100;
				bits[9] = 0b01111000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'E':
				bits[0] = 0b00000000;
				bits[1] = 0b01111110;
				bits[2] = 0b00100010;
				bits[3] = 0b00100000;
				bits[4] = 0b00100100;
				bits[5] = 0b00111100;
				bits[6] = 0b00100100;
				bits[7] = 0b00100000;
				bits[8] = 0b00100010;
				bits[9] = 0b01111110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'F':
				bits[0] = 0b00000000;
				bits[1] = 0b01111110;
				bits[2] = 0b00100010;
				bits[3] = 0b00100000;
				bits[4] = 0b00100100;
				bits[5] = 0b00111100;
				bits[6] = 0b00100100;
				bits[7] = 0b00100000;
				bits[8] = 0b00100000;
				bits[9] = 0b01110000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'G':
				bits[0] = 0b00000000;
				bits[1] = 0b00011100;
				bits[2] = 0b00100100;
				bits[3] = 0b01000000;
				bits[4] = 0b01000000;
				bits[5] = 0b01000000;
				bits[6] = 0b01001110;
				bits[7] = 0b01000100;
				bits[8] = 0b00100100;
				bits[9] = 0b00011000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'H':
				bits[0] = 0b00000000;
				bits[1] = 0b01110111;
				bits[2] = 0b00100010;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00111110;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b01110111;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'I':
				bits[0] = 0b00000000;
				bits[1] = 0b00111110;
				bits[2] = 0b00001000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00111110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'J':
				bits[0] = 0b00000000;
				bits[1] = 0b00111110;
				bits[2] = 0b00001000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b1001000;
				bits[11] = 0b1110000;
				break;
			case 'K':
				bits[0] = 0b00000000;
				bits[1] = 0b01110110;
				bits[2] = 0b00100100;
				bits[3] = 0b00101000;
				bits[4] = 0b00110000;
				bits[5] = 0b00111000;
				bits[6] = 0b00101000;
				bits[7] = 0b00100100;
				bits[8] = 0b00100100;
				bits[9] = 0b01110110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'L':
				bits[0] = 0b00000000;
				bits[1] = 0b01110000;
				bits[2] = 0b00100000;
				bits[3] = 0b00100000;
				bits[4] = 0b00100000;
				bits[5] = 0b00100000;
				bits[6] = 0b00100000;
				bits[7] = 0b00100000;
				bits[8] = 0b00100010;
				bits[9] = 0b01111110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'M':
				bits[0] = 0b00000000;
				bits[1] = 0b01110111;
				bits[2] = 0b00110110;
				bits[3] = 0b00110110;
				bits[4] = 0b00110110;
				bits[5] = 0b00101010;
				bits[6] = 0b00101010;
				bits[7] = 0b00101010;
				bits[8] = 0b00101010;
				bits[9] = 0b01101011;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'N':
				bits[0] = 0b00000000;
				bits[1] = 0b01110111;
				bits[2] = 0b00110010;
				bits[3] = 0b00110010;
				bits[4] = 0b00101010;
				bits[5] = 0b00101010;
				bits[6] = 0b00101010;
				bits[7] = 0b00100110;
				bits[8] = 0b00100110;
				bits[9] = 0b01110010;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'O':
				bits[0] = 0b00000000;
				bits[1] = 0b00011000;
				bits[2] = 0b00100100;
				bits[3] = 0b01000010;
				bits[4] = 0b01000010;
				bits[5] = 0b01000010;
				bits[6] = 0b01000010;
				bits[7] = 0b01000010;
				bits[8] = 0b00100100;
				bits[9] = 0b00011000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'P':
				bits[0] = 0b00000000;
				bits[1] = 0b01111100;
				bits[2] = 0b00100010;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00111100;
				bits[6] = 0b00100000;
				bits[7] = 0b00100000;
				bits[8] = 0b00100000;
				bits[9] = 0b01110000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'Q':
				bits[0] = 0b00000000;
				bits[1] = 0b00011000;
				bits[2] = 0b00100100;
				bits[3] = 0b01000010;
				bits[4] = 0b01000010;
				bits[5] = 0b01000010;
				bits[6] = 0b01000010;
				bits[7] = 0b01011010;
				bits[8] = 0b00100100;
				bits[9] = 0b00011100;
				bits[10] = 0b0000110;
				bits[11] = 0b0000000;
				break;
			case 'R':
				bits[0] = 0b00000000;
				bits[1] = 0b01111100;
				bits[2] = 0b00100010;
				bits[3] = 0b00100010;
				bits[4] = 0b00111100;
				bits[5] = 0b00101000;
				bits[6] = 0b00100100;
				bits[7] = 0b00100100;
				bits[8] = 0b00100010;
				bits[9] = 0b01110011;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'S':
				bits[0] = 0b00000000;
				bits[1] = 0b00011110;
				bits[2] = 0b00100010;
				bits[3] = 0b00100000;
				bits[4] = 0b00010000;
				bits[5] = 0b00001100;
				bits[6] = 0b00000010;
				bits[7] = 0b00000010;
				bits[8] = 0b00100010;
				bits[9] = 0b00111100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'T':
				bits[0] = 0b00000000;
				bits[1] = 0b00111110;
				bits[2] = 0b00101010;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00011100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'U':
				bits[0] = 0b00000000;
				bits[1] = 0b01110111;
				bits[2] = 0b00100010;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00100010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00011100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'V':
				bits[0] = 0b00000000;
				bits[1] = 0b01110111;
				bits[2] = 0b00100010;
				bits[3] = 0b00100010;
				bits[4] = 0b00010100;
				bits[5] = 0b00010100;
				bits[6] = 0b00010100;
				bits[7] = 0b00010100;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'W':
				bits[0] = 0b00000000;
				bits[1] = 0b01101011;
				bits[2] = 0b00101010;
				bits[3] = 0b00101010;
				bits[4] = 0b00101010;
				bits[5] = 0b00101010;
				bits[6] = 0b00110110;
				bits[7] = 0b00010100;
				bits[8] = 0b00010100;
				bits[9] = 0b00010100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'X':
				bits[0] = 0b00000000;
				bits[1] = 0b01100110;
				bits[2] = 0b00100100;
				bits[3] = 0b00100100;
				bits[4] = 0b00011000;
				bits[5] = 0b00011000;
				bits[6] = 0b00011000;
				bits[7] = 0b00100100;
				bits[8] = 0b00100100;
				bits[9] = 0b01100110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'Y':
				bits[0] = 0b00000000;
				bits[1] = 0b01110111;
				bits[2] = 0b00100010;
				bits[3] = 0b00010100;
				bits[4] = 0b00010100;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00011100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'Z':
				bits[0] = 0b00000000;
				bits[1] = 0b00111110;
				bits[2] = 0b00100100;
				bits[3] = 0b00000100;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00010000;
				bits[8] = 0b00010010;
				bits[9] = 0b00111110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '[':
				bits[0] = 0b00001110;
				bits[1] = 0b00001000;
				bits[2] = 0b00001000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0001110;
				bits[11] = 0b0000000;
				break;
			case '\\':
				bits[0] = 0b00000000;
				bits[1] = 0b00100000;
				bits[2] = 0b00100000;
				bits[3] = 0b00010000;
				bits[4] = 0b00010000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00000100;
				bits[8] = 0b00000100;
				bits[9] = 0b00000010;
				bits[10] = 0b0000010;
				bits[11] = 0b0000000;
				break;
			case ']':
				bits[0] = 0b00111000;
				bits[1] = 0b00001000;
				bits[2] = 0b00001000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0111000;
				bits[11] = 0b0000000;
				break;
			case '^':
				bits[0] = 0b00010100;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '_':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b1111110;
				break;
			case '`':
				bits[0] = 0b00001000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'a':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00011100;
				bits[5] = 0b00100010;
				bits[6] = 0b00001110;
				bits[7] = 0b00010010;
				bits[8] = 0b00100010;
				bits[9] = 0b00011111;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'b':
				bits[0] = 0b00000000;
				bits[1] = 0b01100000;
				bits[2] = 0b00100000;
				bits[3] = 0b00100000;
				bits[4] = 0b00111100;
				bits[5] = 0b00100010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00111100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'c':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00011110;
				bits[5] = 0b00100010;
				bits[6] = 0b00100000;
				bits[7] = 0b00100000;
				bits[8] = 0b00100010;
				bits[9] = 0b00011100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'd':
				bits[0] = 0b00000000;
				bits[1] = 0b00000110;
				bits[2] = 0b00000010;
				bits[3] = 0b00000010;
				bits[4] = 0b00011110;
				bits[5] = 0b00100010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00011111;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'e':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00011100;
				bits[5] = 0b00100010;
				bits[6] = 0b00111110;
				bits[7] = 0b00100000;
				bits[8] = 0b00100010;
				bits[9] = 0b00011100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'f':
				bits[0] = 0b00000000;
				bits[1] = 0b00001100;
				bits[2] = 0b00010010;
				bits[3] = 0b00010000;
				bits[4] = 0b01111100;
				bits[5] = 0b00010000;
				bits[6] = 0b00010000;
				bits[7] = 0b00010000;
				bits[8] = 0b00010000;
				bits[9] = 0b00111100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'g':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00011110;
				bits[5] = 0b00100100;
				bits[6] = 0b00100100;
				bits[7] = 0b00011000;
				bits[8] = 0b00100000;
				bits[9] = 0b00011100;
				bits[10] = 0b0100010;
				bits[11] = 0b0011100;
				break;
			case 'h':
				bits[0] = 0b00000000;
				bits[1] = 0b01100000;
				bits[2] = 0b00100000;
				bits[3] = 0b00100000;
				bits[4] = 0b00101100;
				bits[5] = 0b00110010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b01110111;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'i':
				bits[0] = 0b00000000;
				bits[1] = 0b00001000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00111000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00111110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'j':
				bits[0] = 0b00000000;
				bits[1] = 0b00000100;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00011100;
				bits[5] = 0b00000100;
				bits[6] = 0b00000100;
				bits[7] = 0b00000100;
				bits[8] = 0b00000100;
				bits[9] = 0b00000100;
				bits[10] = 0b0000100;
				bits[11] = 0b0111000;
				break;
			case 'k':
				bits[0] = 0b00000000;
				bits[1] = 0b01100000;
				bits[2] = 0b00100000;
				bits[3] = 0b00100000;
				bits[4] = 0b00101110;
				bits[5] = 0b00100100;
				bits[6] = 0b00101000;
				bits[7] = 0b00111000;
				bits[8] = 0b00100100;
				bits[9] = 0b01110110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'l':
				bits[0] = 0b00000000;
				bits[1] = 0b00111000;
				bits[2] = 0b00001000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00111110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'm':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b01111100;
				bits[5] = 0b00101010;
				bits[6] = 0b00101010;
				bits[7] = 0b00101010;
				bits[8] = 0b00101010;
				bits[9] = 0b01101011;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'n':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b01101100;
				bits[5] = 0b00110010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b01110111;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'o':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00011100;
				bits[5] = 0b00100010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00011100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'p':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b01111100;
				bits[5] = 0b00100010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00111100;
				bits[10] = 0b0100000;
				bits[11] = 0b1110000;
				break;
			case 'q':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00011110;
				bits[5] = 0b00100010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00011110;
				bits[10] = 0b0000010;
				bits[11] = 0b0000111;
				break;
			case 'r':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00110110;
				bits[5] = 0b00011000;
				bits[6] = 0b00010000;
				bits[7] = 0b00010000;
				bits[8] = 0b00010000;
				bits[9] = 0b00111100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 's':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00011110;
				bits[5] = 0b00100010;
				bits[6] = 0b00011000;
				bits[7] = 0b00000100;
				bits[8] = 0b00100010;
				bits[9] = 0b00111100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 't':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00010000;
				bits[3] = 0b00010000;
				bits[4] = 0b01111100;
				bits[5] = 0b00010000;
				bits[6] = 0b00010000;
				bits[7] = 0b00010000;
				bits[8] = 0b00010010;
				bits[9] = 0b00001100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'u':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b01100110;
				bits[5] = 0b00100010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100110;
				bits[9] = 0b00011011;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'v':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b01100011;
				bits[5] = 0b00100010;
				bits[6] = 0b00100010;
				bits[7] = 0b00010100;
				bits[8] = 0b00010100;
				bits[9] = 0b00001000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'w':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b01101011;
				bits[5] = 0b00101010;
				bits[6] = 0b00101010;
				bits[7] = 0b00101010;
				bits[8] = 0b00010100;
				bits[9] = 0b00010100;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'x':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b01110110;
				bits[5] = 0b00100100;
				bits[6] = 0b00011000;
				bits[7] = 0b00011000;
				bits[8] = 0b00100100;
				bits[9] = 0b01101110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case 'y':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b01110111;
				bits[5] = 0b00100010;
				bits[6] = 0b00010100;
				bits[7] = 0b00010100;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0001000;
				bits[11] = 0b1110000;
				break;
			case 'z':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00111110;
				bits[5] = 0b00100100;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00010010;
				bits[9] = 0b00111110;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;
			case '{':
				bits[0] = 0b00000110;
				bits[1] = 0b00000100;
				bits[2] = 0b00000100;
				bits[3] = 0b00000100;
				bits[4] = 0b00000100;
				bits[5] = 0b00001000;
				bits[6] = 0b00000100;
				bits[7] = 0b00000100;
				bits[8] = 0b00000100;
				bits[9] = 0b00000100;
				bits[10] = 0b0000110;
				bits[11] = 0b0000000;
				break;
			case '|':
				bits[0] = 0b00001000;
				bits[1] = 0b00001000;
				bits[2] = 0b00001000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0001000;
				bits[11] = 0b0001000;
				break;
			case '}':
				bits[0] = 0b00110000;
				bits[1] = 0b00010000;
				bits[2] = 0b00010000;
				bits[3] = 0b00010000;
				bits[4] = 0b00010000;
				bits[5] = 0b00001000;
				bits[6] = 0b00010000;
				bits[7] = 0b00010000;
				bits[8] = 0b00010000;
				bits[9] = 0b00010000;
				bits[10] = 0b0110000;
				bits[11] = 0b0000000;
				break;
			case '~':
				bits[0] = 0b00100111;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				break;

			default:
				break;
			}
		}
		int make(char c)
		{
			if (c < 32 || c > 126)
				return -1;
			// clear pic
			memset(pic.data, 0, sizeof(pic.data));
			uint8_t bits[12] = {0}; // 12 lines, 6 bits each line
			font1(c, bits);
			// copy bits to pic.data
			for (int i = 0; i < 12; i++)
			{
				pic.data[i] = bits[i];
			}
			return 0;
		}
	};

	class text2pic_7x13 : public text2pic
	{
		/*bmp pic("C:\\Users\\bionukg\\source\\header\\font.bmp");
		for (char c = ' '; c <= '~'; c++)
		{
			printf("case '%c':\n", c);
			for (size_t i = 0; i < 13; i++)
			{
				printf("bits[%d] = 0b", i);
				if (i < 10)
					printf("0");
				for (size_t j = 0; j < 7; j++)
				{

					pic.pointread_r(j + (c - ' ') * 7, i);
					if (pic.rgb[0] < 40)
						printf("0");
					else
						printf("1");
				}
				printf(";\n");
			}
			printf("break;\n");
		}*/

		void font1(char c, uint8_t bits[13])
		{
			switch (c)
			{
			case ' ':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '!':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00010000;
				bits[3] = 0b00010000;
				bits[4] = 0b00010000;
				bits[5] = 0b00010000;
				bits[6] = 0b00010000;
				bits[7] = 0b00010000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0010000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '"':
				bits[0] = 0b00000000;
				bits[1] = 0b00010100;
				bits[2] = 0b00010100;
				bits[3] = 0b00101000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '#':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00010010;
				bits[3] = 0b00010010;
				bits[4] = 0b01111110;
				bits[5] = 0b00010010;
				bits[6] = 0b00100100;
				bits[7] = 0b00100100;
				bits[8] = 0b01111110;
				bits[9] = 0b00100100;
				bits[10] = 0b0100100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '$':
				bits[0] = 0b00000000;
				bits[1] = 0b00001000;
				bits[2] = 0b00011100;
				bits[3] = 0b00101010;
				bits[4] = 0b00101000;
				bits[5] = 0b00011000;
				bits[6] = 0b00001100;
				bits[7] = 0b00001010;
				bits[8] = 0b00101010;
				bits[9] = 0b00101010;
				bits[10] = 0b0011100;
				bits[11] = 0b0001000;
				bits[12] = 0b0000000;
				break;
			case '%':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00100100;
				bits[3] = 0b01010100;
				bits[4] = 0b01011000;
				bits[5] = 0b01011000;
				bits[6] = 0b00110100;
				bits[7] = 0b00011010;
				bits[8] = 0b00011010;
				bits[9] = 0b00101010;
				bits[10] = 0b0100100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '&':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00010000;
				bits[3] = 0b00101000;
				bits[4] = 0b00101000;
				bits[5] = 0b00110000;
				bits[6] = 0b00100110;
				bits[7] = 0b01010100;
				bits[8] = 0b01010100;
				bits[9] = 0b01001001;
				bits[10] = 0b0110110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '\'':
				bits[0] = 0b00000000;
				bits[1] = 0b00110000;
				bits[2] = 0b00010000;
				bits[3] = 0b00100000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '(':
				bits[0] = 0b00000000;
				bits[1] = 0b00000010;
				bits[2] = 0b00000100;
				bits[3] = 0b00000100;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00000100;
				bits[10] = 0b0000100;
				bits[11] = 0b0000010;
				bits[12] = 0b0000000;
				break;
			case ')':
				bits[0] = 0b00000000;
				bits[1] = 0b00100000;
				bits[2] = 0b00010000;
				bits[3] = 0b00010000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00010000;
				bits[10] = 0b0010000;
				bits[11] = 0b0100000;
				bits[12] = 0b0000000;
				break;
			case '*':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00001000;
				bits[4] = 0b00101010;
				bits[5] = 0b00011100;
				bits[6] = 0b00011100;
				bits[7] = 0b00101010;
				bits[8] = 0b00001000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '+':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b01111111;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case ',':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00110000;
				bits[10] = 0b0010000;
				bits[11] = 0b0100000;
				bits[12] = 0b0000000;
				break;
			case '-':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b01111110;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '.':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00110000;
				bits[10] = 0b0110000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '/':
				bits[0] = 0b00000000;
				bits[1] = 0b00000010;
				bits[2] = 0b00000100;
				bits[3] = 0b00000100;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00010000;
				bits[7] = 0b00010000;
				bits[8] = 0b00010000;
				bits[9] = 0b00100000;
				bits[10] = 0b0100000;
				bits[11] = 0b1000000;
				bits[12] = 0b0000000;
				break;
			case '0':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00011100;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00100010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b0011100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '1':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00001000;
				bits[3] = 0b00011000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0011100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '2':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00011100;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00000010;
				bits[6] = 0b00000100;
				bits[7] = 0b00001000;
				bits[8] = 0b00010000;
				bits[9] = 0b00100010;
				bits[10] = 0b0111110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '3':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00011100;
				bits[3] = 0b00100010;
				bits[4] = 0b00000010;
				bits[5] = 0b00000010;
				bits[6] = 0b00001100;
				bits[7] = 0b00000010;
				bits[8] = 0b00000010;
				bits[9] = 0b00100010;
				bits[10] = 0b0011100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '4':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000100;
				bits[3] = 0b00000100;
				bits[4] = 0b00001100;
				bits[5] = 0b00010100;
				bits[6] = 0b00010100;
				bits[7] = 0b00100100;
				bits[8] = 0b00111110;
				bits[9] = 0b00000100;
				bits[10] = 0b0001110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '5':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00111110;
				bits[3] = 0b00100000;
				bits[4] = 0b00100000;
				bits[5] = 0b00111100;
				bits[6] = 0b00100010;
				bits[7] = 0b00000010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b0011100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '6':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00001100;
				bits[3] = 0b00010010;
				bits[4] = 0b00100000;
				bits[5] = 0b00101100;
				bits[6] = 0b00110010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b0011100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '7':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00111110;
				bits[3] = 0b00100010;
				bits[4] = 0b00000100;
				bits[5] = 0b00000100;
				bits[6] = 0b00000100;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0001000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '8':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00011100;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00100010;
				bits[6] = 0b00011100;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b0011100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '9':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00011100;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00100010;
				bits[6] = 0b00100110;
				bits[7] = 0b00011010;
				bits[8] = 0b00000010;
				bits[9] = 0b00100100;
				bits[10] = 0b0011000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case ':':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00010000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0010000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case ';':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00001000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0001000;
				bits[11] = 0b0001000;
				bits[12] = 0b0000000;
				break;
			case '<':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000010;
				bits[3] = 0b00000100;
				bits[4] = 0b00001000;
				bits[5] = 0b00010000;
				bits[6] = 0b00100000;
				bits[7] = 0b00010000;
				bits[8] = 0b00001000;
				bits[9] = 0b00000100;
				bits[10] = 0b0000010;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '=':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b01111110;
				bits[6] = 0b00000000;
				bits[7] = 0b01111110;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '>':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00100000;
				bits[3] = 0b00010000;
				bits[4] = 0b00001000;
				bits[5] = 0b00000100;
				bits[6] = 0b00000010;
				bits[7] = 0b00000100;
				bits[8] = 0b00001000;
				bits[9] = 0b00010000;
				bits[10] = 0b0100000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '?':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00011100;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00000010;
				bits[6] = 0b00000100;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00000000;
				bits[10] = 0b0001000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '@':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00011110;
				bits[3] = 0b00100001;
				bits[4] = 0b01001101;
				bits[5] = 0b01010101;
				bits[6] = 0b01010101;
				bits[7] = 0b01010101;
				bits[8] = 0b01001110;
				bits[9] = 0b00100001;
				bits[10] = 0b0011110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'A':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00001000;
				bits[3] = 0b00001000;
				bits[4] = 0b00010100;
				bits[5] = 0b00010100;
				bits[6] = 0b00010100;
				bits[7] = 0b00011100;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b1110111;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'B':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01111100;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00100010;
				bits[6] = 0b00111100;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b1111100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'C':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00011110;
				bits[3] = 0b00100010;
				bits[4] = 0b01000000;
				bits[5] = 0b01000000;
				bits[6] = 0b01000000;
				bits[7] = 0b01000000;
				bits[8] = 0b01000000;
				bits[9] = 0b00100010;
				bits[10] = 0b0011100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'D':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01111000;
				bits[3] = 0b00100100;
				bits[4] = 0b00100010;
				bits[5] = 0b00100010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100100;
				bits[10] = 0b1111000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'E':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01111110;
				bits[3] = 0b00100010;
				bits[4] = 0b00100000;
				bits[5] = 0b00100100;
				bits[6] = 0b00111100;
				bits[7] = 0b00100100;
				bits[8] = 0b00100000;
				bits[9] = 0b00100010;
				bits[10] = 0b1111110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'F':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01111110;
				bits[3] = 0b00100010;
				bits[4] = 0b00100000;
				bits[5] = 0b00100100;
				bits[6] = 0b00111100;
				bits[7] = 0b00100100;
				bits[8] = 0b00100000;
				bits[9] = 0b00100000;
				bits[10] = 0b1110000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'G':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00011100;
				bits[3] = 0b00100100;
				bits[4] = 0b01000000;
				bits[5] = 0b01000000;
				bits[6] = 0b01000000;
				bits[7] = 0b01001110;
				bits[8] = 0b01000100;
				bits[9] = 0b00100100;
				bits[10] = 0b0011000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'H':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01110111;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00100010;
				bits[6] = 0b00111110;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b1110111;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'I':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00111110;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0111110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'J':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00111110;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0001000;
				bits[11] = 0b1001000;
				bits[12] = 0b1110000;
				break;
			case 'K':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01110110;
				bits[3] = 0b00100100;
				bits[4] = 0b00101000;
				bits[5] = 0b00110000;
				bits[6] = 0b00111000;
				bits[7] = 0b00101000;
				bits[8] = 0b00100100;
				bits[9] = 0b00100100;
				bits[10] = 0b1110110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'L':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01110000;
				bits[3] = 0b00100000;
				bits[4] = 0b00100000;
				bits[5] = 0b00100000;
				bits[6] = 0b00100000;
				bits[7] = 0b00100000;
				bits[8] = 0b00100000;
				bits[9] = 0b00100010;
				bits[10] = 0b1111110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'M':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01110111;
				bits[3] = 0b00110110;
				bits[4] = 0b00110110;
				bits[5] = 0b00110110;
				bits[6] = 0b00101010;
				bits[7] = 0b00101010;
				bits[8] = 0b00101010;
				bits[9] = 0b00101010;
				bits[10] = 0b1101011;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'N':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01110111;
				bits[3] = 0b00110010;
				bits[4] = 0b00110010;
				bits[5] = 0b00101010;
				bits[6] = 0b00101010;
				bits[7] = 0b00101010;
				bits[8] = 0b00100110;
				bits[9] = 0b00100110;
				bits[10] = 0b1110010;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'O':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00011000;
				bits[3] = 0b00100100;
				bits[4] = 0b01000010;
				bits[5] = 0b01000010;
				bits[6] = 0b01000010;
				bits[7] = 0b01000010;
				bits[8] = 0b01000010;
				bits[9] = 0b00100100;
				bits[10] = 0b0011000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'P':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01111100;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00100010;
				bits[6] = 0b00111100;
				bits[7] = 0b00100000;
				bits[8] = 0b00100000;
				bits[9] = 0b00100000;
				bits[10] = 0b1110000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'Q':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00011000;
				bits[3] = 0b00100100;
				bits[4] = 0b01000010;
				bits[5] = 0b01000010;
				bits[6] = 0b01000010;
				bits[7] = 0b01000010;
				bits[8] = 0b01011010;
				bits[9] = 0b00100100;
				bits[10] = 0b0011100;
				bits[11] = 0b0000110;
				bits[12] = 0b0000000;
				break;
			case 'R':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01111100;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00111100;
				bits[6] = 0b00101000;
				bits[7] = 0b00100100;
				bits[8] = 0b00100100;
				bits[9] = 0b00100010;
				bits[10] = 0b1110011;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'S':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00011110;
				bits[3] = 0b00100010;
				bits[4] = 0b00100000;
				bits[5] = 0b00010000;
				bits[6] = 0b00001100;
				bits[7] = 0b00000010;
				bits[8] = 0b00000010;
				bits[9] = 0b00100010;
				bits[10] = 0b0111100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'T':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00111110;
				bits[3] = 0b00101010;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0011100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'U':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01110111;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00100010;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b0011100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'V':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01110111;
				bits[3] = 0b00100010;
				bits[4] = 0b00100010;
				bits[5] = 0b00010100;
				bits[6] = 0b00010100;
				bits[7] = 0b00010100;
				bits[8] = 0b00010100;
				bits[9] = 0b00001000;
				bits[10] = 0b0001000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'W':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01101011;
				bits[3] = 0b00101010;
				bits[4] = 0b00101010;
				bits[5] = 0b00101010;
				bits[6] = 0b00101010;
				bits[7] = 0b00110110;
				bits[8] = 0b00010100;
				bits[9] = 0b00010100;
				bits[10] = 0b0010100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'X':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01100110;
				bits[3] = 0b00100100;
				bits[4] = 0b00100100;
				bits[5] = 0b00011000;
				bits[6] = 0b00011000;
				bits[7] = 0b00011000;
				bits[8] = 0b00100100;
				bits[9] = 0b00100100;
				bits[10] = 0b1100110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'Y':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01110111;
				bits[3] = 0b00100010;
				bits[4] = 0b00010100;
				bits[5] = 0b00010100;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0011100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'Z':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00111110;
				bits[3] = 0b00100100;
				bits[4] = 0b00000100;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00010000;
				bits[9] = 0b00010010;
				bits[10] = 0b0111110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '[':
				bits[0] = 0b00000000;
				bits[1] = 0b00001110;
				bits[2] = 0b00001000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0001000;
				bits[11] = 0b0001110;
				bits[12] = 0b0000000;
				break;
			case '\\':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00100000;
				bits[3] = 0b00100000;
				bits[4] = 0b00010000;
				bits[5] = 0b00010000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00000100;
				bits[9] = 0b00000100;
				bits[10] = 0b0000010;
				bits[11] = 0b0000010;
				bits[12] = 0b0000000;
				break;
			case ']':
				bits[0] = 0b00000000;
				bits[1] = 0b00111000;
				bits[2] = 0b00001000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0001000;
				bits[11] = 0b0111000;
				bits[12] = 0b0000000;
				break;
			case '^':
				bits[0] = 0b00001000;
				bits[1] = 0b00010100;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '_':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				bits[12] = 0b1111110;
				break;
			case '`':
				bits[0] = 0b00110000;
				bits[1] = 0b00001000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'a':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00011100;
				bits[6] = 0b00100010;
				bits[7] = 0b00001110;
				bits[8] = 0b00010010;
				bits[9] = 0b00100010;
				bits[10] = 0b0011111;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'b':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01100000;
				bits[3] = 0b00100000;
				bits[4] = 0b00100000;
				bits[5] = 0b00111100;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b0111100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'c':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00011110;
				bits[6] = 0b00100010;
				bits[7] = 0b00100000;
				bits[8] = 0b00100000;
				bits[9] = 0b00100010;
				bits[10] = 0b0011100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'd':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000110;
				bits[3] = 0b00000010;
				bits[4] = 0b00000010;
				bits[5] = 0b00011110;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b0011111;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'e':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00011100;
				bits[6] = 0b00100010;
				bits[7] = 0b00111110;
				bits[8] = 0b00100000;
				bits[9] = 0b00100010;
				bits[10] = 0b0011100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'f':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00001100;
				bits[3] = 0b00010010;
				bits[4] = 0b00010000;
				bits[5] = 0b01111100;
				bits[6] = 0b00010000;
				bits[7] = 0b00010000;
				bits[8] = 0b00010000;
				bits[9] = 0b00010000;
				bits[10] = 0b0111100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'g':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00011110;
				bits[6] = 0b00100100;
				bits[7] = 0b00100100;
				bits[8] = 0b00011000;
				bits[9] = 0b00100000;
				bits[10] = 0b0011100;
				bits[11] = 0b0100010;
				bits[12] = 0b0011100;
				break;
			case 'h':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01100000;
				bits[3] = 0b00100000;
				bits[4] = 0b00100000;
				bits[5] = 0b00101100;
				bits[6] = 0b00110010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b1110111;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'i':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00001000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00111000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0111110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'j':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000100;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00011100;
				bits[6] = 0b00000100;
				bits[7] = 0b00000100;
				bits[8] = 0b00000100;
				bits[9] = 0b00000100;
				bits[10] = 0b0000100;
				bits[11] = 0b0000100;
				bits[12] = 0b0111000;
				break;
			case 'k':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b01100000;
				bits[3] = 0b00100000;
				bits[4] = 0b00100000;
				bits[5] = 0b00101110;
				bits[6] = 0b00100100;
				bits[7] = 0b00101000;
				bits[8] = 0b00111000;
				bits[9] = 0b00100100;
				bits[10] = 0b1110110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'l':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00111000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0111110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'm':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b01111100;
				bits[6] = 0b00101010;
				bits[7] = 0b00101010;
				bits[8] = 0b00101010;
				bits[9] = 0b00101010;
				bits[10] = 0b1101011;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'n':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b01101100;
				bits[6] = 0b00110010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b1110111;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'o':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00011100;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b0011100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'p':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b01111100;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b0111100;
				bits[11] = 0b0100000;
				bits[12] = 0b1110000;
				break;
			case 'q':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00011110;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100010;
				bits[10] = 0b0011110;
				bits[11] = 0b0000010;
				bits[12] = 0b0000111;
				break;
			case 'r':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00110110;
				bits[6] = 0b00011000;
				bits[7] = 0b00010000;
				bits[8] = 0b00010000;
				bits[9] = 0b00010000;
				bits[10] = 0b0111100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 's':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00011110;
				bits[6] = 0b00100010;
				bits[7] = 0b00011000;
				bits[8] = 0b00000100;
				bits[9] = 0b00100010;
				bits[10] = 0b0111100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 't':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00010000;
				bits[4] = 0b00010000;
				bits[5] = 0b01111100;
				bits[6] = 0b00010000;
				bits[7] = 0b00010000;
				bits[8] = 0b00010000;
				bits[9] = 0b00010010;
				bits[10] = 0b0001100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'u':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b01100110;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00100010;
				bits[9] = 0b00100110;
				bits[10] = 0b0011011;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'v':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b01100011;
				bits[6] = 0b00100010;
				bits[7] = 0b00100010;
				bits[8] = 0b00010100;
				bits[9] = 0b00010100;
				bits[10] = 0b0001000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'w':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b01101011;
				bits[6] = 0b00101010;
				bits[7] = 0b00101010;
				bits[8] = 0b00101010;
				bits[9] = 0b00010100;
				bits[10] = 0b0010100;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'x':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b01110110;
				bits[6] = 0b00100100;
				bits[7] = 0b00011000;
				bits[8] = 0b00011000;
				bits[9] = 0b00100100;
				bits[10] = 0b1101110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case 'y':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b01110111;
				bits[6] = 0b00100010;
				bits[7] = 0b00010100;
				bits[8] = 0b00010100;
				bits[9] = 0b00001000;
				bits[10] = 0b0001000;
				bits[11] = 0b0001000;
				bits[12] = 0b1110000;
				break;
			case 'z':
				bits[0] = 0b00000000;
				bits[1] = 0b00000000;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00111110;
				bits[6] = 0b00100100;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00010010;
				bits[10] = 0b0111110;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			case '{':
				bits[0] = 0b00000000;
				bits[1] = 0b00000110;
				bits[2] = 0b00000100;
				bits[3] = 0b00000100;
				bits[4] = 0b00000100;
				bits[5] = 0b00000100;
				bits[6] = 0b00001000;
				bits[7] = 0b00000100;
				bits[8] = 0b00000100;
				bits[9] = 0b00000100;
				bits[10] = 0b0000100;
				bits[11] = 0b0000110;
				bits[12] = 0b0000000;
				break;
			case '|':
				bits[0] = 0b00001000;
				bits[1] = 0b00001000;
				bits[2] = 0b00001000;
				bits[3] = 0b00001000;
				bits[4] = 0b00001000;
				bits[5] = 0b00001000;
				bits[6] = 0b00001000;
				bits[7] = 0b00001000;
				bits[8] = 0b00001000;
				bits[9] = 0b00001000;
				bits[10] = 0b0001000;
				bits[11] = 0b0001000;
				bits[12] = 0b0001000;
				break;
			case '}':
				bits[0] = 0b00000000;
				bits[1] = 0b00110000;
				bits[2] = 0b00010000;
				bits[3] = 0b00010000;
				bits[4] = 0b00010000;
				bits[5] = 0b00010000;
				bits[6] = 0b00001000;
				bits[7] = 0b00010000;
				bits[8] = 0b00010000;
				bits[9] = 0b00010000;
				bits[10] = 0b0010000;
				bits[11] = 0b0110000;
				bits[12] = 0b0000000;
				break;
			case '~':
				bits[0] = 0b00011010;
				bits[1] = 0b00100100;
				bits[2] = 0b00000000;
				bits[3] = 0b00000000;
				bits[4] = 0b00000000;
				bits[5] = 0b00000000;
				bits[6] = 0b00000000;
				bits[7] = 0b00000000;
				bits[8] = 0b00000000;
				bits[9] = 0b00000000;
				bits[10] = 0b0000000;
				bits[11] = 0b0000000;
				bits[12] = 0b0000000;
				break;
			default:
				break;
			}
		}

	public:
		text2pic_7x13() : text2pic(7, 13) {}
		bitPic<7, 13> pic;
		int make(char c)
		{
			if (c < 32 || c > 126)
				return -1;
			// clear pic
			memset(pic.data, 0, sizeof(pic.data));
			uint8_t bits[13] = {0}; // 13 lines, 7 bits each line
			font1(c, bits);
			// copy bits to pic.data
			for (int i = 0; i < 13; i++)
			{
				pic.data[i] = bits[i];
			}
			return 0;
		}
	};
	
	typedef class bmp_header
	{
	public:
		uint32_t width = 0;	   // 像素宽度
		uint32_t memwidth = 0; // 内存中一行的字节数
		uint32_t height = 0;   // 像素高度
		uint16_t bitdepth = 0; // 位深度(24或32)
		uint32_t size = 0;	   // 总大小=数据大小+bmp_headerlength
		uint32_t datasize = 0; // 数据块大小

		bmp_header(uint32_t wid, uint32_t hei, uint32_t bitdep)
		{
			init(wid, hei, bitdep);
		}
		bmp_header()
		{
		}
		void init(uint32_t wid, uint32_t hei, uint32_t bitdep)
		{
			width = wid;
			height = hei;
			bitdepth = bitdep;
			while (wid % 4 != 0)
			{
				wid++;
			}
			memwidth = wid * (bitdepth / 8);
			datasize = memwidth * height;
			size = datasize + 54;
		}
	} bmphd;

	struct point
	{
		uint32_t x;
		uint32_t y;
	};

	/// <summary>
	/// 右下角为原点，坐标系在第二象限，坑的一批
	/// </summary>
	class bmp
	{

#define bmp_headerlength 54

	public:
		uint32_t width;	   // 像素宽度
		uint32_t memwidth; // 内存中一行的字节数
		uint32_t height;   // 像素高度
		uint16_t bitdepth; // 位深度(24或32)
		uint32_t size;	   // 总大小
		uint32_t datasize; // 数据块大小

		uint8_t *file = 0; // 文件数据地址
		uint8_t *data = 0; // 像素数据地址= file + bmp_headerlength;

		char path[MAX_PATH] = {0}; // 文件路径
		// 以上为静态值
		// 以下为动态值
		uint32_t x = 0;			// 操作点横坐标（右下角原点）
		uint32_t y = 0;			// 操作点纵坐标（右下角原点）
		uint32_t memoffset = 0; // 操作点内存坐标
		uint8_t rgb[4] = {0};	// 兼容普通位图的单个像素的颜色RGB值存放

		// 创建一个格式大小一模一样的新的空bmp
		bmp(bmp *formatted)
		{
			this->width = formatted->width;
			this->memwidth = formatted->memwidth;
			this->height = formatted->height;
			this->bitdepth = formatted->bitdepth;
			this->size = formatted->size;
			this->datasize = formatted->datasize;
			this->file = new uint8_t[size];
			this->data = this->file + bmp_headerlength;
		}
		// 复制一个bmp
		bmp(const bmp &copyed)
		{
			this->width = copyed.width;
			this->memwidth = copyed.memwidth;
			this->height = copyed.height;
			this->bitdepth = copyed.bitdepth;
			this->size = copyed.size;
			this->datasize = copyed.datasize;
			this->file = new uint8_t[this->size];
			this->data = this->file + bmp_headerlength;
			memcpy(this->file, copyed.file, this->size);
		}
		// 利用bmp文件头结构创建空bmp
		bmp(bmphd hd)
		{
			width = hd.width;		//
			memwidth = hd.memwidth; // 内存中一行的字节数
			height = hd.height;		// 像素高度
			bitdepth = hd.bitdepth; // 位深度(24或32)
			size = hd.size;			// 总大小
			datasize = hd.datasize; // 数据块大小
			this->file = (uint8_t *)malloc(sizeof(uint8_t) * size);
			this->data = this->file + bmp_headerlength;
		}
		// 啥都没有初始化
		bmp(void){};

		// 打开路径下的.bmp文件
		bmp(const char *newpath)
		{
			if (0 != this->open(newpath))
			{
				printf_s("\nbmp open exception\n");
				system("pause");
			}
		}
		~bmp()
		{
			close();
		}

		void close()
		{
			if (file != NULL)
			{
				free(file);
				file = NULL;
				data = NULL;
			}
		}

		// 把文件newpath写入内存file(原内存未删除)
		int32_t // -1:路径不存在 -3:内存不足
		open(const char *newpath)
		{
			if (newpath == 0)
				return -1;

			FILE *fp;
			strcpy_s(path, newpath);
			fopen_s(&fp, path, "rb");
			if (fp == 0)
			{
				return -2;
			}
			uint8_t headerdata[bmp_headerlength] = {0};
			fread(headerdata, 1, bmp_headerlength, fp);

			size = *(uint32_t *)&headerdata[2];
			width = *(uint32_t *)&headerdata[18];
			height = *(uint32_t *)&headerdata[22];
			bitdepth = *(uint16_t *)&headerdata[28];
			datasize = *(uint32_t *)&headerdata[34];
			file = new uint8_t[size];
			if (file == 0)
			{
				fclose(fp);
				return -3;
			}
			data = file + bmp_headerlength;
			fseek(fp, 0, SEEK_SET);
			fread(file, 1, size, fp);

			fclose(fp);
			memwidth = width * bitdepth / 8;
			while (memwidth % 4 != 0)
			{
				memwidth++;
			}
			return 0;
		}

		void presave()
		{
			uint8_t def[bmp_headerlength] = {
				66, 77,		 // 固定BM 0~1
				0, 0, 0, 0,	 // 文件大小size=datasize+bmp_headerlength 2~5
				0, 0, 0, 0,	 // 固定 6~9
				54, 0, 0, 0, // 固定文件开头长度 10~13
				40, 0, 0, 0, // 固定 14~17
				0, 0, 0, 0,	 // 宽width 18~21
				0, 0, 0, 0,	 // 高height 22~25
				1, 0,		 // 固定 26~27
				24, 0,		 // 24位图bitdepth24，固定 28~29
				0, 0, 0, 0,	 // 固定 30~33
				0, 0, 0, 0,	 // 像素数据大小datasize 34~37
				0, 0, 0, 0,	 // 固定 38~41
				0, 0, 0, 0,	 // 固定 42~45
				0, 0, 0, 0,	 // 固定 46~49
				0, 0, 0, 0,	 // 固定 50~53
							 // 默认文件开头
			};
			*(uint32_t *)&def[2] = size;
			*(uint32_t *)&def[18] = width;
			*(uint32_t *)&def[22] = height;
			*(uint16_t *)&def[28] = bitdepth;
			*(uint32_t *)&def[34] = datasize;
			memcpy(file, def, bmp_headerlength);
		}
		// 把文件写入硬盘
		uint32_t //-1:路径不存在；-2:
		save_as(
			const char *newpath // 输入0时存入原路径，否则修改路径为输入路径
		)
		{
			if (newpath == 0)
				return -1;
			strcpy_s(path, newpath);

			FILE *fp;
			fopen_s(&fp, path, "wb+");
			if (fp == 0)
			{
				return -2;
			}

			//a~z=97~122
			//A~Z=65~90
			//0~9=48~57

			uint8_t def[bmp_headerlength] = {
				66, 77,		 // 固定BM 0~1
				0, 0, 0, 0,	 // 文件大小size=datasize+bmp_headerlength 2~5
				0, 0, 0, 0,	 // 固定 6~9
				54, 0, 0, 0, // 固定文件开头长度 10~13
				40, 0, 0, 0, // 固定 14~17
				0, 0, 0, 0,	 // 宽width 18~21
				0, 0, 0, 0,	 // 高height 22~25
				1, 0,		 // 固定 26~27
				24, 0,		 // 24位图bitdepth24，固定 28~29
				0, 0, 0, 0,	 // 固定 30~33
				0, 0, 0, 0,	 // 像素数据大小datasize 34~37
				0, 0, 0, 0,	 // 固定 38~41
				0, 0, 0, 0,	 // 固定 42~45
				0, 0, 0, 0,	 // 固定 46~49
				0, 0, 0, 0,	 // 固定 50~53
							// 默认文件开头
			};
			*(uint32_t *)&def[2] = size;
			*(uint32_t *)&def[18] = width;
			*(uint32_t *)&def[22] = height;
			*(uint16_t *)&def[28] = bitdepth;
			*(uint32_t *)&def[34] = datasize;
			fwrite(def, 1, bmp_headerlength, fp);
			fwrite(data, 1, datasize, fp);
			fclose(fp);
			return 0;

			// fopen_s(&fp, path, "wb+");
			// if (fp == 0)
			//{
			//	return -2;
			// }

			//*(uint32_t*)&def[2] = size;
			//*(uint32_t*)&def[18] = width;
			//*(uint32_t*)&def[22] = height;
			//*(uint16_t*)&def[28] = bitdepth;
			//*(uint32_t*)&def[34] = datasize;
			// fwrite(def, 1, bmp_headerlength, fp);
			// fwrite(data, 1, datasize, fp);
			// fclose(fp);
			// return 0;
		}

		/// 另存为png
		// 参考文献：
		//  [1]PNG文件格式白皮书 W3C https://blog.csdn.net/yongzhewuwei_2008/article/details/1007204 [EB/OL]2022/7/31
		//  [2]svpng miloyip http://github.com/miloyip/svpng [EB/OL]2022/7/31
		int32_t		 //-1传入路径无效,-2文件打开失败，-3文件位深度不为24或32
		save_as_png( // 参考：
			_In_ const char *newpath)
		{
#ifndef SVPNG_INC_
#ifdef bionukg_optimize
#define fwriteFp(ptr, len) fwrite_biopt(ptr, size, fp, buffer, idx)
#else // bionukg_optimize
#define fwriteFp(ptr, len) fwrite(ptr, 1, len, fp)
#endif
#define fwrite32swap(u32p)                                      \
	{                                                           \
		*(uint32_t *)u32p = _byteswap_ulong(*(uint32_t *)u32p); \
		fwriteFp(u32p, 4);                                      \
		*(uint32_t *)u32p = _byteswap_ulong(*(uint32_t *)u32p); \
	}
#define fwriteFpCRC(ptr, len)                      \
	{                                              \
		fwriteFp(ptr, len);                        \
		for (i = 0; i < len; i++)                  \
		{                                          \
			crc ^= ((uint8_t *)ptr)[i];            \
			crc = (crc >> 4) ^ CRCtable[crc & 15]; \
			crc = (crc >> 4) ^ CRCtable[crc & 15]; \
		}                                          \
	}
#define fwrite32CRCswap(u32p)                                   \
	{                                                           \
		*(uint32_t *)u32p = _byteswap_ulong(*(uint32_t *)u32p); \
		fwriteFpCRC(u32p, 4);                                   \
		*(uint32_t *)u32p = _byteswap_ulong(*(uint32_t *)u32p); \
	}
#define fwrite8CRCadler(u8p)                    \
	{                                           \
		fwriteFpCRC(u8p, 1);                    \
		ADLER_a = (ADLER_a + (*u8p)) % 0xfff1;  \
		ADLER_b = (ADLER_b + ADLER_a) % 0xfff1; \
	}
#define fwriteChunkBegin(name32p, leng32p) \
	{                                      \
		fwrite32swap(leng32p);             \
		crc = ~0U;                         \
		fwriteFpCRC(name32p, 4);           \
	}
#define fwriteChunkEnd()   \
	{                      \
		crc = ~crc;        \
		fwrite32swap(&crc) \
	};
			if (newpath == 0)
				return -1;
			bool alpha = false;

			if (this->bitdepth == 24)
				alpha = false;
			else if (this->bitdepth == 32)
				alpha = true;
			else
				return -3;
			FILE *fp;
			fopen_s(&fp, newpath, "wb+");
			if (fp == 0)
				return -2;
			uint8_t pngStart[] =
				{
					0x89, 'P', 'N', 'G',
					'\r', '\n', 0x1A, '\n'};
			static const unsigned CRCtable[] = {// CRC校验表
												0, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
												0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
												0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
												0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c};
			unsigned ADLER_a = 1, ADLER_b = 0, crc;
			uint16_t MemWidthP = this->width * (alpha ? 4 : 3) + 1, x, y, i;
			uint32_t uint32;
			uint16_t uint16, idx = 0;
			uint8_t uint8, buffer[256] = {0};

			fwriteFp(pngStart, 8);

			uint32 = 13;

			fwriteChunkBegin("IHDR", &uint32);
			fwrite32CRCswap(&(this->width));
			fwrite32CRCswap(&(this->height));
			uint8 = 8;
			fwriteFpCRC(&uint8, 1);
			uint8 = (alpha ? 6 : 2);
			fwriteFpCRC(&uint8, 1);
			uint32 = 0;
			fwriteFpCRC(&uint32, 3);
			fwriteChunkEnd();

			uint32 = 2 + this->height * (5 + MemWidthP) + 4;

			fwriteChunkBegin("IDAT", &uint32);
			uint16 = 0x0178;
			fwriteFpCRC(&uint16, 2);
			uint8_t *img = this->data;

			uint32 = ((~MemWidthP) << 16) | MemWidthP;
			MemWidthP -= 1;
			for (y = 0; y < this->height; y++)
			{
				uint8 = (y == height - 1) ? 1 : 0;
				fwriteFpCRC(&uint8, 1);
				fwriteFpCRC(&uint32, 4);
				uint8 = 0;
				fwriteFpCRC(&uint8, 1);
				fwriteFpCRC(img, MemWidthP);

				ADLER_a %= 0xfff1;
				ADLER_b = (ADLER_b + ADLER_a) % 0xfff1;
				for (x = 0; x < MemWidthP; x++, img++)
				{
					ADLER_a = (ADLER_a + (*img)) % 0xfff1;
					ADLER_b = (ADLER_b + ADLER_a) % 0xfff1;
				}
			}
			uint32 = (ADLER_b << 16) | ADLER_a;
			fwrite32CRCswap(&uint32);
			fwriteChunkEnd();

			uint32 = 0;

			fwriteChunkBegin("IEND", &uint32);
			fwriteChunkEnd();

			fclose(fp);
			// printf("2:%lld\n", fw);

			return 0;
			// #ifdef fwriteFp
			// #undef fwriteFp
			// #endif
#ifdef fwrite32swap
#undef fwrite32swap
#endif
#else
		if (newpath == 0)
			return -1;
		bool alpha = false;

		if (this->bitdepth == 24)
			alpha = false;
		else if (this->bitdepth == 32)
			alpha = true;
		else
			return -3;
		FILE *fp = fopen(newpath, "wb+");
		if (fp == 0)
			return -2;
		svpng(fp, this->width, this->height, this->data, alpha);
		fclose(fp);
#endif
		}
		// 从坐标找内存
		// uint(& open)(const char*newpath) = memloadfile;
		// uint(bmp::* save)(const char*) = diskloadfile;

		void pointfind(void)
		{
			if (x >= width)
				x = width - 1;
			if (x < 0)
				x = 0;
			if (y >= height)
				y = height - 1;
			if (y < 0)
				y = 0;

			memoffset = y * memwidth + x * (bitdepth / 8) + bmp_headerlength;
			return;
		}
		uint32_t pointfind(uint32_t x, uint32_t y)
		{
			if (x >= width)
				x = width - 1;
			if (x < 0)
				x = 0;
			if (y >= height)
				y = height - 1;
			if (y < 0)
				y = 0;
			return y * memwidth + x * (bitdepth / 8) + bmp_headerlength;
		}
		void pointfind_r(void)
		{
			if (x >= width)
				x = width - 1;
			if (x < 0)
				x = 0;
			if (y >= height)
				y = height - 1;
			if (y < 0)
				y = 0;

			memoffset = (height - y - 1) * memwidth + x * (bitdepth / 8) + bmp_headerlength;
			return;
		}
		uint32_t pointfind_r(uint32_t x, uint32_t y)
		{
			if (x >= width)
				x = width - 1;
			if (x < 0)
				x = 0;
			if (y >= height)
				y = height - 1;
			if (y < 0)
				y = 0;
			return (height - y - 1) * memwidth + x * (bitdepth / 8) + bmp_headerlength;
		}

		void pointread(uint32_t x, uint32_t y, uint8_t *rgb)
		{
			memcpy(rgb, file + pointfind(x, y), bitdepth / 8);
		}
		void pointread(uint32_t x, uint32_t y)
		{
			memcpy(rgb, file + pointfind(x, y), bitdepth / 8);
		}
		void pointread(void)
		{
			pointfind();
			memcpy(rgb, file + memoffset, bitdepth / 8);
		}
		void pointread_r(uint32_t x, uint32_t y, uint8_t *rgb)
		{
			memcpy(rgb, file + pointfind_r(x, y), bitdepth / 8);
		}
		// 反序读取到rgbs
		void pointread_r(uint32_t x, uint32_t y)
		{
			memcpy(rgb, file + pointfind_r(x, y), bitdepth / 8);
		}
		void pointread_r(void)
		{
			pointfind_r();
			memcpy(rgb, file + memoffset, bitdepth / 8);
		}
		void pointwrite(uint32_t x, uint32_t y, uint8_t *rgb)
		{
			memcpy(file + pointfind(x, y), rgb, bitdepth / 8);
		}
		void pointwrite(uint32_t x, uint32_t y)
		{
			memcpy(file + pointfind(x, y), rgb, bitdepth / 8);
		}
		void pointwrite(void)
		{
			pointfind();
			memcpy(file + memoffset, rgb, bitdepth / 8);
		}
		void pointwrite_r(uint32_t x, uint32_t y, uint8_t *rgb)
		{
			memcpy(file + pointfind_r(x, y), rgb, bitdepth / 8);
		}
		void pointwrite_r(uint32_t x, uint32_t y)
		{
			memcpy(file + pointfind_r(x, y), rgb, bitdepth / 8);
		}
		void pointwrite_r(void)
		{
			pointfind_r();
			memcpy(file + memoffset, rgb, bitdepth / 8);
		}

		// uint pixelfunc(uint32_t x,uint32_t y,void(*fun)(uint*))
		void traversalpoints(void (*func)(bmp *))
		{
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < width; x++)
				{
					pointfind();
					memcpy(rgb, file + memoffset, bitdepth / 8);
					;
					func(this);
					memcpy(file + memoffset, rgb, bitdepth / 8);
				}
			}
		}
		void printtextfile(void)
		{
			// ix = 0;
			for (uint32_t i = 0; i < bmp_headerlength; i++)
			{

				printf("%3d,", file[i]);
				i++;
				printf("%3d,", file[i]);
				i++;
				printf("%3d\t", file[i]);
				// ix++;
				/*if (ix == 4)
				{
					ix = 0;
					printf("\n");
				}*/
			}
			// ix = 0;
			printf("\n");
			for (uint32_t i = bmp_headerlength; i < size; i++)
			{

				printf("%3d,", file[i]);
				i++;
				printf("%3d,", file[i]);
				i++;
				printf("%3d\t", file[i]);
				/*if (ix == 4)
				{
					ix = 0;
					printf("\n");
				}*/
			}
		}
		void clear(uint32_t color = 0)
		{
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < width; x++)
				{
					memcpy(file + y * memwidth + x * (bitdepth / 8) + bmp_headerlength, &color, bitdepth / 8);
				}
			}
		}
		/*void blockread(uint8_t*** block, uint wid, uint hei)
		{
			uint32_t x0 = x;
			uint32_t y0 = y;
			for (uint i = 0; i < hei; i++)
			{
				x++;
				for (uint j = 0; j < wid; j++)
				{
					y++;
					pointread();
					memcpy(block[i][j], rgb, bitdepth / 8);
				}
			}
			x = x0;
			y = y0;
			return;
		}*/

		void draw_line(point p1, point p2)
		{
			int64_t dx = p1.x, dy = p1.y; // delta
			dx -= p2.x, dy -= p2.y;
			int64_t udx = abs(dx), udy = abs(dy);
			int64_t px = 0, py = 0; // pen
			int64_t
				hori = 0, // 水平
				vert = 0, // 竖直
				diag = 0; // 斜的

			for (; px < udx && py < udy;)
			{
				pointwrite(p1.x + dx > 0 ? px : -px, p1.y + dy > 0 ? py : -py);

				hori += udy;
				vert -= udx;
				diag = vert + udy;

				if (abs(diag) <= abs(vert) && abs(diag) <= abs(hori)) // diag最小
				{
					hori = diag;
					vert = diag;
					dx++, dy++;
				}
				else if (abs(vert) <= abs(hori)) // hori最小
				{
					vert = hori;
					dx++;
				}
				else // vert最小
				{
					hori = vert;
					dy++;
				}
			}
		}
		void draw_line_testpic(point p1, point p2)
		{
			double dx = p1.x, dy = p1.y; // delta
			dx -= p2.x, dy -= p2.y;
			double dl = sqrt(dx * dx + dy * dy);
			dx /= dl, dy /= dl;
			uint8_t rgbx[4] = {0, 0, 0, 0};
			double vx = 0, vy = 0, vt = 0;
			for (int x = 0; x < this->width; x++)
			{
				vx = (double)p1.x - x;
				for (int y = 0; y < this->height; y++)
				{
					vy = (double)p1.y - y;
					vt = vy * dx + vx * dy;
					vt = abs(vt);
					const double w = 1.5;
					if (vt > w)
					{
						vt = 255;
					}
					else
					{
						vt *= 256 / w;
					}
					rgbx[0] = vt;
					rgbx[1] = rgbx[0];
					rgbx[2] = rgbx[0];
					rgbx[3] = rgbx[0];
					pointwrite(x, y, rgbx);
				}
			}
		}
		void draw_line_segment(double p1x, double p1y, double p2x, double p2y, double l_width = 1.5)
		{
			double dx = p1x, dy = p1y; // delta
			dx -= p2x, dy -= p2y;
			double dl = sqrt(dx * dx + dy * dy);
			dx /= dl, dy /= dl;
			uint8_t rgbx[4] = {0, 0, 0, 0};
			double vx = 0, vy = 0, vt = 0;
			double xmind = 0, xmaxd = this->width,
				   ymind = 0, ymaxd = this->height;
			if (p1x < p2x)
			{
				xmind = xmind > p1x - l_width ? xmind : p1x - l_width;
				xmaxd = xmaxd < p2x + l_width ? xmaxd : p2x + l_width;
			}
			else
			{
				xmind = xmind > p2x - l_width ? xmind : p2x - l_width;
				xmaxd = xmaxd < p1x + l_width ? xmaxd : p1x + l_width;
			}

			if (p1y < p2y)
			{
				ymind = ymind > p1y - l_width ? ymind : p1y - l_width;
				ymaxd = ymaxd < p2y + l_width ? ymaxd : p2y + l_width;
			}
			else
			{
				ymind = ymind > p2y - l_width ? ymind : p2y - l_width;
				ymaxd = ymaxd < p1y + l_width ? ymaxd : p1y + l_width;
			}
			xmind = xmind < 0 ? 0 : xmind > this->width ? this->width
														: xmind;
			xmaxd = xmaxd < 0 ? 0 : xmaxd > this->width ? this->width
														: xmaxd;
			ymind = ymind < 0 ? 0 : ymind > this->height ? this->height
														 : ymind;
			ymaxd = ymaxd < 0 ? 0 : ymaxd > this->height ? this->height
														 : ymaxd;

			uint32_t xmin = xmind, xmax = xmaxd, ymin = ymind, ymax = ymaxd;
			for (uint32_t y = ymin; y < ymax; y++)
			{
				vy = (double)p1y - y;
				for (uint32_t x = xmin; x < xmax; x++)
				{
					vx = (double)p1x - x;

					vt = vy * dx - vx * dy;
					vt = abs(vt);
					if (vt > l_width)
					{
						continue;
					}
					else
					{
						vt *= 256 / l_width;
					}
					rgbx[0] = vt;
					rgbx[1] = rgbx[0];
					rgbx[2] = rgbx[0];
					rgbx[3] = rgbx[0];
					pointwrite(x, y, rgbx);
				}
			}
		}
		void draw_line_segment_uncovered(double p1x, double p1y, double p2x, double p2y, double l_width = 1.5)
		{
			double dx = p1x, dy = p1y; // delta
			dx -= p2x, dy -= p2y;
			double dx2 = dx * dx, dy2 = dy * dy;
			double dl = sqrt(dx2 + dy2);
			dx /= dl, dy /= dl;
			uint8_t rgbx[4] = {0, 0, 0, 0};
			double vx = 0, vy = 0, vt = 0;
			double xmind = 0, xmaxd = this->width,
				   ymind = 0, ymaxd = this->height;
			if (p1x < p2x)
			{
				xmind = xmind > p1x - l_width ? xmind : p1x - l_width;
				xmaxd = xmaxd < p2x + l_width ? xmaxd : p2x + l_width;
			}
			else
			{
				xmind = xmind > p2x - l_width ? xmind : p2x - l_width;
				xmaxd = xmaxd < p1x + l_width ? xmaxd : p1x + l_width;
			}

			if (p1y < p2y)
			{
				ymind = ymind > p1y - l_width ? ymind : p1y - l_width;
				ymaxd = ymaxd < p2y + l_width ? ymaxd : p2y + l_width;
			}
			else
			{
				ymind = ymind > p2y - l_width ? ymind : p2y - l_width;
				ymaxd = ymaxd < p1y + l_width ? ymaxd : p1y + l_width;
			}
			xmind = xmind < 0 ? 0 : xmind > this->width ? this->width
														: xmind;
			xmaxd = xmaxd < 0 ? 0 : xmaxd > this->width ? this->width
														: xmaxd;
			ymind = ymind < 0 ? 0 : ymind > this->height ? this->height
														 : ymind;
			ymaxd = ymaxd < 0 ? 0 : ymaxd > this->height ? this->height
														 : ymaxd;

			uint32_t xmin = xmind, xmax = xmaxd, ymin = ymind, ymax = ymaxd;
			for (uint32_t y = ymin; y < ymax; y++)
			{
				vy = (double)p1y - y;
				for (uint32_t x = xmin; x < xmax; x++)
				{
					vx = (double)p1x - x;

					vt = vy * dx - vx * dy;
					vt = abs(vt);
					if (vt > l_width)
					{
						continue;
					}

					vt /= l_width;
					pointread(x, y, rgbx);

					rgbx[0] *= vt;
					rgbx[1] = rgbx[0];
					rgbx[2] = rgbx[0];
					rgbx[3] = rgbx[0];
					pointwrite(x, y, rgbx);
				}
			};
		}
		void draw_line_segment_uncovered_o1(double p1x, double p1y, double p2x, double p2y, double l_width = 1.5, uint8_t grayscale = 0)
		{

			double dx = p1x, dy = p1y; // delta
			dx -= p2x, dy -= p2y;
			double dx2 = dx * dx, dy2 = dy * dy;
			double dl = sqrt(dx2 + dy2);
			dx /= dl, dy /= dl;
			uint8_t rgbx[4] = {0, 0, 0, 0};
			double vx = 0, vy = 0, vt = 0;
			double xmind = 0, xmaxd = this->width - 1,
				   ymind = 0, ymaxd = this->height - 1;
			if (p1x < p2x)
			{
				xmind = xmind > p1x - l_width ? xmind : p1x - l_width;
				xmaxd = xmaxd < p2x + l_width ? xmaxd : p2x + l_width;
			}
			else
			{
				xmind = xmind > p2x - l_width ? xmind : p2x - l_width;
				xmaxd = xmaxd < p1x + l_width ? xmaxd : p1x + l_width;
			}

			if (p1y < p2y)
			{
				ymind = ymind > p1y - l_width ? ymind : p1y - l_width;
				ymaxd = ymaxd < p2y + l_width ? ymaxd : p2y + l_width;
			}
			else
			{
				ymind = ymind > p2y - l_width ? ymind : p2y - l_width;
				ymaxd = ymaxd < p1y + l_width ? ymaxd : p1y + l_width;
			}
			xmind = xmind < 0 ? 0 : xmind > this->width - 1 ? this->width - 1
															: xmind;
			xmaxd = xmaxd < 0 ? 0 : xmaxd > this->width - 1 ? this->width - 1
															: xmaxd;
			ymind = ymind < 0 ? 0 : ymind > this->height - 1 ? this->height - 1
															 : ymind;
			ymaxd = ymaxd < 0 ? 0 : ymaxd > this->height - 1 ? this->height - 1
															 : ymaxd;

			int32_t xmin = xmind, xmax = xmaxd, ymin = ymind, ymax = ymaxd;
			if (dx * dy > 0) // 斜向右上，x从0开始，随y增加
			{
				for (int32_t y = ymin; y <= ymax; y++)
				{
					vy = (double)p1y - y;
					uint8_t flag = 0;
					for (int32_t x = xmin; x <= xmax; x++)
					{
						vx = (double)p1x - x;

						vt = vy * dx - vx * dy;
						vt = abs(vt);

						if (vt > l_width) // 在界外
						{
							if (flag == 0)
								continue;
							else
								break;
						}
						else if (flag == 0)
						{
							flag = 1;
							xmin = x;
						}

						vt /= l_width;
						vt = (vt - 1) * 255 / (256 - grayscale) + 1;
						pointread(x, y, rgbx);
						rgbx[0] *= vt;
						rgbx[1] = rgbx[0];
						rgbx[2] = rgbx[0];
						rgbx[3] = rgbx[0];
						pointwrite(x, y, rgbx);
					}
				};
			}
			else // 斜向右下
			{
				for (int32_t y = ymin; y <= ymax; y++)
				{
					vy = (double)p1y - y;
					uint8_t flag = 0;
					for (int32_t x = xmax; x >= xmin; x--)
					{
						vx = (double)p1x - x;

						vt = vy * dx - vx * dy;
						vt = abs(vt);

						if (vt > l_width) // 在界外
						{
							if (flag == 0)
								continue;
							else
								break;
						}
						else if (flag == 0)
						{
							flag = 1;
							xmax = x;
						}

						vt /= l_width;
						pointread(x, y, rgbx);
						rgbx[0] *= vt;
						rgbx[1] = rgbx[0];
						rgbx[2] = rgbx[0];
						rgbx[3] = rgbx[0];
						pointwrite(x, y, rgbx);
					}
				};
			}
		}

		void draw_solid_circle(double px, double py, double r = 0, double edge = 1.5)
		{
			double lim = r + edge + 1;
			double xmind = px - lim, xmaxd = px + lim,
				   ymind = py - lim, ymaxd = py + lim;
			xmind = xmind < 0 ? 0 : xmind > this->width ? this->width
														: xmind;
			xmaxd = xmaxd < 0 ? 0 : xmaxd > this->width ? this->width
														: xmaxd;
			ymind = ymind < 0 ? 0 : ymind > this->height ? this->height
														 : ymind;
			ymaxd = ymaxd < 0 ? 0 : ymaxd > this->height ? this->height
														 : ymaxd;

			lim -= 1;
			uint32_t xmin = xmind, xmax = xmaxd;
			uint32_t ymin = ymind, ymax = ymaxd;
			uint8_t rgbx[4] = {};
			for (uint32_t x = xmin; x < xmax; x++)
			{
				for (uint32_t y = ymin; y < ymax; y++)
				{
					double dx = px - x, dy = py - y;
					double dl = sqrt(dx * dx + dy * dy);
					double color = 0;
					if (dl > lim)
					{
						continue;
					}
					else if (dl > r)
					{
						color = (dl - r) / edge * 256;
					}
					rgbx[0] = floor(color);
					rgbx[1] = rgbx[0];
					rgbx[2] = rgbx[0];
					rgbx[3] = rgbx[0];
					pointwrite(x, y, rgbx);
				}
			}
		}
		void draw_line_segment_pointed(double p1x, double p1y, double p2x, double p2y, double l_width = 1.5)
		{
			draw_line_segment(p1x, p1y, p2x, p2y, l_width);
			draw_solid_circle(p1x, p1y, l_width / 2, l_width);
			draw_solid_circle(p2x, p2y, l_width / 2, l_width);
		}
		void draw_line_segment_pointed_uncovered(double p1x, double p1y, double p2x, double p2y, double l_width = 1.5)
		{
			draw_line_segment_uncovered(p1x, p1y, p2x, p2y, l_width);
			draw_solid_circle(p1x, p1y, l_width / 2, l_width);
			draw_solid_circle(p2x, p2y, l_width / 2, l_width);
		}
		// use text2pic_6x12 to draw text
		void draw_text_6x12(double px, double py, const char *text, uint8_t background_color = 0, uint8_t textcolor = 255)
		{
			uint32_t x = px, y = py;
			uint8_t rgbx[4] = {};
			text2pic_7x12 t2p;
			// loop until the end of the text
			for (; *text != '\0'; text++)
			{
				// get the character
				char c = *text;
				switch (c)
				{
				case '\n':
					y += 12;
					x = px;
					break;
				case '\t':
					x += 6 * 4;
					break;
				case '\b':
					x -= 6;
				default:
					// make as bitmap
					if (t2p.make(c) == 0)
					{
						// draw the character
						for (uint32_t j = 0; j < 12; j++)
						{
							for (uint32_t i = 0; i < 7; i++)
							{
								if (t2p.pic.read_unsafe(i, j))
								{
									rgbx[0] = textcolor;
									rgbx[1] = textcolor;
									rgbx[2] = textcolor;
									rgbx[3] = textcolor;
								}
								else
								{
									rgbx[0] = background_color;
									rgbx[1] = background_color;
									rgbx[2] = background_color;
									rgbx[3] = background_color;
								}
								pointwrite_r(x + 6 - i, y + j, rgbx);
							}
						}
						// move to the next character
						x += 7;
					}
					break;
				}
			}
		}
		void draw_text_7x13(double px, double py, const char *text, uint32_t background_rgba = 0x00000000, uint32_t text_argb = 0x00ffffff)
		{
			uint32_t x = px, y = py;
			uint8_t rgbx[4] = {};
			text2pic_7x13 t2p;
			// loop until the end of the text
			for (; *text != '\0'; text++)
			{
				// get the character
				char c = *text;
				switch (c)
				{
				case '\n':
					y += 13;
					x = px;
					break;
				case '\t':
					x += 7 * 4;
					break;
				case '\b':
					x -= 7;
				default:
					// make as bitmap
					if (t2p.make(c) == 0)
					{
						// draw the character
						for (uint32_t j = 0; j < 13; j++)
						{
							for (uint32_t i = 0; i < 7; i++)
							{
								pointread_r(x + 6 - i, y + j, rgbx);
								if (t2p.pic.read_unsafe(i, j))
								{
									rgbx[0] = text_argb & 0xff;
									rgbx[1] = (text_argb >> 8) & 0xff;
									rgbx[2] = (text_argb >> 16) & 0xff;
									rgbx[3] = (text_argb >> 24) & 0xff;
								}
								else
								{
									rgbx[0] = background_rgba & 0xff;
									rgbx[1] = (background_rgba >> 8) & 0xff;
									rgbx[2] = (background_rgba >> 16) & 0xff;
									rgbx[3] = (background_rgba >> 24) & 0xff;
								}
								pointwrite_r(x + 6 - i, y + j, rgbx);
							}
						}
						// move to the next character
						x += 7;
					}
					break;
				}
			}
		}
#ifdef _WINGDI_
		int display(HWND hwnd = NULL)
		{
			presave();
			return show_on_v2(hwnd);
		}
		// show bmp in window
		int show_on(HWND hwnd = NULL)
		{
			if (hwnd == NULL)
			{
				// get current program window
				hwnd = GetForegroundWindow();
			}

			HDC hdc = GetDC(hwnd);
			HDC memdc = CreateCompatibleDC(hdc);
			HBITMAP hbitmap = CreateCompatibleBitmap(hdc, this->width, this->height);
			SelectObject(memdc, hbitmap);
			SetDIBits(memdc, hbitmap, 0, this->height, this->data, (BITMAPINFO *)(this->file + sizeof(BITMAPFILEHEADER)), DIB_RGB_COLORS);
			BitBlt(hdc, 0, 0, this->width, this->height, memdc, 0, 0, SRCCOPY);
			DeleteDC(memdc);
			DeleteObject(hbitmap);
			ReleaseDC(hwnd, hdc);
			return 0;
		}
		// NULL->ForegroundWindow
		int show_on_v2(HWND hwnd = NULL)
		{
			static HWND s_hwnd = NULL;

			if (hwnd == NULL)
			{
				if (s_hwnd == NULL)
				{
					s_hwnd = GetForegroundWindow();
				}
				hwnd = s_hwnd;
			}

			HDC hdc = GetDC(hwnd);
			HDC memdc = CreateCompatibleDC(hdc);
			HBITMAP hbitmap = CreateCompatibleBitmap(hdc, this->width, this->height);
			SelectObject(memdc, hbitmap);
			SetDIBits(memdc, hbitmap, 0, this->height, this->data, (BITMAPINFO *)(this->file + sizeof(BITMAPFILEHEADER)), DIB_RGB_COLORS);
			BitBlt(hdc, 0, 0, this->width, this->height, memdc, 0, 0, SRCCOPY);
			DeleteDC(memdc);
			DeleteObject(hbitmap);
			ReleaseDC(hwnd, hdc);
			return 0;
		}

#endif
		float *matrix_mul_3x3(float *matA, float *matB, float *matRet)
		{
			float temp[9] = {0};
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						temp[i * 3 + j] += matA[i * 3 + k] * matB[k * 3 + j];
					}
				}
			}
			for (int i = 0; i < 9; i++)
			{
				matRet[i] = temp[i];
			}
			return matRet;
		}
		float *matrix_mul_4x4(float *matA, float *matB, float *matRet)
		{
			float temp[16] = {0};
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					for (int k = 0; k < 4; k++)
					{
						temp[i * 4 + j] += matA[i * 4 + k] * matB[k * 4 + j];
					}
				}
			}
			for (int i = 0; i < 16; i++)
			{
				matRet[i] = temp[i];
			}
			return matRet;
		}
		void rotate_as(bmp *rotated, float rad)
		{
			float sinx = sin(rad), cosx = cos(rad);
			uint8_t rgbx[4] = {0};
			for (uint32_t rot_y = 0; rot_y < rotated->height; rot_y++)
			{
				for (uint32_t rot_x = 0; rot_x < rotated->width; rot_x++)
				{
					// get the original position
					float x = (rot_x - rotated->width / 2) * cosx - (rot_y - rotated->height / 2) * sinx + this->width / 2;
					float y = (rot_x - rotated->width / 2) * sinx + (rot_y - rotated->height / 2) * cosx + this->height / 2;
					uint32_t x0 = x, y0 = y;
					if (x0 >= this->width || y0 >= this->height)
					{
						continue;
					}
					// get the color
					this->pointread(x, y, rgbx);
					// write the color
					rotated->pointwrite(rot_x, rot_y, rgbx);
				}
			}
		}
		void rotate_as_v1(bmp *rotated, float rad)
		{
			float sinx = sin(rad), cosx = cos(rad);
			uint8_t rgbx[2][2][4] = {0}, rgbx2[4] = {0};
			for (uint32_t rot_y = 0; rot_y < rotated->height; rot_y++)
			{
				for (uint32_t rot_x = 0; rot_x < rotated->width; rot_x++)
				{
					// get the original position
					float x = (rot_x - rotated->width / 2) * cosx - (rot_y - rotated->height / 2) * sinx + this->width / 2;
					float y = (rot_x - rotated->width / 2) * sinx + (rot_y - rotated->height / 2) * cosx + this->height / 2;
					uint32_t x0 = x, y0 = y;
					float dx = x - x0, dy = y - y0;
					if (x0 >= this->width || y0 >= this->height)
					{
						continue;
					}
					// get the color of 4 points near the original position
					this->pointread(x, y, rgbx[0][0]);
					this->pointread(x + 1, y, rgbx[0][1]);
					this->pointread(x, y + 1, rgbx[1][0]);
					this->pointread(x + 1, y + 1, rgbx[1][1]);
					// calculate the color
					for (int i = 0; i < 4; i++)
					{
						rgbx2[i] =
							rgbx[0][0][i] * (1 - dx) * (1 - dy) +
							rgbx[0][1][i] * dx * (1 - dy) +
							rgbx[1][0][i] * (1 - dx) * dy +
							rgbx[1][1][i] * dx * dy;
					}

					// write the color
					rotated->pointwrite(rot_x, rot_y, rgbx2);
				}
			}
		}
		void linear_trans_as(bmp *transformed, float *trans_matrix_3x3)
		{
			uint8_t rgbx[2][2][4] = {0}, rgbx_out[4] = {0};
			float mat[3][3] = {0};
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					mat[i][j] = trans_matrix_3x3[i * 3 + j];
				}
			}
			for (uint32_t trans_y = 0; trans_y < transformed->height; trans_y++)
			{
				for (uint32_t trans_x = 0; trans_x < transformed->width; trans_x++)
				{
					float x = mat[0][0] * trans_x + mat[0][1] * trans_y + mat[0][2];
					float y = mat[1][0] * trans_x + mat[1][1] * trans_y + mat[1][2];
					float z = mat[2][0] * trans_x + mat[2][1] * trans_y + mat[2][2];
					x /= z;
					y /= z;
					uint32_t x0 = x, y0 = y;
					float dx = x - x0, dy = y - y0;
					if (x0 >= this->width || y0 >= this->height)
					{
						continue;
					}
					// get the color of 4 points near the original position
					this->pointread(x, y, rgbx[0][0]);
					this->pointread(x + 1, y, rgbx[0][1]);
					this->pointread(x, y + 1, rgbx[1][0]);
					this->pointread(x + 1, y + 1, rgbx[1][1]);
					// calculate the color
					for (int i = 0; i < 4; i++)
					{
						rgbx_out[i] =
							rgbx[0][0][i] * (1 - dx) * (1 - dy) +
							rgbx[0][1][i] * dx * (1 - dy) +
							rgbx[1][0][i] * (1 - dx) * dy +
							rgbx[1][1][i] * dx * dy;
					}

					// write the color
					transformed->pointwrite(trans_x, trans_y, rgbx_out);
				}
			}
		}
		void rotate_trans(bmp *rotated, float rad, float center_x = 0.0f, float center_y = 0.0f)
		{
			float sinx = sin(rad), cosx = cos(rad);
			float rotate_matrix[3][3] =
				{cosx, -sinx, 0.0f,
				 sinx, cosx, 0.0f,
				 0.0f, 0.0f, 1.0f};
			float move_matrix[3][3] =
				{1.0f, 0.0f, center_x,
				 0.0f, 1.0f, center_y,
				 0.0f, 0.0f, 1.0f};
			float move_back_matrix[3][3] =
				{1.0f, 0.0f, -center_x,
				 0.0f, 1.0f, -center_y,
				 0.0f, 0.0f, 1.0f};

			if (center_x != 0.0f || center_y != 0.0f)
			{
				matrix_mul_3x3((float *)move_matrix, (float *)rotate_matrix, (float *)rotate_matrix);
				matrix_mul_3x3((float *)rotate_matrix, (float *)move_back_matrix, (float *)rotate_matrix);
			}

			linear_trans_as(rotated, (float *)rotate_matrix);
			return;
		}
		void move_trans(bmp *moved, float dx, float dy)
		{
			float move_matrix[3][3] =
				{1.0f, 0.0f, dx,
				 0.0f, 1.0f, dy,
				 0.0f, 0.0f, 1.0f};
			linear_trans_as(moved, (float *)move_matrix);
			return;
		}
		void scale_trans(bmp *scaled, float sx, float sy)
		{
			float scale_matrix[3][3] =
				{sx, 0.0f, 0.0f,
				 0.0f, sy, 0.0f,
				 0.0f, 0.0f, 1.0f};
			linear_trans_as(scaled, (float *)scale_matrix);
		}
		float *rotate_matrix_3x3(float rad, float *mat)
		{
			float sinx = sin(rad), cosx = cos(rad);
			float rotate_matrix[3][3] =
				{cosx, -sinx, 0.0f,
				 sinx, cosx, 0.0f,
				 0.0f, 0.0f, 1.0f};
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					mat[i * 3 + j] = rotate_matrix[i][j];
				}
			}
			return mat;
		}
		float *move_matrix_3x3(float dx, float dy, float *mat)
		{
			float move_matrix[3][3] =
				{1.0f, 0.0f, dx,
				 0.0f, 1.0f, dy,
				 0.0f, 0.0f, 1.0f};
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					mat[i * 3 + j] = move_matrix[i][j];
				}
			}
			return mat;
		}
		float *scale_matrix_3x3(float sx, float sy, float *mat)
		{
			float scale_matrix[3][3] =
				{sx, 0.0f, 0.0f,
				 0.0f, sy, 0.0f,
				 0.0f, 0.0f, 1.0f};
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					mat[i * 3 + j] = scale_matrix[i][j];
				}
			}
			return mat;
		}

		void nonlinear_trans_as(bmp *transformed, void transfunc(float *inXY, float *outXY))
		{
			uint8_t rgbx[2][2][4] = {0}, rgbx_out[4] = {0};
			float inXY[2] = {0}, outXY[2] = {0};
			for (uint32_t trans_y = 0; trans_y < transformed->height; trans_y++)
			{
				for (uint32_t trans_x = 0; trans_x < transformed->width; trans_x++)
				{
					inXY[0] = trans_x;
					inXY[1] = trans_y;
					transfunc(inXY, outXY);
					uint32_t x0 = outXY[0], y0 = outXY[1];
					float dx = outXY[0] - x0, dy = outXY[1] - y0;
					if (x0 >= this->width || y0 >= this->height)
					{
						continue;
					}
					// get the color of 4 points near the original position
					this->pointread(x0, y0, rgbx[0][0]);
					this->pointread(x0 + 1, y0, rgbx[0][1]);
					this->pointread(x0, y0 + 1, rgbx[1][0]);
					this->pointread(x0 + 1, y0 + 1, rgbx[1][1]);
					// calculate the color
					for (int i = 0; i < 4; i++)
					{
						rgbx_out[i] =
							rgbx[0][0][i] * (1 - dx) * (1 - dy) +
							rgbx[0][1][i] * dx * (1 - dy) +
							rgbx[1][0][i] * (1 - dx) * dy +
							rgbx[1][1][i] * dx * dy;
					}

					// write the color
					transformed->pointwrite(trans_x, trans_y, rgbx_out);
				}
			}
		}

		bmp *new_rotate_as(float rad)
		{
			// use original bmp to create a new bmp
			bmp *ret = new bmp(this);
			// rotate the new bmp
			this->rotate_as(ret, rad);
			return ret;
		}
		bmp *new_rotate_as_v1(float rad)
		{
			// use original bmp to create a new bmp
			bmp *ret = new bmp(this);
			// rotate the new bmp
			this->rotate_as_v1(ret, rad);
			return ret;
		}

		bmp *new_partial_derivative_X()
		{
			bmp *ret = new bmp(bmphd(this->width, this->height, 24));
			uint8_t rgbx_thisp[4] = {0}, rgbx_lastp[4] = {0}, rgbx[4] = {0};
			for (uint32_t y = 0; y < this->height; y++)
			{
				for (uint32_t x = 1; x < this->width; x++)
				{
					this->pointread(x, y, rgbx_thisp);
					this->pointread(x - 1, y, rgbx_lastp);
					// the difference value is between -255~255,
					// scaling it to 0~255,by adding 255 and dividing 2
					rgbx[0] = (rgbx_thisp[0] - rgbx_lastp[0] + 255) / 2;
					rgbx[1] = (rgbx_thisp[1] - rgbx_lastp[1] + 255) / 2;
					rgbx[2] = (rgbx_thisp[2] - rgbx_lastp[2] + 255) / 2;
					rgbx[3] = (rgbx_thisp[3] - rgbx_lastp[3] + 255) / 2;
					ret->pointwrite(x, y, rgbx);
				}
			}
			// process the first column
			for (uint32_t y = 0; y < this->height; y++)
			{
				this->pointread(0, y, rgbx_thisp);
				rgbx[0] = (rgbx_thisp[0] + 255) / 2;
				rgbx[1] = (rgbx_thisp[1] + 255) / 2;
				rgbx[2] = (rgbx_thisp[2] + 255) / 2;
				rgbx[3] = (rgbx_thisp[3] + 255) / 2;
				ret->pointwrite(0, y, rgbx);
			}
			return ret;
		}
		bmp *new_partial_derivative_Y()
		{
			bmp *ret = new bmp(bmphd(this->width, this->height, 24));
			uint8_t rgbx_thisp[4] = {0}, rgbx_lastp[4] = {0}, rgbx[4] = {0};

			for (uint32_t y = 1; y < this->height; y++)
			{
				for (uint32_t x = 0; x < this->width; x++)
				{
					this->pointread(x, y, rgbx_thisp);
					this->pointread(x, y - 1, rgbx_lastp);
					// the difference value is between -255~255,
					// scaling it to 0~255,by adding 255 and dividing 2
					rgbx[0] = (rgbx_thisp[0] - rgbx_lastp[0] + 255) / 2;
					rgbx[1] = (rgbx_thisp[1] - rgbx_lastp[1] + 255) / 2;
					rgbx[2] = (rgbx_thisp[2] - rgbx_lastp[2] + 255) / 2;
					rgbx[3] = (rgbx_thisp[3] - rgbx_lastp[3] + 255) / 2;
					ret->pointwrite(x, y, rgbx);
				}
			}
			// process the first row
			for (uint32_t x = 0; x < this->width; x++)
			{
				this->pointread(x, 0, rgbx_thisp);
				rgbx[0] = (rgbx_thisp[0] + 255) / 2;
				rgbx[1] = (rgbx_thisp[1] + 255) / 2;
				rgbx[2] = (rgbx_thisp[2] + 255) / 2;
				rgbx[3] = (rgbx_thisp[3] + 255) / 2;
				ret->pointwrite(x, 0, rgbx);
			}
			return ret;
		}
	};
	inline void bmp_mixup_pixel(bmp &base, bmp &add, uint32_t x, uint32_t y, float base_percentage)
	{
		uint8_t rgbbase[4] = {0}, rgbadd[4] = {0};
		base.pointread(x, y, rgbbase);
		add.pointread(x, y, rgbadd);
		for (int i = 0; i < 4; i++)
		{
			rgbbase[i] = rgbbase[i] * base_percentage + rgbadd[i] * (1 - base_percentage);
		}
		base.pointwrite(x, y, rgbbase);
		return;
	}
	void bmp_mixup_block(bmp &base, bmp &add, uint32_t x, uint32_t y, uint32_t wid, uint32_t hei, float base_percentage)
	{
		uint32_t xw = x + wid, yh = y + hei;
		if (xw > base.width)
			xw = base.width;
		if (yh > base.height)
			yh = base.height;
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;
		for (int i = x; i < xw; i++)
		{
			for (int j = y; j < yh; j++)
			{
				bmp_mixup_pixel(base, add, i, j, base_percentage);
			}
		}
		return;
	}

	// color in rgb[3]
	bmp *bmp_mixup_color(bmp &base)
	{
		bmphd bhd;
		bhd.init(base.width, base.height, 24);
		bmp *ret = new bmp(bhd);
		uint8_t color[4] = {0};
		for (int i = 0; i < 3; i++)
		{
			color[i] = base.rgb[i];
		}
		for (int i = 0; i < base.width; i++)
		{
			for (int j = 0; j < base.height; j++)
			{
				base.pointread(i, j);
				for (int p = 0; p < 3; p++)
				{
					ret->rgb[p] = (float)base.rgb[p] * (255 - base.rgb[3]) / 255 + color[p] * base.rgb[3] / 255;
				}
				ret->pointwrite(i, j);
			}
		}
		return ret;
	}

	bmp *mix_funcal(bmp *base, bmp *another, void (*func)(bmp *b, bmp *a))
	{

		for (uint32_t y = 0; y < base->height; y++)
		{
			for (uint32_t x = 0; x < base->width; x++)
			{
				base->pointread(x, y);
				another->pointread(x, y);
				base->x = x;
				base->y = y;
				func(base, another);
				base->pointwrite(x, y);
			}
		}
		return base;
	}

	template <typename dataType = float>
	class camera
	{
	private:
		int width = 0;
		int height = 0;
		dataType *raw_data = NULL;

	public:
		// long exposure
		// inputs a series of bmps, outputs a bmp like long exposure
		typedef double (*fn_f64__f64)(double);
		void long_exposure(bmp *picin, float exposure_time = 1.0f, fn_f64__f64 none_linear_transform = 0)
		{
			// set width and height
			if (width == 0)
			{
				width = picin->width;
				height = picin->height;
				if (raw_data != NULL)
					delete[] raw_data;
				raw_data = new dataType[width * height * 3]{};
			}
			// clear raw_data
			memset(raw_data, 0, width * height * 4);
			int BYTE_PER_PIXEL = picin->bitdepth / 8;
			// add picin to raw_data
			if (none_linear_transform == 0)
			{
				for (int i = 0; i < width * height; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						raw_data[i * 3 + j] += picin->data[i * BYTE_PER_PIXEL + j] * exposure_time;
					}
				}
			}
			else
			{
				for (int i = 0; i < width * height; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						raw_data[i * 3 + j] += none_linear_transform(picin->data[i * BYTE_PER_PIXEL + j]) * exposure_time;
					}
				}
			}
		}

		bmp *get_pic(fn_f64__f64 none_linear_transform = 0)
		{
			bmp *ret = new bmp(bmphd(width, height, 24));

			// get the range of raw_data
			dataType max = 0;
			if (none_linear_transform == 0)
			{
				for (int i = 0; i < width * height; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						if (raw_data[i * 3 + j] > max)
							max = raw_data[i * 3 + j];
					}
				}
			}
			else
			{
				for (int i = 0; i < width * height; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						raw_data[i * 3 + j] = none_linear_transform(raw_data[i * 3 + j]);
						if (raw_data[i * 3 + j] > max)
							max = raw_data[i * 3 + j];
					}
				}
			}
			// map the value of raw_data to ret
			for (int i = 0; i < width * height; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					ret->data[i * 3 + j] = (uint8_t)((double)raw_data[i * 3 + j] / max * 255);
				}
			}
			return ret;
		}
		~camera()
		{
			if (raw_data != NULL)
				delete[] raw_data;
		}
	};

#ifdef _WINGDI_

	bool
	show_bmp(uint8_t *mempic, HWND hwnd = NULL)
	{
		HDC hdc = GetDC(hwnd);
		RECT rect;
		if (hwnd == NULL)
		{
			GetClientRect(GetDesktopWindow(), &rect);
		}
		else
		{
			GetClientRect(hwnd, &rect);
		}
		// get the hwnd of this program

		HDC memdc = CreateCompatibleDC(hdc);
		HBITMAP hbitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
		HGDIOBJ oldobj = SelectObject(memdc, hbitmap);
		BITMAPFILEHEADER *bfh = (BITMAPFILEHEADER *)mempic;
		BITMAPINFOHEADER *bih = (BITMAPINFOHEADER *)(mempic + sizeof(BITMAPFILEHEADER));
		RGBQUAD *palette = (RGBQUAD *)(mempic + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
		BYTE *bits = (BYTE *)(mempic + bfh->bfOffBits);
		SetDIBits(memdc, hbitmap, 0, bih->biHeight, bits, (BITMAPINFO *)bih, DIB_RGB_COLORS);

		BitBlt(hdc, rect.left, rect.top, rect.right - rect.left,
			   rect.bottom - rect.top,
			   memdc, 0, 0,
			   SRCCOPY);
		SelectObject(memdc, oldobj);
		DeleteObject(hbitmap);
		DeleteDC(memdc);
		ReleaseDC(NULL, hdc);
		return TRUE;
	};
	bool show_bmp_v2(uint8_t *mempic, HWND hwnd = NULL)
	{

		static HWND s_hwnd = NULL;

		if (hwnd == NULL)
		{
			if (s_hwnd == NULL)
			{
				s_hwnd = GetForegroundWindow();
			}
			hwnd = s_hwnd;
		}
		HDC hdc = GetDC(hwnd);
		RECT rect;
		GetClientRect(hwnd, &rect);

		// get the hwnd of this program

		HDC memdc = CreateCompatibleDC(hdc);
		HBITMAP hbitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
		HGDIOBJ oldobj = SelectObject(memdc, hbitmap);
		BITMAPFILEHEADER *bfh = (BITMAPFILEHEADER *)mempic;
		BITMAPINFOHEADER *bih = (BITMAPINFOHEADER *)(mempic + sizeof(BITMAPFILEHEADER));
		RGBQUAD *palette = (RGBQUAD *)(mempic + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
		BYTE *bits = (BYTE *)(mempic + bfh->bfOffBits);
		SetDIBits(memdc, hbitmap, 0, bih->biHeight, bits, (BITMAPINFO *)bih, DIB_RGB_COLORS);

		BitBlt(hdc, rect.left, rect.top, rect.right - rect.left,
			   rect.bottom - rect.top,
			   memdc, 0, 0,
			   SRCCOPY);
		SelectObject(memdc, oldobj);
		DeleteObject(hbitmap);
		DeleteDC(memdc);
		ReleaseDC(NULL, hdc);
		return TRUE;
	};

#endif // _WINGDI_
#ifdef namespace_bionukg
};
#endif // namespace_bionukg
#endif // !_bionukg_graphics_h
