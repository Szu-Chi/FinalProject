#include <stdint.h>
#include <math.h>
#include "BMPLibrary.h"
typedef struct _HSLColor {
	int H;
	double S;
	double L;
}HSLColor;

double _RGBmax(double , double , double );
double _RGBmin(double , double , double);

HSLColor *convertRGBtoHSL(BMP_Header, color*);