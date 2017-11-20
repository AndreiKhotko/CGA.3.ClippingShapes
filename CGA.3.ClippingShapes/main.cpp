//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Point.h"
#include "Rectangle.h"
#include "Ellipse.h"
#include "Color.h"

#define PI 3.14159265358979323846

using namespace std;

//Screen dimension constants
bool isCircleFront;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int MID_X = SCREEN_WIDTH / 2;
int MID_Y = SCREEN_HEIGHT / 2;

std::vector<Point> points;
std::vector<Point> all_points;

int rounding(double value)
{
	return (int)(value + 0.5);
}

void centering(double &x, double &y)
{
	x += MID_X;
	y += MID_Y;
}

void centering(int &x, int &y, Ellipse ellipse)
{
	x += ellipse.getX();
	y += ellipse.getY();
}

void uncentering(double &x, double &y)
{
	x -= MID_X;
	y -= MID_Y;
}

void uncentering(int &x, int &y, Ellipse ellipse)
{
	x -= ellipse.getX();
	y -= ellipse.getY();
}

void matrixMultiply(double src[2], double matrix[][2], double res[2])
{
	res[0] = matrix[0][0] * src[0] + matrix[0][1] * src[1];
	res[1] = matrix[1][0] * src[0] + matrix[1][1] * src[1];
}

void rotating(int &x, int &y, double alpha, Ellipse ellipse)
{
	uncentering(x, y, ellipse);
	double coords[2] =
	{
		x, //x
		y  //y
	};

	double rotate[2][2] =
	{
		{ cos(alpha), sin(alpha) },
		{ -sin(alpha), cos(alpha) }
	};

	double newCoords[2];

	matrixMultiply(coords, rotate, newCoords);

	x = rounding(newCoords[0]);
	y = rounding(newCoords[1]);

	centering(x, y, ellipse);
}

void SetColor(color_t *color, int r, int g, int b)
{
	(*color).R = r;
	(*color).G = g;
	(*color).B = b;
}

void DrawLine(SDL_Renderer *r, int x1, int y1, int x2, int y2, color_t color)
{
	// Set color:
	SDL_SetRenderDrawColor(r, color.R, color.G, color.B, SDL_ALPHA_OPAQUE);

	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = x2 >= x1 ? 1 : -1;
	int sy = y2 >= y1 ? 1 : -1;
	if (dy <= dx) {
		int d = (dy << 1) - dx;
		int d1 = dy << 1;
		int d2 = (dy - dx) << 1;
		SDL_RenderDrawPoint(r, x1, y1);
		for (int x = x1 + sx, y = y1, i = 1; i <= dx; i++, x += sx) {
			if (d > 0) {
				d += d2; y += sy;
			}
			else
				d += d1;
			SDL_RenderDrawPoint(r, x, y);
		}
	}

	else {
		int d = (dx << 1) - dy;
		int d1 = dx << 1;
		int d2 = (dx - dy) << 1;
		SDL_RenderDrawPoint(r, x1, y1);
		for (int x = x1, y = y1 + sy, i = 1; i <= dy; i++, y += sy) {
			if (d > 0) {
				d += d2; x += sx;
			}
			else
				d += d1;
			SDL_RenderDrawPoint(r, x, y);
		}
	}
}

void DrawMainLines(SDL_Renderer *r, color_t color)
{
	DrawLine(r, 0, MID_Y, SCREEN_WIDTH, MID_Y, color);
	DrawLine(r, MID_X, SCREEN_HEIGHT, MID_X, 0, color);
}

void saveVector(std::vector<Point> *dest, std::vector<Point> src)
{
	(*dest).clear();

	for (int i = 0; i < (int)src.size(); i++)
	{
		(*dest).push_back(src[i]);
	}
}

int GetWidth()
{
	int tmp;
	bool ok = false;

	while (!ok)
	{
		cout << "Enter width: ";
		while (!(cin >> tmp)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input.  Try again: ";
		}

		if (tmp <= 0 || tmp > SCREEN_WIDTH)
		{
			cout << "Invalid input. Should be between 0 and " << SCREEN_WIDTH << ". Try again: ";
			continue;
		}

		// Everything is ok
		return tmp;
	}
}

int GetHeight()
{
	int tmp;
	bool ok = false;

	while (!ok)
	{
		cout << "Enter height: ";
		while (!(cin >> tmp)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input.  Try again: ";
		}

		if (tmp <= 0 || tmp > SCREEN_WIDTH)
		{
			cout << "Invalid input. Should be between 0 and " << SCREEN_HEIGHT << ". Try again: ";
			continue;
		}

		// Everything is ok
		return tmp;
	}
}

void GetClippingWindowSize(int &width, int &height)
{
	width = GetWidth();
	height = GetHeight();
}

void DrawRectangle(SDL_Renderer *r, Rectangle rect)
{
	DrawLine(r, rect.getTopLeftX(), rect.getTopLeftY(), rect.topRightX(), rect.topRightY(), rect.getColor());
	DrawLine(r, rect.topRightX(), rect.topRightY(), rect.botRightX(), rect.botRightY(), rect.getColor());
	DrawLine(r, rect.botRightX(), rect.botRightY(), rect.botLeftX(), rect.botLeftY(), rect.getColor());
	DrawLine(r, rect.botLeftX(), rect.botLeftY(), rect.getTopLeftX(), rect.getTopLeftY(), rect.getColor());
}

void DrawClippingWindow(SDL_Renderer *r, Rectangle window)
{
	DrawRectangle(r, window);
}

