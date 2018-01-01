#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
uint16_t uint16_tConvertBigEndian(uint16_t);
uint32_t uint32_tConvertBigEndian(uint32_t);
typedef struct RBGcolor {
	uint8_t R;
	uint8_t G;
	uint8_t B;
}color;
typedef struct _BMP_Header {
	/*Bitmap File Header*/
	uint16_t Identifier;
	uint32_t FileSize;
	uint32_t Reserved;
	uint32_t BitmapDataOffset;
	/*Bitmap Info Header*/
	uint32_t BitmapHeaderSize;
	uint32_t Width;
	int32_t Height;
	uint16_t Planes;
	uint16_t BitsPerPixel;
	uint32_t Compression;
	uint32_t BitmapDataSize;
	uint32_t HResolution;
	uint32_t VResolution;
	uint32_t UsedColors;
	uint32_t ImportantColors;
}BMP_Header;

void BMPHeaderRead(FILE*, BMP_Header*);
void BMPOutput(FILE*, BMP_Header, color*);
void BMPPrintHeader(BMP_Header*);
color *getBitMap(BMP_Header, FILE*);
void printPixelData(BMP_Header, color*);