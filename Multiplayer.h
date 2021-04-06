#pragma once

#include "Client.h"

#include "Graphics.h"
#include "Sprite.h"
#include "Input.h"

class Multiplayer
{
public:
	Multiplayer(Graphics *graphics, HWND wndHandle);
	~Multiplayer();

	void Update();
	void Render();

	Client *GetClient();

private:
	Graphics *graphics;

	std::pair<int, int> window;

	Client *looker;
	bool clicked;

	Sprite *findSprite;
	std::pair<int, int> findPos;
	bool find;
	int findFrame;
};