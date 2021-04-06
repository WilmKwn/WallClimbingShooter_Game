#include "Multiplayer.h"

Multiplayer::Multiplayer(Graphics *graphics, HWND wndHandle)
{
	this->graphics = graphics;
	RECT r;
	GetClientRect(wndHandle, &r);
	window.first = r.right;
	window.second = r.bottom;

	clicked = false;

	looker = new Client();
	looker->RunConnect();
	looker->RunReceive();
}

Multiplayer::~Multiplayer()
{

}

void Multiplayer::Update()
{
	if (Input::LeftClick() && !clicked) {
		looker->SendMsg("wow");
		clicked = true;
	}
	else if (!Input::LeftClick()) {
		clicked = false;
	}
}

void Multiplayer::Render()
{
	graphics->Begin();

	graphics->ClearScreen(0.5f, 0.0f, 0.5f);
	
	graphics->End();
}

Client *Multiplayer::GetClient()
{
	return looker;
}