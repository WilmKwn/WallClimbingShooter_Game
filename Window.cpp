#include "Window.h"

Window::Window()
{
	m_handle = NULL;
}

Window::~Window()
{
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MOUSEMOVE:
		Input::SetMousePos(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
		return 0;
	case WM_LBUTTONDOWN:
		Input::SetLeftClick(true);
		return 0;
	case WM_LBUTTONUP:
		Input::SetLeftClick(false);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}

void Window::Init(int width, int height, HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = "game";
	wcex.lpszMenuName = NULL;
	RegisterClassEx(&wcex);

	m_handle = CreateWindowEx(NULL, "game", "crazy game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, width, height, NULL, NULL, hInstance, NULL);
}

HWND Window::GetHandle()
{
	return m_handle;
}