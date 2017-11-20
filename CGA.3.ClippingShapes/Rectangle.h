#pragma once
#include "Color.h"

class Rectangle 
{
	private:
		int topLeftX, topLeftY, height, width;
		color_t color;
	public:
		Rectangle(int topLeftX, int topLeftY, int height, int width);
		Rectangle(int topLeftX, int topLeftY, int height, int width, color_t color);
		int getTopLeftX();
		int getTopLeftY();
		int getHeight();
		int getWidth();
		color_t getColor();

		void setTopLeftX(int);
		void setTopLeftY(int);
		void setHeight(int);
		void setWidth(int);
		void setColor(color_t);
		
		int topRightX();
		int topRightY();
		int botLeftX();
		int botLeftY();
		int botRightX();
		int botRightY();

		bool isInBounds(int x, int y);
};