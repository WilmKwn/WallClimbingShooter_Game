#include "Window.h"
#include "Graphics.h"

#include "Game.h"
#include "MenuScreen.h"
#include "Multiplayer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow)
{
	Window *window = new Window();
	window->Init(800, 600, hInstance);

	Graphics *graphics = new Graphics();
	graphics->Init(window->GetHandle());

	MenuScreen *menuScreen = new MenuScreen(graphics, window->GetHandle());

	Multiplayer *multiplayer = NULL;
	bool newMultiplayer = false;

	Game *game = NULL;
	bool newGame = false;

	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (menuScreen->GetSingleplayer()) {
			if (!newGame) {
				game = new Game(graphics, window->GetHandle(), false, false);
				game->Init(NULL);
				newGame = true;
			}
			game->Update();
			game->Render();

			if (game->GetPlayAgain()) {
				newGame = false;
			}
			else if (game->GetReturn()) {
				newGame = false;
				menuScreen->SetSingleplayer(false);
			}
		}
		else if (menuScreen->GetMultiplayer()) {
			if (!newMultiplayer) {
				multiplayer = new Multiplayer(graphics, window->GetHandle());
				newMultiplayer = true;
			}
			//multiplayer->Update();
			//multiplayer->Render();
			if (!newGame) {
				game = new Game(graphics, window->GetHandle(), false, true);
				game->Init(multiplayer->GetClient());
				newGame = true;
			}
			game->Update();
			game->Render();

			if (game->GetPlayAgain()) {
				newGame = false;
			}
			else if (game->GetReturn()) {
				newGame = false;
				menuScreen->SetMultiplayer(false);
			}
		}
		else if (menuScreen->GetPractice()) {
			if (!newGame) {
				game = new Game(graphics, window->GetHandle(), true, false);
				game->Init(NULL);
				newGame = true;
			}
			game->Update();
			game->Render();

			if (game->GetReturn()) {
				newGame = false;
				menuScreen->SetPractice(false);
			}
		}
		else if (menuScreen->GetOptions()) {
			// STUFF
		}
		else {
			menuScreen->Update();
			menuScreen->Render();
		}
	}
	return msg.wParam;
}