#include "Game.h"

Game::Game(Graphics *graphics, HWND wndHandle, bool practice, bool multiplayer)
{
	this->graphics = graphics;
	RECT r;
	GetClientRect(wndHandle, &r);
	window.first = r.right;
	window.second = r.bottom;

	srand(time(0));

	this->practice = practice;
	this->multiplayer = multiplayer;

	GetModuleFileName(NULL, filePath, sizeof(filePath));
	sPath = std::string(filePath);
	sPath = sPath.substr(0, sPath.size() - 21);
}

Game::~Game()
{
}

void Game::Init(Client *enemyClient)
{
	gameOver = false;
	reloadVelocity = 0.3f;
	startCounter = 0;
	timeBetween = 50;

	std::string path = sPath + "SmashRipOff\\Sprites\\playAgain.png";
	std::wstring wsPath = std::wstring(path.begin(), path.end());
	playAgainSprite = new Sprite(wsPath.c_str(), graphics, 3.0f, 3.0f, 2, 1);
	playAgain = false;
	playAgainFrame = 0;
	playAgainPos.first = (int)((window.first * 0.5) - (playAgainSprite->GetSize().first / 2));
	playAgainPos.second = (int)(window.second * 0.6);

	path = sPath + "SmashRipOff\\Sprites\\return.png";
	wsPath = std::wstring(path.begin(), path.end());
	returnSprite = new Sprite(wsPath.c_str(), graphics, 3.0f, 3.0f, 2, 1);
	returnToMenu = false;
	returnFrame = 0;
	returnPos.first = (int)((window.first * 0.5) - (returnSprite->GetSize().first / 2));
	returnPos.second = (int)(window.second * 0.8);

	pause = false;
	pauseScreen = new PauseScreen(graphics, window);

	// ENEMY AI INIT
	enemySpeed = 5.0f;
	path = sPath + "SmashRipOff\\Sprites\\enemy.png";
	wsPath = std::wstring(path.begin(), path.end());
	enemyAI = new Player(wsPath.c_str(), graphics, window, true, playerSpeed);
	enemyAI->Goto(window.first / 10, 0);
	enemyAI->SetWall(1);
	enemyGlideSpeed = enemySpeed * 3;

	enemyAI->SetReloadLength((float)enemyAI->GetReloadSize().first);

	enemyCounter = 0;
	
	enemyMoveCounter = 0;
	enemyMoveIndex = 0;
	enemyMoveDelay = 100;
	enemyChangeMove = false;

	enemyAirCounter = 0;
	enemyAirIndex = 0;
	enemyAirDelay = 100;
	enemyChangeAir = false;

	moveActions[0] = &Game::EnemyMoveLeft;
	moveActions[1] = &Game::EnemyMoveRight;
	moveActionsLength = sizeof(moveActions) / sizeof(moveActions[0]);

	airActions[0] = &Game::EnemyGlide;
	airActions[1] = &Game::EnemyJump;
	airActions[2] = &Game::EnemyJump;
	airActions[3] = &Game::EnemyJump;
	airActionsLength = sizeof(airActions) / sizeof(airActions[0]);
	
	// PLAYER INIT
	playerSpeed = enemySpeed;
	path = sPath + "SmashRipOff\\Sprites\\player.png";
	wsPath = std::wstring(path.begin(), path.end());
	player = new Player(wsPath.c_str(), graphics, window, false, playerSpeed);
	player->Goto(window.first*0.9 - player->GetSize().first, window.second - player->GetSize().second);
	player->SetWall(3);
	playerGlideSpeed = playerSpeed * 3;

	prevPlayerPos = player->GetPos();

	player->SetReloadLength((float)player->GetReloadSize().first);

	playerCounter = 0;

	// BULLET INIT
	playerNBullets = sizeof(playerBullets) / sizeof(playerBullets[0]);
	path = sPath + "SmashRipOff\\Sprites\\playerBullet.png";
	wsPath = std::wstring(path.begin(), path.end());
	for (int i = 0; i < playerNBullets; i++) {
		playerBullets[i] = new Bullet(wsPath.c_str(), graphics);
	}
	playerBulletsLeft = playerNBullets;
	playerReload = false;

	enemyNBullets = sizeof(enemyBullets) / sizeof(enemyBullets[0]);
	path = sPath + "SmashRipOff\\Sprites\\enemyBullet.png";
	wsPath = std::wstring(path.begin(), path.end());
	for (int i = 0; i < enemyNBullets; i++) {
		enemyBullets[i] = new Bullet(wsPath.c_str(), graphics);
	}
	enemyBulletsLeft = enemyNBullets;
	enemyReload = false;
	
	// BACKGROUND INIT
	path = sPath + "SmashRipOff\\Sprites\\background2.png";
	wsPath = std::wstring(path.begin(), path.end());
	background = new Background(wsPath.c_str(), graphics, window);

	gravity = 0.5f;
	bulletSpeed = 15.0f;

	// OTHER INIT (KEY_INPUT)
	for (int i = 0; i < sizeof(keyPressed) / sizeof(keyPressed[0]); i++) {
		keyPressed[i] = false;
	}

	// SOUND INIT
	path = sPath + "SmashRipOff\\Audio\\Failure.wav";
	bgm = new Sound(path.c_str(), true);
	bgm->Stop();

	// Enemy Multiplayer
	if (multiplayer) {
		this->client = enemyClient;
	}
}

