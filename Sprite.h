#pragma once

#include <wincodec.h>
#include "Graphics.h"

class Sprite
{
public:
	Sprite(const wchar_t *filename, Graphics *graphics, float scaleWidth, float scaleHeight, int nFrameWidth, int nFrameHeight);
	~Sprite();

	void Draw(int frameWidth, int frameHeight, int x, int y, float scaleWidth, float scaleHeight);

	std::pair<int, int> GetSize();

private:
	Graphics *graphics;
	ID2D1Bitmap *bmp;

	int nFrameWidth, nFrameHeight;
	float scaleWidth, scaleHeight;
};