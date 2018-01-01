#include <windows.h>
#include <stdint.h>
#include "ReadBMP.h"

void printImagesInConsole(BMP_Header, color*, int, int);
color *Grayscale(BMP_Header, color*);
color getAverColor(BMP_Header, color*, int , int , int);
color *LostFocus(BMP_Header, color*, int);
color *BinaryImage(BMP_Header, color*);
color *copyImage(BMP_Header, color*);
color *FloydSteinbergDithering(BMP_Header, color*);
