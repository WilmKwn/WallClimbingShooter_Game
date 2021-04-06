#pragma once

#include "Sprite.h"

class Background
{
public:
	Background(const wchar_t* filename, Graphics *graphics, std::pair<int,int> window);
	~Background();

	void DrawBackground();

private:
	std::pair<int, int> window;
	
	Sprite *backgroundSprite;
	float bgScaleWidth, bgScaleHeight;
};