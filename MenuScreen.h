#pragma once

#include "Input.h"

#include "Sprite.h"
#include "Background.h"

#include "Sound.h"

class MenuScreen
{
public:
	MenuScreen(Graphics *graphics, HWND wndHandle);
	~MenuScreen();

	void Update();
	void Render();

	void SetSingleplayer(bool singleplayer);
	bool GetSingleplayer();

	void SetMultiplayer(bool multiplayer);
	bool GetMultiplayer();
	
	void SetPractice(bool practice);
	bool GetPractice();
	
	void SetOptions(bool options);
	bool GetOptions();

private:
	Graphics *graphics;

	std::pair<int, int> window;

	Background *background;
	Sound *bgm;

	Sprite *singleplayerSprite;
	bool singleplayer;
	std::pair<int, int> singleplayerPos;
	int singleplayerFrame;

	Sprite *multiplayerSprite;
	bool multiplayer;
	std::pair<int, int> multiplayerPos;
	int multiplayerFrame;

	Sprite *practiceSprite;
	bool practice;
	std::pair<int, int> practicePos;
	int practiceFrame;

	Sprite *optionsSprite;
	bool options;
	std::pair<int, int> optionsPos;
	int optionsFrame;
};