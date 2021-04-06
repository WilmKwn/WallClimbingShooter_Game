#include "Input.h"

bool Input::leftClick = false;
POINT Input::mousePos = { 0,0 };

Input::Input()
{
}

Input::~Input()
{
}

void Input::SetLeftClick(bool down)
{
	leftClick = down;
}

bool Input::LeftClick()
{
	return leftClick;
}

bool Input::KeyDown(int vk_code)
{
	return (GetAsyncKeyState(vk_code) & 0x8000);
}

void Input::SetMousePos(int x, int y)
{
	mousePos = { x,y };
}

POINT Input::GetMousePos()
{
	return mousePos;
}