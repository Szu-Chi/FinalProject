#include <windows.h>
#include <stdint.h>
#include "HSLconvert.h"

void printImagesInConsole(BMP_Header Header, color *BMPColor, int x, int y) {
	//Get a console handle
	HWND myconsole = GetConsoleWindow();
	//Get a handle to device context
	HDC mydc = GetDC(myconsole);
	COLORREF COLOR;
	for (int i = Header.Height - 1; i >= 0; i--) {
		for (unsigned int j = 0; j < Header.Width; j++) {
			int index = i*Header.Width + j;
			COLOR = RGB(BMPColor[index].R, BMPColor[index].G, BMPColor[index].B);
			SetPixel(mydc, j + x, Header.Height - 1 - i + y, COLOR);
		}
	}
	ReleaseDC(myconsole, mydc);
}

color *Grayscale(BMP_Header Header, color *BMPColor) {
	color *data = calloc(Header.Height*Header.Width, sizeof(color));
	for (int i = Header.Height - 1; i >= 0; i--) {
		for (unsigned int j = 0; j < Header.Width; j++) {
			int index = i*Header.Width + j;
			int avg = (BMPColor[index].R + BMPColor[index].G + BMPColor[index].B) / 3;
			data[index].R = avg;
			data[index].G = avg;
			data[index].B = avg;
		}
	}
	return data;
}

color getAverColor(BMP_Header Header, color *BMPColor, int x, int y, int range) {
	color  avg;
	long long R, G, B;
	R = 0;
	G = 0;
	B = 0;
	if (y - range >= 0 && x - range >= 0 && y + range < Header.Height && x + (unsigned)range < Header.Width) {
		for (int i = y - range; i < y + range + 1; i++)
			for (int j = x - range; j < x + range + 1; j++) {
				int index = i*Header.Width + j;
				R += BMPColor[index].R;
				G += BMPColor[index].G;
				B += BMPColor[index].B;
			}

		avg.R = (uint8_t)(R / pow(2 * range + 1, 2));
		avg.G = (uint8_t)(G / pow(2 * range + 1, 2));
		avg.B = (uint8_t)(B / pow(2 * range + 1, 2));

	}
	else {
		int index = y*Header.Width + x;
		avg.R = 0;
		avg.G = 0;
		avg.B = 0;
	}
	return avg;
}

color *LostFocus(BMP_Header Header, color *BMPColor, int range) {
	color *data = calloc(Header.Height*Header.Width, sizeof(color));
	for (int i = Header.Height - 1; i >= 0; i--) {
		for (unsigned int j = 0; j < Header.Width; j++) {
			color avg;
			int index = i*Header.Width + j;
			avg = getAverColor(Header, BMPColor, j, i, range);
			data[index] = avg;
		}
	}
	return data;
}

color *BinaryImage(BMP_Header Header, color* Gray) {
	HSLColor *BMP_HSLColor;
	BMP_HSLColor = convertRGBtoHSL(Header, Gray);

	double totalL = 0;
	for (int i = 0; i < Header.Height; i++)
		for (unsigned int j = 0; j < Header.Width; j++) {
			int index = i*Header.Width + j;
			totalL += BMP_HSLColor[i].L;
		}
	double averageL = totalL / (Header.Width * Header.Height);

	color *data = calloc(Header.Height * Header.Width, sizeof(color));
	for (int i = 0; i < Header.Height; i++)
		for (unsigned int j = 0; j < Header.Width; j++) {
			int index = i*Header.Width + j;
			if (BMP_HSLColor[index].L > averageL) {
				data[index].R = 255;
				data[index].G = 255;
				data[index].B = 255;
			}
			else {
				data[index].R = 0;
				data[index].G = 0;
				data[index].B = 0;
			}
		}
	free(BMP_HSLColor);
	return data;
}

color *FloydSteinbergDithering(BMP_Header Header, color *BMPColor) {
	color *Gray = Grayscale(Header, BMPColor);
	color *Binary = BinaryImage(Header, Gray);
	//color *data = calloc(Header.Height*Header.Width, sizeof(color));
	for (int i = Header.Height - 2; i >= 1; i--) {
		for (unsigned int j = 1; j < Header.Width - 1; j++) {
			int index = i*Header.Width + j;
			int index2 = (i + 1)*Header.Width + j;
			Gray[index2].R = Gray[index].R;
			Gray[index2].B = Gray[index].B;
			Gray[index2].G = Gray[index].G;
			/*
			color avg = getAverColor(Header, Gray, j, i, 1);
			color err;
			err.R = Gray[index].R - avg.R;
			err.G = Gray[index].G - avg.G;
			err.B = Gray[index].B - avg.B;

			Gray[index].R = avg.R;
			Gray[index].G = avg.G;
			Gray[index].B = avg.B;

			Gray[index + 1].R += err.R * 7 / 16;
			Gray[index2 - 1].R += err.R * 3 / 16;
			Gray[index2].R += err.R * 5 / 16;
			Gray[index2 + 1].R += err.R * 1 / 16;

			Gray[index + 1].G += err.G * 7 / 16;
			Gray[index2 - 1].G += err.G * 3 / 16;
			Gray[index2].G += err.G * 5 / 16;
			Gray[index2 + 1].G += err.G * 1 / 16;

			Gray[index + 1].B += err.B * 7 / 16;
			Gray[index2 - 1].B += err.B * 3 / 16;
			Gray[index2].B += err.B * 5 / 16;
			Gray[index2 + 1].B += err.B * 1 / 16;*/
		}
	}
	free(Gray);
	return Binary;
}
