#include "ReadBMP.h"
uint16_t uint16_tConvertBigEndian(uint16_t data) {
	return (data << 8) | (data >> 8);
}

uint32_t uint32_tConvertBigEndian(uint32_t data) {
	return ((data >> 24)) | ((data >> 8) & 0x00ff00) | ((data << 8) & 0xff0000) | ((data << 24));
}
void BMPHeaderRead(FILE *BMPFILE, BMP_Header *Header) {
	fseek(BMPFILE, 0x00, SEEK_SET);
	fread(&Header->Identifier, sizeof(Header->Identifier), 2, BMPFILE);

	fseek(BMPFILE, 0x02, SEEK_SET);
	fread(&Header->FileSize, sizeof(Header->FileSize), 4, BMPFILE);

	fseek(BMPFILE, 0x0A, SEEK_SET);
	fread(&Header->BitmapDataOffset, sizeof(Header->BitmapDataOffset), 4, BMPFILE);


	fseek(BMPFILE, 0x0E, SEEK_SET);
	fread(&Header->BitmapHeaderSize, sizeof(Header->BitmapHeaderSize), 4, BMPFILE);

	fseek(BMPFILE, 0x12, SEEK_SET);
	fread(&Header->Width, sizeof(Header->Width), 4, BMPFILE);

	fseek(BMPFILE, 0x16, SEEK_SET);
	fread(&Header->Height, sizeof(Header->Height), 4, BMPFILE);

	fseek(BMPFILE, 0x1A, SEEK_SET);
	fread(&Header->Planes, sizeof(Header->Planes), 2, BMPFILE);

	fseek(BMPFILE, 0x1C, SEEK_SET);
	fread(&Header->BitsPerPixel, sizeof(Header->BitsPerPixel), 2, BMPFILE);

	fseek(BMPFILE, 0x1E, SEEK_SET);
	fread(&Header->Compression, sizeof(Header->Compression), 4, BMPFILE);

	fseek(BMPFILE, 0x22, SEEK_SET);
	fread(&Header->BitmapDataSize, sizeof(Header->BitmapDataSize), 4, BMPFILE);
	/*
	fseek(BMPFILE, 0x0026, SEEK_SET);
	fread(&Header->HResolution, sizeof(Header->HResolution), 4, BMPFILE);

	fseek(BMPFILE, 0X002A, SEEK_SET);
	fread(&Header->VResolution, sizeof(Header->VResolution), 4, BMPFILE);

	fseek(BMPFILE, 0X002E, SEEK_SET);
	fread(&Header->UsedColors, sizeof(Header->UsedColors), 4, BMPFILE);

	fseek(BMPFILE, 0X0032, SEEK_SET);
	fread(&Header->ImportantColors, sizeof(Header->ImportantColors), 4, BMPFILE);
	*/
}

void convertBMPDataToBigEndian(BMP_Header *Header) {
	Header->Identifier = uint16_tConvertBigEndian(Header->Identifier);
	//Header->FileSize = uint32_tConvertBigEndian(Header->FileSize);
	//Header->BitmapDataOffset = uint32_tConvertBigEndian(Header->BitmapDataOffset);
	//Header->BitmapDataSize = uint32_tConvertBigEndian(Header->BitmapDataSize);
}
void BMPPrintHeader(BMP_Header *Header) {
	printf("\n--------------BMP Header Info-----------------\n");
	printf("Identifier \t\t:");
	printf("'%c", Header->Identifier >> 8);
	printf("%c'\n", Header->Identifier & 0xff);

	printf("FileSize \t\t: ");
	printf("%zu Byte\n", Header->FileSize);
	//printf("0x%08x\n", Header->FileSize);

	printf("Bitmap Data Offset\t: ");
	printf("0x%x Byte\n", Header->BitmapDataOffset);

	printf("Bitmap Header Size\t: ");
	printf("0x%x Byte\n", Header->BitmapHeaderSize);

	printf("Width\t\t\t: ");
	printf("%zu Pixel\n", Header->Width);

	printf("Height\t\t\t: ");
	printf("%zu Pixel\n", Header->Height);

	printf("Planes\t\t\t: ");
	printf("%zu\n", Header->Planes);

	printf("Bit Per Pixel\t\t: ");
	printf("%zu\n", Header->BitsPerPixel);

	printf("Compression\t\t: ");
	printf("%zu\n", Header->Compression);

	printf("Bitmap Data Size\t: ");
	printf("%x\n", Header->BitmapDataSize);
	/*
	printf("H-Resolution\t\t: ");
	printf("%zu\n",Header->HResolution);

	printf("V-Resolution\t\t: ");
	printf("%zu\n", Header->VResolution);

	printf("Used Colors\t\t:");
	printf("%zu\n", Header->UsedColors);

	printf("Important Colors\t:");
	printf("%zu\n", Header->ImportantColors);
	*/
	printf("---------------------------------------------\n");
}

color *getBitMap(BMP_Header Header, FILE *BMPFILE) {
	color *BMPColor = calloc(Header.Height*Header.Width, sizeof(color));
	int address = Header.BitmapDataOffset;
	for (int i = 0; i < Header.Height; i++) {
		static int alignment = 0;
		for (unsigned int j = 0; j < Header.Width; j++) {
			int index = i*Header.Width + j;

			fseek(BMPFILE, address++, SEEK_SET);
			fread(&BMPColor[index].B, 1, 1, BMPFILE);

			fseek(BMPFILE, address++, SEEK_SET);
			fread(&BMPColor[index].G, 1, 1, BMPFILE);

			fseek(BMPFILE, address++, SEEK_SET);
			fread(&BMPColor[index].R, 1, 1, BMPFILE);
		}
		address += Header.Width % 4;
	}
	return BMPColor;
}

void printPixelData(BMP_Header Header, color *BMPColor) {
	for (int i = 0; i < Header.Height; i++) {
		for (unsigned int j = 0; j < Header.Width; j++) {
			int index = i*Header.Width + j;
			//printf("%d",index);
			printf("%02X,", BMPColor[index].R);
			printf("%02X,", BMPColor[index].G);
			printf("%02X\t", BMPColor[index].B);
		}
		printf("\n");
	};

}