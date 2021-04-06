#pragma once

#include "Input.h"

#include "Sprite.h"
#include "Background.h"

class PauseScreen
{
public:
	PauseScreen(Graphics *graphics, std::pair<int, int> window);
	~PauseScreen();

	void Update();
	void Render();

	void SetResume(bool resume);
	bool GetResume();

	void SetReturn(bool returnToMenu);
	bool GetReturn();

	void SetOptions(bool options);
	bool GetOptions();

private:
	Graphics *graphics;
	std::pair<int, int> window;

	//Background *background;

	Sprite *resumeSprite;
	std::pair<int, int> resumePos;
	int resumeFrame;
	bool resume;

	Sprite *returnSprite;
	std::pair<int, int> returnPos;
	int returnFrame;
	bool returnToMenu;

	Sprite *optionsSprite;
	std::pair<int, int> optionsPos;
	int optionsFrame;
	bool options;
};