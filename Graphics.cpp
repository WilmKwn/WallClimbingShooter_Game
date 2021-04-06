#include "Graphics.h"

Graphics::Graphics()
{
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;

	if (writeFactory) writeFactory->Release();
	if (textFormat) textFormat->Release();
}

Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
	if (brush) brush->Release();
}

void Graphics::Init(HWND wndHandle)
{
	RECT w;
	GetClientRect(wndHandle, &w);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(wndHandle, D2D1::SizeU(w.right, w.bottom)), &renderTarget);

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
}

void Graphics::ClearScreen(float r, float g, float b)
{
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::DrawRect(int x1, int y1, int x2, int y2, float r, float g, float b)
{
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b), &brush);
	renderTarget->FillRectangle(D2D1::RectF(x1, y1, x2, y2), brush);
}

// TODO: NEEDS WORK
void Graphics::DrawTexts(const wchar_t* text, int x, int y, float size, int length, float r, float g, float b)
{
	std::wstring temp = text;
	std::string textString = std::string(temp.begin(), temp.end());
	int n = textString.length();

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(r,g,b), &brush);
	writeFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_MEDIUM, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, L"en-us", &textFormat);
	renderTarget->DrawTextA(text, sizeof(text) + length, textFormat, D2D1::RectF(x, y, x + (size * n), y+ size), brush);
}

bool Graphics::PointOnRect(int px, int py, int x1, int y1, int x2, int y2)
{
	return (px > x1 && px<x2 && py>y1 && py < y2);
}

void Graphics::Begin()
{
	renderTarget->BeginDraw();
}

void Graphics::End()
{
	renderTarget->EndDraw();
}

ID2D1HwndRenderTarget* Graphics::GetRenderTarget()
{
	return renderTarget;
}