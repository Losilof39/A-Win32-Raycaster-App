#pragma once
#include <chrono>
#include <vector>

void* backbuffer;
BITMAPINFO bitmap;

void Clear(uint32_t color);
void Draw(int32_t x, int32_t y, uint32_t color);
void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color);
void DrawTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color);
void FillTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color, bool frag_lerp);
void DrawRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color);
void FillRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color);
int rgb_to_hex(uint8_t red, uint8_t green, uint8_t blue);

void Clear(uint32_t color) {
	uint32_t* pixel = (uint32_t*)backbuffer;

	for (int i = 0; i < width * height; i++)
	{
		*pixel++ = color;
	}
}

void Draw(int32_t x, int32_t y, uint32_t color) {
	uint32_t* pixel = (uint32_t*)backbuffer;
	
	// clip any pixel outside client window
	if (y > height - 1 || x > width - 1 || x < 0 || y < 0) {
		// do nothing
	}
	else
		pixel = pixel + y * width + x;
		*pixel = color;
}

void DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color) {

	//olcOneLoneCoder: https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x1 - x0; dy = y1 - y0;
	dx1 = abs(dx); dy1 = abs(dy);
	px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x0; y = y0; xe = x1;
		}
		else
		{
			x = x1; y = y1; xe = x0;
		}

		Draw(x, y, color);

		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
				px = px + 2 * dy1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			Draw(x, y, color);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x0; y = y0; ye = y1;
		}
		else
		{
			x = x1; y = y1; ye = y0;
		}

		Draw(x, y, color);

		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			Draw(x, y, color);
		}
	}
}

void DrawTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color) {
	DrawLine(x0, y0, x1, y1, color);
	DrawLine(x0, y0, x2, y2, color);
	DrawLine(x1, y1, x2, y2, color);
}

void BarcycentricCoord(int32_t px, int32_t py, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, float *w1, float *w2, float *w3) {

	float s1 = float(y2 - y0);
	float s2 = float(x2 - x0);
	float s3 = float(y1 - y0);
	float s4 = float(py - y0);

	*w1 = (x0 * s1 + s4 * s2 - px * s1) / (s3 * s2 - (x1 - x0) * s1);
	*w2 = (s4 - *w1 * s3) / s1;
	*w3 = 1 - *w1 - *w2;
}

void FillTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color, bool frag_lerp) {

	//https://www.avrfreaks.net/sites/default/files/triangles.c
	int32_t t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int32_t signx1, signx2, dx1, dy1, dx2, dy2;
	int32_t e1, e2;
	// Sort vertices
	if (y0 > y1) { SWAP(y0, y1); SWAP(x0, x1); }
	if (y0 > y2) { SWAP(y0, y2); SWAP(x0, x2); }
	if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }

	t1x = t2x = x0; y = y0;   // Starting points

	dx1 = (int32_t)(x1 - x0); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int32_t)(y1 - y0);

	dx2 = (int32_t)(x2 - x0); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int32_t)(y2 - y0);

	if (dy1 > dx1) {   // swap values
		SWAP(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		SWAP(dy2, dx2);
		changed2 = true;
	}

	e2 = (int32_t)(dx2 >> 1);
	// Flat top, just process the second half
	if (y0 == y1) goto next;
	e1 = (int32_t)(dx1 >> 1);

	for (int32_t i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;

		if(frag_lerp != true)
			DrawLine(minx, y, maxx, y, color);    // Draw line from min to max points found on the y
		else {
			for (int i = minx; i <= maxx; i++) {
				float w1;
				float w2;
				float w3;
				BarcycentricCoord(i, y, x0, y0, x1, y1, x2, y2, &w1, &w2, &w3);

				Draw(i, y, rgb_to_hex(uint8_t(w1 * 255), uint8_t(w2 * 255), uint8_t(w3 * 255)));
			}
		}

		// Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2) break;

	}
next:
	// Second half
	dx1 = (int32_t)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int32_t)(y2 - y1);
	t1x = x1;

	if (dy1 > dx1) {   // swap values
		SWAP(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int32_t)(dx1 >> 1);

	for (int32_t i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x2) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next4:

		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;

		if (frag_lerp != true)
			DrawLine(minx, y, maxx, y, color);    // Draw line from min to max points found on the y
		else {
			for (int i = minx; i <= maxx; i++) {
				float w1;
				float w2;
				float w3;
				BarcycentricCoord(i, y, x0, y0, x1, y1, x2, y2, &w1, &w2, &w3);

				Draw(i, y, rgb_to_hex(uint8_t(w1 * 255), uint8_t(w2 * 255), uint8_t(w3 * 255)));
			}
		}

		// Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y2) return;
	}

}

void DrawRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color) {
	DrawLine(x, y, x + width, y, color);
	DrawLine(x + width, y, x + width, y + height, color);
	DrawLine(x + width, y + height, x, y + height, color);
	DrawLine(x, y + height, x, y, color);


}

void FillRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color) {

	for (int j = y; j < y + height; j++) {

		for (int i = x; i < x + width; i++) {
		
			Draw(i, j, color);
		
		}
	}
}

int rgb_to_hex(uint8_t red, uint8_t green, uint8_t blue) {
	return ((red & 0xff) << 16) + ((green & 0xff) << 8) + (blue & 0xff);
}