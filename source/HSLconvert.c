#include "HSLconvert.h"
double _RGBmax(double _R, double _G, double _B) {
	if (_R > _G && _R > _B)
		return _R;
	else if (_G > _B)
		return _G;
	else
		return _B;
}
double _RGBmin(double _R, double _G, double _B) {
	if (_R < _G && _R < _B)
		return _R;
	else if (_G < _B)
		return _G;
	else
		return _B;
}

HSLColor *convertRGBtoHSL(BMP_Header Header, color *RGB) {
	HSLColor *HSL = calloc(Header.Height*Header.Width, sizeof(HSLColor));
	double _R, _G, _B;
	double Cmax, Cmin, Delta;
	for (int i = 0; i < Header.Height; i++)
		for (unsigned int j = 0; j < Header.Width; j++) {
			int index = i*Header.Width + j;
			_R = RGB[index].R / 255.0;
			_G = RGB[index].G / 255.0;
			_B = RGB[index].B / 255.0;

			Cmax = _RGBmax(_R, _G, _B);
			Cmin = _RGBmin(_R, _G, _B);
			Delta = Cmax - Cmin;
			if (Delta == 0) {
				HSL[index].H = 0;
			}
			else if (Cmax == _R) {
				HSL[index].H = (int)(60 * ((_G - _B) / Delta));
			}
			else if (Cmax == _G) {
				HSL[index].H = (int)(60 * (((_B - _R) / Delta) + 2));
			}
			else if (Cmax == _B) {
				HSL[index].H = (int)(60 * (((_R - _G) / Delta) + 4));
			}

			if (HSL[index].H < 0) HSL[index].H += 360;

			HSL[index].L = (Cmax + Cmin) * 0.5;

			if (HSL[index].L == 1)
				HSL[index].S = 0;
			else
				HSL[index].S = Delta / (1 - fabs(2 * HSL[index].L - 1));
		}
	return HSL;
}
