#pragma once

#include "Graphics.h"

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

class Input
{
public:
	Input();
	~Input();

	static bool KeyDown(int vk_code);

	static void SetLeftClick(bool down);
	static bool LeftClick();

	static void SetMousePos(int x, int y);
	static POINT GetMousePos();

private:
	static bool leftClick;

	static POINT mousePos;
};