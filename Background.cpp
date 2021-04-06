#include "Background.h"

Background::Background(const wchar_t* filename, Graphics *graphics, std::pair<int,int> window)
{
	this->window = window;
	backgroundSprite = new Sprite(filename, graphics, 8.0f, 8.0f, 1, 1);
	bgScaleWidth = window.first / 100.0f;
	bgScaleHeight = window.second / 75.0f;
}

Background::~Background()
{
}

void Background::DrawBackground()
{
	backgroundSprite->Draw(0, 0, 0, 0, bgScaleWidth, bgScaleHeight);
}