void Game::PressedOnceShoot(int vk_code)
{
	int n = vk_code - VK_LEFT;
	if (Input::KeyDown(vk_code) && !keyPressed[n]) {
		player->SetDirection(n);
		if (playerBulletsLeft > 0) {
			std::string path = sPath + "SmashRipOff\\Audio\\shoot.wav";
			shoot = new Sound(path.c_str(), false);
			for (int i = 0; i < playerNBullets; i++) {
				if (!playerBullets[i]->GetShoot()) {
					playerBullets[i]->SetShoot(true);
					playerBulletsLeft--;
					player->SetReloadLength(player->GetReloadLength() - (player->GetReloadSize().first / playerNBullets));
					break;
				}
			}
		}
		keyPressed[n] = true;
	}
	else if (!Input::KeyDown(vk_code)) {
		keyPressed[n] = false;
	}
}

void Game::PlayerMove()
{
	if (Input::KeyDown(VK_D)) {
		if (player->GetPos().first + player->GetSize().first < window.first && (player->GetWall() == 1 || player->GetWall() == 3) && !player->GetGlide()) {
			player->MoveX(playerSpeed);
			player->SetRightOrLeft((player->GetWall() == 1) ? 0 : 1);
			playerCounter++;
		}
		else if (player->GetWall() == 0) {
			if (Input::KeyDown(VK_SHIFT)) {
				player->SetGlide(true);
				player->SetRightOrLeft(3);
			}
			else if (!player->GetGlide()) {
				player->SetJump(true);
			}
		}
	}
	else if (Input::KeyDown(VK_A)) {
		if (player->GetPos().first > 0 && (player->GetWall() == 1 || player->GetWall() == 3) && !player->GetGlide()) {
			player->MoveX(-playerSpeed);
			player->SetRightOrLeft((player->GetWall() == 1) ? 1 : 0);
			playerCounter++;
		}
		else if (player->GetWall() == 2) {
			if (Input::KeyDown(VK_SHIFT)) {
				player->SetGlide(true);
				player->SetRightOrLeft(3);
			}
			else if (!player->GetGlide()) {
				player->SetJump(true);
			}
		}
	}
	if (Input::KeyDown(VK_W)) {
		if (player->GetPos().second > 0 && (player->GetWall() == 0 || player->GetWall() == 2) && !player->GetGlide()) {
			player->MoveY(-playerSpeed);
			player->SetRightOrLeft((player->GetWall() == 0) ? 0 : 1);
			playerCounter++;
		}
		else if (player->GetWall() == 3) {
			if (Input::KeyDown(VK_SHIFT)) {
				player->SetGlide(true);
				player->SetRightOrLeft(3);
			}
			else if (!player->GetGlide()) {
				player->SetJump(true);
			}
		}
	}
	else if (Input::KeyDown(VK_S)) {
		if (player->GetPos().second + player->GetSize().second < window.second && (player->GetWall() == 0 || player->GetWall() == 2) && !player->GetGlide()) {
			player->MoveY(playerSpeed);
			player->SetRightOrLeft((player->GetWall() == 0) ? 1 : 0);
			playerCounter++;
		}
		else if (player->GetWall() == 1) {
			if (Input::KeyDown(VK_SHIFT)) {
				player->SetGlide(true);
				player->SetRightOrLeft(3);
			}
			else if (!player->GetGlide()) {
				player->SetJump(true);
			}
		}
	}

	if (!Input::KeyDown(VK_W) && !Input::KeyDown(VK_A) && !Input::KeyDown(VK_S) && !Input::KeyDown(VK_D)) {
		player->SetRightOrLeft(3);
		playerCounter = 0;
	}
}

