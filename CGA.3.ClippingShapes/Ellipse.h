#pragma once
#include "Color.h"

class Ellipse
{
	private:
		int x, y, a, b;
		double alpha;
		color_t color;

		int Sqr(int value);
	public:
		Ellipse(int x, int y, int a, int b, color_t color, double alpha = 0);
		
		int getX();
		int getY();
		int getA();
		int getB();
		double getAlpha();
		color_t getColor();

		void setX(int);
		void setY(int);
		void setA(int);
		void setB(int);
		void setColor(color_t color);
		void setAlpha(double);

		bool isInBounds(int x, int y);
};
