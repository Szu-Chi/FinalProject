#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
int main(void) {
	int R, G, B;
	scanf_s("%d%d%d",&R,&G,&B);
	double _R = R / 255.0;
	double _G = G / 255.0;
	double _B = B / 255.0;

	double Cmax = _RGBmax(_R,_G,_B);
	double Cmin = _RGBmin(_R,_G,_B);
	double Delta = Cmax - Cmin;
	printf("Cmax = %lf\n", Cmax);
	printf("Cmin = %lf\n", Cmin);
	printf("Delta = %lf\n",Delta);
	
	double H ,H_ = 0,S = 0, L = 0;
	if (Delta == 0) {
		H_ = 0;
	}
	else if (Cmax == _R) {
		H_ = ((_G - _B) / Delta);
	}
	else if (Cmax == _G) {
		H_ = ((_B - _R) / Delta) + 2;
	}
	else if (Cmax == _B) {
		H_ = ((_R - _G) / Delta) + 4;
	}
	H = 60 * H_;
	if (H < 0) H += 360;
	if (H > 360) H -= 360;

	L = (Cmax + Cmin) * 0.5;

	if (L == 1)
		S = 0;
	else
		S = Delta / (1 - fabs( 2 * L - 1 ));

	printf("H = %lf\n", H);
	printf("S = %lf\n", S);
	printf("L = %lf\n", L);
	system("pause");
	return 0;
}