void Game::SpriteOnWall(Player *character)
{
	float offset = 5.0f;
	if (character->GetPos().first < 2) {
		character->SetWall(0);
		if (character->GetPrevWall() == 1) {
			character->MoveY(offset);
		}
		else if (character->GetPrevWall() == 3) {
			character->MoveY(-offset);
		}
	}
	if (character->GetPos().first + character->GetSize().first > window.first - 2) {
		character->SetWall(2);
		if (character->GetPrevWall() == 1) {
			character->MoveY(offset);
		}
		else if (character->GetPrevWall() == 3) {
			character->MoveY(-offset);
		}
	}
	if (character->GetPos().second < 2) {
		character->SetWall(1);
		if (character->GetPrevWall() == 0) {
			character->MoveX(offset);
		}
		else if (character->GetPrevWall() == 2) {
			character->MoveX(-offset);
		}
	}
	if (character->GetPos().second + character->GetSize().second > window.second - 2) {
		character->SetWall(3);
		if (character->GetPrevWall() == 0) {
			character->MoveX(offset);
		}
		else if (character->GetPrevWall() == 2) {
			character->MoveX(-offset);
		}
	}
	if (character->GetWall() != character->GetPrevWall()) {
		character->CancelJump();
	}
	else {
		character->SetCancelJump(false);
	}
	character->SetPrevWall(character->GetWall());
}

bool Game::Collide(Player *character, Bullet *bullet)
{
	int cx = character->GetPos().first;
	int cy = character->GetPos().second;
	int cWidth = character->GetSize().first;
	int cHeight = character->GetSize().second;
	int bx = bullet->GetPos().first;
	int by = bullet->GetPos().second;
	int bWidth = bullet->GetSize().first;
	int bHeight = bullet->GetSize().second;
	return (bx + bWidth > cx && bx<cx + cWidth && by + bHeight>cy && by < cy + cHeight);
}

void Game::DrawCountDown()
{
	if (startCounter < timeBetween) {
		graphics->DrawTexts(L"3", window.first*0.5 - 40, window.second*0.3, 150.0f, -2, 0.0f, 0.0f, 1.0f);
	}
	else if (startCounter > timeBetween && startCounter < timeBetween * 2) {
		graphics->DrawTexts(L"2", window.first*0.5 - 40, window.second*0.3, 150.0f, -2, 0.0f, 1.0f, 0.0f);
	}
	else if (startCounter > timeBetween * 2 && startCounter < timeBetween * 3) {
		graphics->DrawTexts(L"1", window.first*0.5 - 40, window.second*0.3, 150.0f, -2, 1.0f, 0.0f, 0.0f);
	}
}

bool Game::GetPlayAgain()
{
	return playAgain;
}

bool Game::GetReturn()
{
	return returnToMenu;
}

void Game::PlayerShoot()
{
	if (!Input::KeyDown(VK_RIGHT)) {
		player->SetDirection(4);
	}
	PressedOnceShoot(VK_LEFT);
	PressedOnceShoot(VK_RIGHT);
	PressedOnceShoot(VK_UP);
	PressedOnceShoot(VK_DOWN);
}

void Game::PlayerBulletReload()
{
	if (!playerReload) {
		player->SetReloadLength(0.0f);
		playerReload = true;
	}
	player->ReloadBar(reloadVelocity);
	if (!player->GetReload()) {
		playerBulletsLeft = playerNBullets;
		playerReload = false;
	}
}