void DrawEllipse(SDL_Renderer *r, Ellipse ellipse, Rectangle rect, Ellipse circle, bool isCircle)
{
	float t_increment = 0.001f;

	int x_prev = ellipse.getX() + ellipse.getA(), y_prev = ellipse.getY();

	rotating(x_prev, y_prev, ellipse.getAlpha(), ellipse);
	float t_start = t_increment;

	for (float t = t_start; t < 2 * PI; t += t_increment)
	{
		int x_curr = rounding(ellipse.getX() + ellipse.getA() * cos(t));
		int y_curr = rounding(ellipse.getY() + ellipse.getB() * sin(t));

		rotating(x_curr, y_curr, ellipse.getAlpha(), ellipse);

		if (!isCircle)
		{
			if (circle.isInBounds(x_curr, y_curr))
			{
				bool needDraw = true;
				int space_counter = 1, counter_limit = 100;

				while (circle.isInBounds(x_curr, y_curr))
				{
					if (space_counter > counter_limit)
					{
						space_counter = 0;
						needDraw = !needDraw;
					}

					x_prev = x_curr;
					y_prev = y_curr;

					t += t_increment;

					x_curr = rounding(ellipse.getX() + ellipse.getA() * cos(t));
					y_curr = rounding(ellipse.getY() + ellipse.getB() * sin(t));

					rotating(x_curr, y_curr, ellipse.getAlpha(), ellipse);

					if (needDraw)
					{
						DrawLine(r, x_prev, y_prev, x_curr, y_curr, ellipse.getColor());
					}

					space_counter++;
					if (t >= 2 * PI)
						return;
				}
			}
		}
		if (!rect.isInBounds(x_curr, y_curr))
		{
			bool needDraw = true;
			int space_counter = 1, counter_limit = 100;

			while (!rect.isInBounds(x_curr, y_curr))
			{
				if (space_counter > counter_limit)
				{
					space_counter = 0;
					needDraw = !needDraw;
				}

				x_prev = x_curr;
				y_prev = y_curr;

				t += t_increment;

				x_curr = rounding(ellipse.getX() + ellipse.getA() * cos(t));
				y_curr = rounding(ellipse.getY() + ellipse.getB() * sin(t));

				rotating(x_curr, y_curr, ellipse.getAlpha(), ellipse);

				if (needDraw)
				{
					DrawLine(r, x_prev, y_prev, x_curr, y_curr, ellipse.getColor());
				}

				space_counter++;
				if (t >= 2 * PI)
					return;
			}
		}
		else 
		{
			DrawLine(r, x_prev, y_prev, x_curr, y_curr, ellipse.getColor());

			x_prev = x_curr;
			y_prev = y_curr;
		}
	}
}

void DrawAll(SDL_Renderer *r, Rectangle cWindow, Ellipse ellipse, Ellipse circle)
{
	DrawClippingWindow(r, cWindow);

	DrawEllipse(r, ellipse, cWindow, circle, !isCircleFront);
	DrawEllipse(r, circle, cWindow, ellipse, isCircleFront);
	DrawMainLines(r, cWindow.getColor());	
}

void RedrawAll(SDL_Renderer *r, Rectangle clippingWindow, Ellipse ellipse, Ellipse circle)
{
	SDL_SetRenderDrawColor(r, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(r);

	DrawAll(r, clippingWindow, ellipse, circle);

	SDL_RenderPresent(r);
}

int main(int argc, char* args[])
{
	int width, height;
	GetClippingWindowSize(width, height);

	//The window we'll be rendering to
	SDL_Window* window = NULL;

	SDL_Renderer* renderer = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) == 0)
		{
			SDL_bool done = SDL_FALSE;
			isCircleFront = true;

			color_t color;
			SetColor(&color, 255, 255, 255);
			
			// Draw the clipping windoww in the middle 
			int x = rounding((SCREEN_WIDTH - width) / 2);
			int y = rounding((SCREEN_HEIGHT - height) / 2);

			Rectangle clippingWindow(x, y, height, width, color);
			Ellipse ellipse(200, 200, 100, 50, color);
			Ellipse circle(300, 300, 50, 50, color);
			Ellipse* draggedShape = NULL;
			
			DrawAll(renderer, clippingWindow, ellipse, circle);
			
			SDL_RenderPresent(renderer);

			while (!done)
			{
				SDL_Event event;
				
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						done = SDL_TRUE;
					}

					if (event.type == SDL_KEYDOWN)
					{
						switch (event.key.keysym.scancode)
						{
						case SDL_SCANCODE_LEFT:
							// Rotate left and scale down
							ellipse.setAlpha(ellipse.getAlpha() + PI / 12);
							break;

						case SDL_SCANCODE_RIGHT:
							// Rotate right and scale up
							ellipse.setAlpha(ellipse.getAlpha() - PI / 12);
							break;

							// Это доп.
						/* case SDL_SCANCODE_C:
							isCircleFront = !isCircleFront;*/
						}
					}

					if (event.type == SDL_MOUSEBUTTONDOWN)
					{
						int x, y;
						SDL_GetMouseState(&x, &y);

						if (circle.isInBounds(x, y))
							draggedShape = &circle;
						else if (ellipse.isInBounds(x, y))
							draggedShape = &ellipse; 
						else
							draggedShape = NULL;
					}

					if (event.type == SDL_MOUSEBUTTONUP)
					{
						draggedShape = NULL;
					}

					if (event.type == SDL_MOUSEMOTION)
					{
						if (draggedShape != NULL)
						{
							draggedShape->setX(draggedShape->getX() + event.motion.xrel);
							draggedShape->setY(draggedShape->getY() + event.motion.yrel);
						}
					}

				}

				RedrawAll(renderer, clippingWindow, ellipse, circle);
			}
		}
	}

	SDL_DestroyRenderer(renderer);

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}