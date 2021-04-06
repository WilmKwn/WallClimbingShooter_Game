#include "PauseScreen.h"

PauseScreen::PauseScreen(Graphics *graphics, std::pair<int,int> window)
{
	this->graphics = graphics;
	this->window = window;

	int offset = 20;

	char filePath[256];
	GetModuleFileName(NULL, filePath, sizeof(filePath));
	std::string sPath = std::string(filePath);
	sPath = sPath.substr(0, sPath.size() - 21);

	std::string path = sPath + "SmashRipOff\\Sprites\\resume.png";
	std::wstring wsPath = std::wstring(path.begin(), path.end());
	resumeSprite = new Sprite(wsPath.c_str(), graphics, 3.0f, 3.0f, 2, 1);
	resumePos.first = (int)(window.first * 0.5 - (resumeSprite->GetSize().first / 2));
	resumePos.second = (int)(window.second * 0.2) + offset;
	resumeFrame = 0;
	resume = false;

	path = sPath + "SmashRipOff\\Sprites\\return.png";
	wsPath = std::wstring(path.begin(), path.end());
	returnSprite = new Sprite(wsPath.c_str(), graphics, 3.0f, 3.0f, 2, 1);
	returnPos.first = (int)(window.first * 0.5 - (returnSprite->GetSize().first / 2));
	returnPos.second = resumePos.second + resumeSprite->GetSize().second + offset;
	returnFrame = 0;
	returnToMenu = false;

	path = sPath + "SmashRipOff\\Sprites\\options.png";
	wsPath = std::wstring(path.begin(), path.end());
	optionsSprite = new Sprite(wsPath.c_str(), graphics, 3.0f, 3.0f, 2, 1);
	optionsPos.first = (int)(window.first * 0.5 - (optionsSprite->GetSize().first / 2));
	optionsPos.second = returnPos.second + returnSprite->GetSize().second + offset;
	optionsFrame = 0;
	options = false;
}

PauseScreen::~PauseScreen()
{
}

void PauseScreen::Update()
{
	if (graphics->PointOnRect(Input::GetMousePos().x, Input::GetMousePos().y,
		resumePos.first, resumePos.second, resumePos.first + resumeSprite->GetSize().first, resumePos.second + resumeSprite->GetSize().second)) {

		if (Input::LeftClick()) {
			resume = true;
		}
		resumeFrame = 1;
	}
	else {
		resumeFrame = 0;
	}

	if (graphics->PointOnRect(Input::GetMousePos().x, Input::GetMousePos().y,
		returnPos.first, returnPos.second, returnPos.first + returnSprite->GetSize().first, returnPos.second + returnSprite->GetSize().second)) {

		if (Input::LeftClick()) {
			returnToMenu = true;
		}
		returnFrame = 1;
	}
	else {
		returnFrame = 0;
	}

	if (graphics->PointOnRect(Input::GetMousePos().x, Input::GetMousePos().y,
		optionsPos.first, optionsPos.second, optionsPos.first + optionsSprite->GetSize().first, optionsPos.second + optionsSprite->GetSize().second)) {

		if (Input::LeftClick()) {
			options = true;
		}
		optionsFrame = 1;
	}
	else {
		optionsFrame = 0;
	}
}

void PauseScreen::Render()
{
	graphics->Begin();
	graphics->ClearScreen(0.5f, 0.5f, 0.5f);

	resumeSprite->Draw(resumeFrame, 0, resumePos.first, resumePos.second, 3.0f, 3.0f);
	returnSprite->Draw(returnFrame, 0, returnPos.first, returnPos.second, 3.0f, 3.0f);
	optionsSprite->Draw(optionsFrame, 0, optionsPos.first, optionsPos.second, 3.0f, 3.0f);

	graphics->End();
}

void PauseScreen::SetResume(bool resume)
{
	this->resume = resume;
}

bool PauseScreen::GetResume()
{
	return resume;
}

void PauseScreen::SetReturn(bool returnToMenu)
{
	this->returnToMenu = returnToMenu;
}

bool PauseScreen::GetReturn()
{
	return returnToMenu;
}

void PauseScreen::SetOptions(bool options)
{
	this->options = options;
}

bool PauseScreen::GetOptions()
{
	return options;
}