#include "MenuScreen.h"

MenuScreen::MenuScreen(Graphics *graphics, HWND wndHandle)
{
	this->graphics = graphics;
	RECT r;
	GetClientRect(wndHandle, &r);
	window.first = r.right;
	window.second = r.bottom;

	char filePath[256];
	GetModuleFileName(NULL, filePath, sizeof(filePath));
	std::string sPath = std::string(filePath);
	sPath = sPath.substr(0, sPath.size() - 21);

	std::string path = sPath + "SmashRipOff\\Sprites\\background0.png";
	std::wstring wsPath = std::wstring(path.begin(), path.end());
	background = new Background(wsPath.c_str(), graphics, window);

	path = sPath + "SmashRipOff\\Sprites\\singleplayer.png";
	wsPath = std::wstring(path.begin(), path.end());
	singleplayerSprite = new Sprite(wsPath.c_str(), graphics, 3.0f, 3.0f, 2, 1);
	singleplayerPos.first = (int)(window.first * 0.5 - (singleplayerSprite->GetSize().first/2));
	singleplayerPos.second = 100;
	singleplayerFrame = 0;
	singleplayer = false;

	path = sPath + "SmashRipOff\\Sprites\\multiplayer.png";
	wsPath = std::wstring(path.begin(), path.end());
	multiplayerSprite = new Sprite(wsPath.c_str(), graphics, 3.0f, 3.0f, 2, 1);
	multiplayerPos.first = (int)(window.first * 0.5 - (multiplayerSprite->GetSize().first / 2));
	multiplayerPos.second = singleplayerPos.second + singleplayerSprite->GetSize().second + 50;
	multiplayerFrame = 0;
	multiplayer = false;

	path = sPath + "SmashRipOff\\Sprites\\practice.png";
	wsPath = std::wstring(path.begin(), path.end());
	practiceSprite = new Sprite(wsPath.c_str(), graphics, 3.0f, 3.0f, 2, 1);
	practicePos.first = (int)(window.first * 0.5 - (practiceSprite->GetSize().first / 2));
	practicePos.second = multiplayerPos.second + multiplayerSprite->GetSize().second + 50;
	practiceFrame = 0;
	practice = false;

	path = sPath + "SmashRipOff\\Sprites\\options.png";
	wsPath = std::wstring(path.begin(), path.end());
	optionsSprite = new Sprite(wsPath.c_str(), graphics, 3.0f, 3.0f, 2, 1);
	optionsPos.first = (int)(window.first * 0.5 - (optionsSprite->GetSize().first / 2));
	optionsPos.second = practicePos.second + practiceSprite->GetSize().second + 50;
	optionsFrame = 0;
	options = false;

	//bgm = new Sound("");
}

MenuScreen::~MenuScreen()
{

}

void MenuScreen::Update()
{
	if (graphics->PointOnRect(Input::GetMousePos().x, Input::GetMousePos().y,
		singleplayerPos.first, singleplayerPos.second, singleplayerPos.first + singleplayerSprite->GetSize().first, singleplayerPos.second + singleplayerSprite->GetSize().second)) {
		if (Input::LeftClick()) {
			singleplayer = true;
		}
		singleplayerFrame = 1;
	}
	else {
		singleplayerFrame = 0;
	}

	if (graphics->PointOnRect(Input::GetMousePos().x, Input::GetMousePos().y,
		multiplayerPos.first, multiplayerPos.second, multiplayerPos.first + multiplayerSprite->GetSize().first, multiplayerPos.second + multiplayerSprite->GetSize().second)) {
		if (Input::LeftClick()) {
			multiplayer = true;
		}
		multiplayerFrame = 1;
	}
	else {
		multiplayerFrame = 0;
	}

	if (graphics->PointOnRect(Input::GetMousePos().x, Input::GetMousePos().y, 
		practicePos.first, practicePos.second, practicePos.first + practiceSprite->GetSize().first, practicePos.second + practiceSprite->GetSize().second)) {
		
		if (Input::LeftClick()) {
			practice = true;
		}
		practiceFrame = 1;
	}
	else {
		practiceFrame = 0;
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

void MenuScreen::Render()
{
	graphics->Begin();
	graphics->ClearScreen(0.6f, 0.0f, 0.6f);

	background->DrawBackground();

	singleplayerSprite->Draw(singleplayerFrame, 0, singleplayerPos.first, singleplayerPos.second, 3.0f, 3.0f);
	multiplayerSprite->Draw(multiplayerFrame, 0, multiplayerPos.first, multiplayerPos.second, 3.0f, 3.0f);
	practiceSprite->Draw(practiceFrame, 0, practicePos.first, practicePos.second, 3.0f, 3.0f);
	optionsSprite->Draw(optionsFrame, 0, optionsPos.first, optionsPos.second, 3.0f, 3.0f);
	
	graphics->End();
}

void MenuScreen::SetSingleplayer(bool singleplayer)
{
	this->singleplayer = singleplayer;
}

bool MenuScreen::GetSingleplayer()
{
	return singleplayer;
}

void MenuScreen::SetMultiplayer(bool multiplayer)
{
	this->multiplayer = multiplayer;
}

bool MenuScreen::GetMultiplayer()
{
	return multiplayer;
}

void MenuScreen::SetPractice(bool practice)
{
	this->practice = practice;
}

bool MenuScreen::GetPractice()
{
	return practice;
}

void MenuScreen::SetOptions(bool options)
{
	this->options = options;
}

bool MenuScreen::GetOptions()
{
	return options;
}