#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windowsx.h>

#include "Input.h"

class Window
{
public:
	Window();
	~Window();

	void Init(int width, int height, HINSTANCE hInstance);

	HWND GetHandle();

private:
	HWND m_handle;
};