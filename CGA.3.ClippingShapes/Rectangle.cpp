#include "Rectangle.h"

Rectangle::Rectangle(int topLeftX, int topLeftY, int height, int width, color_t color)
{
	setTopLeftX(topLeftX);
	setTopLeftY(topLeftY);
	setHeight(height);
	setWidth(width);
	setColor(color);
}

void Rectangle::setTopLeftX(int x) { topLeftX = x; }
void Rectangle::setTopLeftY(int y) { topLeftY = y; }
void Rectangle::setHeight(int height) { this->height = height; }
void Rectangle::setWidth(int width) { this->width = width; }
void Rectangle::setColor(color_t color) { this->color = color; }

int Rectangle::getTopLeftX() { return topLeftX; }
int Rectangle::getTopLeftY() { return topLeftY; }
int Rectangle::getHeight() { return height; }
int Rectangle::getWidth() { return width; }
color_t Rectangle::getColor() { return color; }

int Rectangle::topRightX() { return getTopLeftX() + getWidth(); }
int Rectangle::topRightY() { return getTopLeftY(); }
int Rectangle::botLeftX() { return getTopLeftX(); }
int Rectangle::botLeftY() { return getTopLeftY() + getHeight(); }
int Rectangle::botRightX() { return getTopLeftX() + getWidth(); }
int Rectangle::botRightY() { return getTopLeftY() + height; }

bool Rectangle::isInBounds(int x, int y) {
	return x >= getTopLeftX() && x <= topRightX() && y >= getTopLeftY() && y <= botLeftY();
}
