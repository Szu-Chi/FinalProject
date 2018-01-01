#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include "printImage.h"




int main(void) {
	int pixel = 0;
	char PATH[80] = "images.bmp";
	printf("現在所在位置 : ");
	system("ECHO %CD%");
	printf("輸入影像檔案 : ");
	scanf_s("%s",PATH,sizeof(PATH));
	printf("開啟%s\n",PATH);
	FILE *BMPFILE;
	FILE *OUTPUTFILE;
	BMP_Header Header;
	errno_t OpenBMP, OpenOUTPUT;
	OpenBMP = fopen_s(&BMPFILE, PATH, "rb");
	OpenOUTPUT = fopen_s(&OUTPUTFILE, "output.txt", "w");
	if (OpenBMP || OpenOUTPUT) {
		printf("Open File Error!!\n");
		system("pause");
		return 1;
	}
	else {
		printf("Open Feil success\n");
	}
	BMPHeaderRead(BMPFILE, &Header);
	convertBMPDataToBigEndian(&Header);
	BMPPrintHeader(&Header);
	
	color *BMPColor = getBitMap(Header, BMPFILE);
	
	printImagesInConsole(Header,BMPColor,500,100);//BMP_Header, BMP_ColorData, x, y

	color *BMPGrayscale = Grayscale(Header, BMPColor);
	printImagesInConsole(Header, BMPGrayscale, 500 + Header.Width, 100);

	color *BMPLostFocus = LostFocus(Header, BMPColor, 3);
	printImagesInConsole(Header, BMPLostFocus, 500, 100 + Header.Height);
	/*
	color *err = errorImage(Header,BMPGrayscale);
	printImagesInConsole(Header, err, 500 + Header.Width, 100 + Header.Height);
	*/
	color * BMPDithering = FloydSteinbergDithering(Header, BMPColor);
	printImagesInConsole(Header, BMPDithering, 500 + Header.Width, 100 + Header.Height);
	


	
	/*
	for(int i = Header.Height - 1; i >= 0; i--) {
		for(unsigned int j = 0; j < Header.Width; j++) {
			int index = i*Header.Width + j;
			if (BMPColor[index].R == 0xFF && BMPColor[index].G == 0xFF && BMPColor[index].B == 0xFF)
				fprintf(OUTPUTFILE,"  ");
			else
				fprintf(OUTPUTFILE, "██");
		}
		fprintf(OUTPUTFILE, "\n");
	}
	*/
	
	free(BMPColor);
	free(BMPGrayscale);
	free(BMPLostFocus);
	free(BMPDithering);
	
	fclose(BMPFILE);
	fclose(OUTPUTFILE);
	system("pause");
	return 0;
}