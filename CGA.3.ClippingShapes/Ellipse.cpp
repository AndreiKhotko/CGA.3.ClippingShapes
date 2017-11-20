#include "Ellipse.h"
#include <cmath>

Ellipse::Ellipse(int x, int y, int a, int b, color_t color, double alpha)
{
	setX(x);
	setY(y);
	setA(a);
	setB(b);
	setColor(color);
	setAlpha(alpha);
}

void Ellipse::setX(int x) { this->x = x; }
void Ellipse::setY(int y) { this->y = y; }
void Ellipse::setA(int a) { this->a = a; }
void Ellipse::setB(int b) { this->b = b; }
void Ellipse::setColor(color_t color) { this->color = color; }
void Ellipse::setAlpha(double alpha) { this->alpha = alpha; }

int Ellipse::getX() { return x; }
int Ellipse::getY() { return y; }
int Ellipse::getA() { return a; }
int Ellipse::getB() { return b; }
color_t Ellipse::getColor() { return color; }
double Ellipse::getAlpha() { return alpha; }

int Ellipse::Sqr(int value) { return value * value; }

bool Ellipse::isInBounds(int x, int y) 
{
	float expr = 1.0f * Sqr(x - getX()) / Sqr(a) + 1.0f * Sqr(y - getY()) / Sqr(b);

	return expr < 1;
}