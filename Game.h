#pragma once

#include "Client.h"

#include "Sprite.h"
#include "Sound.h"
#include "Input.h"

#include "PauseScreen.h"

#include "Player.h"
#include "Bullet.h"
#include "Background.h"

class Game
{
public:
	Game(Graphics *graphics, HWND wndHandle, bool practice, bool multiplayer);
	~Game();

	void Init(Client *enemyClient);

	void Update();
	void Render();

public:
	void SpriteOnWall(Player *character);

	bool Collide(Player *character, Bullet *bullet);

	bool GetPlayAgain();
	bool GetReturn();

	void DrawCountDown();

public:
	void PlayerMove();
	void PlayerShoot();

	void PressedOnceShoot(int vk_code);

	void PlayerBulletReload();

public:
	void EnemyAIMove();

	void EnemyMultiplayer();

	void EnemyMoveRight();
	void EnemyMoveLeft();
	void EnemyGlide();
	void EnemyJump();
	void EnemyShoot();

	void EnemyBulletsReload();

private:
	Graphics *graphics;

	std::pair<int, int> window;

	char filePath[256];
	std::string sPath;

	Sound *bgm;
	Sound *shoot;

	float gravity;
	float bulletSpeed;
	float reloadVelocity;

	bool keyPressed[4];

	bool gameOver;
	bool practice;
	bool multiplayer;

	int startCounter;
	int timeBetween;

	Sprite *playAgainSprite;
	bool playAgain;
	int playAgainFrame;
	std::pair<int, int> playAgainPos;

	Sprite *returnSprite;
	bool returnToMenu;
	int returnFrame;
	std::pair<int, int> returnPos;

	bool pause;
	PauseScreen *pauseScreen;

private:
	Player *player;

	std::pair<int, int> prevPlayerPos;

	float playerSpeed;
	float playerGlideSpeed;

	int playerCounter;

	Bullet *playerBullets[2];
	int playerNBullets;
	int playerBulletsLeft;
	bool playerReload;

private:
	Background *background;

private:
	Player *enemyAI;

	Client *client;

	float enemySpeed;
	float enemyGlideSpeed;

	int enemyCounter;

	int enemyMoveCounter;
	int enemyMoveIndex;
	int enemyMoveDelay;
	bool enemyChangeMove;

	int enemyAirCounter;
	int enemyAirIndex;
	int enemyAirDelay;
	bool enemyChangeAir;

	Bullet *enemyBullets[3];
	int enemyNBullets;
	int enemyBulletsLeft;
	bool enemyReload;
	
	typedef void (Game::*MoveActions)();
	typedef void (Game::*AirActions)();

	MoveActions moveActions[2];
	int moveActionsLength;
	AirActions airActions[4];
	int airActionsLength;
};