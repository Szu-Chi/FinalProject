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
	OpenOUTPUT = fopen_s(&OUTPUTFILE, "output.bmp", "wb");
	if (OpenBMP || OpenOUTPUT) {
		printf("Open File Error!!\n");
		system("pause");
		return 1;
	}
	else {
		printf("Open Feil success\n");
	}
	BMPHeaderRead(BMPFILE, &Header);
	BMPPrintHeader(&Header);
	
	color *BMPColor = getBitMap(Header, BMPFILE);
	printImagesInConsole(Header,BMPColor,500,100);//BMP_Header, BMP_ColorData, x, y

	color *BMPGrayscale = Grayscale(Header, BMPColor);
	printImagesInConsole(Header, BMPGrayscale, 500 + Header.Width, 100);

	color *BMPReverse = Reverse(Header, BMPColor);
	printImagesInConsole(Header, BMPReverse, 500 + Header.Width * 2, 100);

	color *BMPLostFocus = LostFocus(Header, BMPColor, 3);
	printImagesInConsole(Header, BMPLostFocus, 500, 100 + Header.Height);

	color *BMPDithering_Gray = FloydSteinbergDithering(Header, BMPGrayscale);
	printImagesInConsole(Header, BMPDithering_Gray, 500 + Header.Width, 100 + Header.Height);
	
	color *BMPDithering = FloydSteinbergDithering(Header, BMPColor);
	printImagesInConsole(Header, BMPDithering, 500 + Header.Width * 2, 100 + Header.Height);	
	
	color *BMPFaded = Faded(Header, BMPDithering, 128);

	printf("Output? (Y/N)");
	char Ans;
	getchar();
	scanf_s("%c", &Ans,sizeof(Ans));
	if (Ans == 'Y' || Ans == 'y') {
		printf("Select Output: \n");
		printf(" 1. Grayscale\n");
		printf(" 2. Reverse\n");
		printf(" 3. LostFocus\n");
		printf(" 4. Dithering_Gray\n");
		printf(" 5. Dithering\n");
		int choose;
		scanf_s("%d", &choose);
		switch (choose) {
		case 1:
			printf("Outputing...\n");
			BMPOutput(OUTPUTFILE, Header, BMPGrayscale);
			printf("Finish!!\n");
			break;
		case 2:
			printf("Outputing...\n");
			BMPOutput(OUTPUTFILE, Header, BMPReverse);
			printf("Finish!!\n");
			break;
		case 3:
			printf("Outputing...\n");
			BMPOutput(OUTPUTFILE, Header, BMPLostFocus);
			printf("Finish!!\n");
			break;
		case 4:
			printf("Outputing...\n");
			BMPOutput(OUTPUTFILE, Header, BMPDithering_Gray);
			printf("Finish!!\n");
			break;
		case 5:
			printf("Outputing...\n");
			BMPOutput(OUTPUTFILE, Header, BMPDithering);
			printf("Finish!!\n");
			break;
		default:
			printf("ERROR INPUT!!!");
			break;
		}
	}
	
	
	free(BMPColor);
	free(BMPGrayscale);
	free(BMPLostFocus);
	free(BMPDithering);
	free(BMPDithering_Gray);
	free(BMPFaded);
	free(BMPReverse);

	fclose(BMPFILE);
	fclose(OUTPUTFILE);
	system("pause");
	return 0;
}