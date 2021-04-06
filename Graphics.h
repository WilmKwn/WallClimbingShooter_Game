#pragma once

#include <d2d1.h>
#include <dwrite.h>

#include <iostream>
#include <ctime>
#include <sstream>

class Graphics
{
public:
	Graphics();
	~Graphics();

	void Init(HWND wndHandle);
	
	void ClearScreen(float r, float g, float b);

	void DrawRect(int x1, int y1, int x2, int y2, float r, float g, float b);
	void DrawTexts(const wchar_t* text, int x, int y, float size, int length, float r, float g, float b);

	bool PointOnRect(int px, int py, int x1, int y1, int x2, int y2);

	void Begin();
	void End();

	ID2D1HwndRenderTarget* GetRenderTarget();

private:
	ID2D1Factory *factory;
	ID2D1HwndRenderTarget *renderTarget;
	ID2D1SolidColorBrush *brush;

	IDWriteFactory *writeFactory;
	IDWriteTextFormat *textFormat;
};