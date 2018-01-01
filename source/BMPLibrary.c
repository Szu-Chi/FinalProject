#include "BMPLibrary.h"
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

void BMPOutput(FILE *Output, BMP_Header Header, color* color) {
	/*----------------Header--------------------*/
	fseek(Output, 0x00, SEEK_SET);
	fwrite(&Header.Identifier, sizeof(Header.Identifier), 2, Output);

	fseek(Output, 0x02, SEEK_SET);
	fwrite(&Header.FileSize, sizeof(Header.FileSize), 4, Output);

	fseek(Output, 0x0A, SEEK_SET);
	fwrite(&Header.BitmapDataOffset, sizeof(Header.BitmapDataOffset), 4, Output);


	fseek(Output, 0x0E, SEEK_SET);
	fwrite(&Header.BitmapHeaderSize, sizeof(Header.BitmapHeaderSize), 4, Output);

	fseek(Output, 0x12, SEEK_SET);
	fwrite(&Header.Width, sizeof(Header.Width), 4, Output);

	fseek(Output, 0x16, SEEK_SET);
	fwrite(&Header.Height, sizeof(Header.Height), 4, Output);

	fseek(Output, 0x1A, SEEK_SET);
	fwrite(&Header.Planes, sizeof(Header.Planes), 2, Output);

	fseek(Output, 0x1C, SEEK_SET);
	fwrite(&Header.BitsPerPixel, sizeof(Header.BitsPerPixel), 2, Output);

	fseek(Output, 0x1E, SEEK_SET);
	fwrite(&Header.Compression, sizeof(Header.Compression), 4, Output);

	fseek(Output, 0x22, SEEK_SET);
	fwrite(&Header.BitmapDataSize, sizeof(Header.BitmapDataSize), 4, Output);


	/*-----------------BitMap-------------------*/
	int address = Header.BitmapDataOffset;
	for (int i = 0; i < Header.Height; i++) {
		for (unsigned int j = 0; j < Header.Width; j++) {
			int index = i*Header.Width + j;

			fseek(Output, address++, SEEK_SET);
			fwrite(&color[index].B, 1, 1, Output);

			fseek(Output, address++, SEEK_SET);
			fwrite(&color[index].G, 1, 1, Output);

			fseek(Output, address++, SEEK_SET);
			fwrite(&color[index].R, 1, 1, Output);
		}
		address += Header.Width % 4;
	}
	putc(0x00,Output);
}


void BMPPrintHeader(BMP_Header *Header) {
	printf("\n--------------BMP Header Info-----------------\n");
	printf("Identifier \t\t:");
	printf("'%c", Header->Identifier & 0xff);
	printf("%c'\n", Header->Identifier >> 8);

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
	}
}