void Game::EnemyMoveRight()
{
	enemyAI->SetRightOrLeft(1);
	enemyCounter++;
	switch (enemyAI->GetWall()) {
	case 0:
		enemyAI->MoveY(enemySpeed);
		break;
	case 1:
		enemyAI->MoveX(-enemySpeed);
		break;
	case 2:
		enemyAI->MoveY(-enemySpeed);
		break;
	case 3:
		enemyAI->MoveX(enemySpeed);
		break;
	}
}

void Game::EnemyMoveLeft()
{
	enemyAI->SetRightOrLeft(0);
	enemyCounter++;
	switch (enemyAI->GetWall()) {
	case 0:
		enemyAI->MoveY(-enemySpeed);
		break;
	case 1:
		enemyAI->MoveX(enemySpeed);
		break;
	case 2:
		enemyAI->MoveY(enemySpeed);
		break;
	case 3:
		enemyAI->MoveX(-enemySpeed);
		break;
	}
}

void Game::EnemyGlide()
{
	enemyAI->SetGlide(true);
}

void Game::EnemyJump()
{
	enemyAI->SetJump(true);
}

void Game::EnemyShoot()
{
	if (enemyBulletsLeft > 0) {
		for (int i = 0; i < enemyNBullets; i++) {
			if (!enemyBullets[i]->GetShoot()) {
				enemyBullets[i]->SetShoot(true);
				enemyBulletsLeft--;
				enemyAI->SetReloadLength(enemyAI->GetReloadLength() - (enemyAI->GetReloadSize().first / enemyNBullets));
				break;
			}
		}
	}
}

void Game::EnemyBulletsReload()
{
	if (!enemyReload) {
		enemyAI->SetReloadLength(0.0f);
		enemyReload = true;
	}
	enemyAI->ReloadBar(reloadVelocity);
	if (!enemyAI->GetReload()) {
		enemyBulletsLeft = enemyNBullets;
		enemyReload = false;
	}
}

void Game::EnemyMultiplayer()
{

}

void Game::EnemyAIMove()
{
	SpriteOnWall(enemyAI);
	
	if (!multiplayer) {
		enemyMoveCounter++;
		enemyAirCounter++;
		
		if (!enemyChangeMove) {
			enemyMoveCounter = 0;
			enemyMoveIndex = rand() % moveActionsLength;
			enemyMoveDelay = rand() % 100 + 30;
			enemyChangeMove = true;
		}
		if (enemyMoveCounter > enemyMoveDelay) {
			enemyChangeMove = false;
		}
		if (!enemyAI->GetGlide()) {
			(this->*moveActions[enemyMoveIndex])();
		}

		if (!enemyChangeAir) {
			enemyAirCounter = 0;
			enemyAirIndex = rand() % airActionsLength;
			enemyAirDelay = rand() % 30 + 10;
			enemyChangeAir = true;
		}
		if (enemyAirCounter > enemyAirDelay) {
			enemyChangeAir = false;
		}
		if (!enemyAI->GetGlide()) {
			(this->*airActions[enemyAirIndex])();
		}

		if (enemyBulletsLeft == 0) {
			enemyAI->SetReload(true);
			EnemyBulletsReload();
		}
		else if (enemyBulletsLeft > 0) {
			int n = rand() % 5;
			if (n == 0) {
				enemyAI->SetDirection(rand() % 4);
				EnemyShoot();
			}
		}
	}
	else {
		// Multiplayer Stuff
		std::string mX = "", mY = "";

		std::stringstream temp;
		temp << enemyAI->GetPos().first << " " << enemyAI->GetPos().second;
		std::string msg = temp.str();
		msg = client->GetMsg(msg);

		int index = -1;
		for (int i = 0; i < msg.size(); i++) {
			if (msg[i] == ' ') {
				index = i;
				break;
			}
			mX += msg[i];
		}
		for (int i = index + 1; i < msg.size(); i++) {
			mY += msg[i];
		}
		enemyAI->Goto(std::stoi(mX), std::stoi(mY));
	}

	if (enemyAI->GetJump()) {
		enemyAI->Jump(gravity);
	}
	if (enemyAI->GetGlide()) {
		enemyAI->Glide(enemyGlideSpeed);
	}
	for (int i = 0; i < enemyNBullets; i++) {
		if (enemyBullets[i]->GetShoot()) {
			enemyBullets[i]->Shoot(enemyAI->GetDirection(), bulletSpeed, window, enemyAI->GetPos(), enemyAI->GetSize());
		}
		if (Collide(player, enemyBullets[i])) {
			enemyBullets[i]->Goto(window.first + 10, window.second + 10);
			player->SetHealth(player->GetHealth() - 1);
		}
	}
}

void Game::Update()
{
	std::stringstream temp;

	startCounter++;
	if (!gameOver && startCounter > timeBetween * 3 && !pause) {
		if (Input::KeyDown(VK_ESCAPE)) {
			pause = true;
			pauseScreen->SetResume(false);
			pauseScreen->SetReturn(false);
			pauseScreen->SetOptions(false);
		}
		SpriteOnWall(player);

		PlayerMove();
		
		PlayerShoot();
		if (playerBulletsLeft == 0) {
			player->SetReload(true);
			PlayerBulletReload();
		}

		if (player->GetJump()) {
			player->Jump(gravity);
		}
		if (player->GetGlide()) {
			player->Glide(playerGlideSpeed);
		}

		if (multiplayer) {
			if (player->GetPos().first != prevPlayerPos.first || player->GetPos().second != prevPlayerPos.second) {
				temp << (window.first - player->GetPos().first) << " " << (window.second - player->GetPos().second);
				client->SendMsg(temp.str());
			}
			prevPlayerPos = player->GetPos();
		}
		
		for (int i = 0; i < playerNBullets; i++) {
			if (playerBullets[i]->GetShoot()) {
				playerBullets[i]->Shoot(player->GetDirection(), bulletSpeed, window, player->GetPos(), player->GetSize());
			}
			if (Collide(enemyAI, playerBullets[i])) {
				playerBullets[i]->Goto(window.first + 10, window.second + 10);
				enemyAI->SetHealth(enemyAI->GetHealth() - 1);
			}
		}
		
		if (player->GetHealth() == 0 || enemyAI->GetHealth() == 0) {
			gameOver = true;
		}

		if (!practice) {
			EnemyAIMove();
		}
		else {
			enemyAI->Goto(window.first + 100, window.second + 100);
		}
	}
	else if (gameOver) {
		//bgm->Stop();
		if (graphics->PointOnRect(Input::GetMousePos().x, Input::GetMousePos().y,
			playAgainPos.first, playAgainPos.second, playAgainPos.first + playAgainSprite->GetSize().first, playAgainPos.second + playAgainSprite->GetSize().second)) {

			if (Input::LeftClick()) {
				playAgain = true;
			}
			playAgainFrame = 1;
		}
		else {
			playAgainFrame = 0;
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
	}
}

void Game::Render()
{
	if (!pause) {
		graphics->Begin();
		graphics->ClearScreen(0.0f, 0.3f, 0.0f);

		background->DrawBackground();

		player->Draw(playerCounter);
		for (int i = 0; i < playerNBullets; i++) {
			playerBullets[i]->Draw();
		}
		if (!practice) {
			enemyAI->Draw(enemyCounter);
			for (int i = 0; i < enemyNBullets; i++) {
				enemyBullets[i]->Draw();
			}
		}

		if (gameOver) {
			bool won = player->GetHealth() > 0;
			const wchar_t* text = (won) ? L"YOU WON" : L" YOU LOST";
			graphics->DrawTexts(text, window.first * 0.5 - ((won) ? 120 : 130), window.second * 0.3, 50.0f, 5, (won)?0.0f:1.0f, (won)?1.0f:0.0f, 0.0f);

			playAgainSprite->Draw(playAgainFrame, 0, playAgainPos.first, playAgainPos.second, 3.0f, 3.0f);
			returnSprite->Draw(returnFrame, 0, returnPos.first, returnPos.second, 3.0f, 3.0f);
		}

		DrawCountDown();

		graphics->End();
	}
	else {
		//bgm->Stop();

		pauseScreen->Update();
		pauseScreen->Render();

		if (pauseScreen->GetResume()) {
			//bgm->Play();
			pause = false;
		}
		else if (pauseScreen->GetReturn()) {
			returnToMenu = true;
		}
	}
}