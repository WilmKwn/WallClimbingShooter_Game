#pragma once

#include "Sprite.h"

class Player
{
public:
	Player(const wchar_t* filename, Graphics *graphics, std::pair<int,int> window, bool enemyAI, float playerSpeed);
	~Player();

	void SetHealth(int health);
	int GetHealth();

	void SetReloadLength(float reloadLength);
	void ReloadBar(float velocity);
	void SetReload(bool reload);
	bool GetReload();
	float GetReloadLength();

	void SetDirection(int direction);
	int GetDirection();
	std::pair<int, int> ArrowPos();

	void SetGlide(bool glide);
	bool GetGlide();
	void Glide(float speed);

	void SetWall(int wall);
	void SetPrevWall(int wall);
	int GetWall();
	int GetPrevWall();

	void SetJump(bool jump);
	bool GetJump();
	void Jump(float gravity);

	void MoveX(float speed);
	void MoveY(float speed);
	void Goto(int x, int y);

	void SetCancelJump(bool cancelJump);
	void CancelJump();

	void SetRightOrLeft(int rightOrLeft);
	bool GetRight();
	bool GetLeft();

	void Draw(int counter);

	void DrawPlayerInfo();

	std::pair<int, int> GetPos();
	std::pair<int, int> GetSize();

	std::pair<int, int> GetArrowPos();

	std::pair<int, int> GetReloadSize();

private:
	Graphics *graphics;
	std::pair<int, int> window;

	Sprite *sprite;
	Sprite *spriteRight, *spriteLeft;

	Sprite *arrow;

	std::pair<int, int> pos;
	std::pair<int, int> arrowPos;

	std::pair<int, int> healthBarSize;
	int healthCount;
	int health;

	std::pair<int, int> reloadBarSize;
	float reloadLength;
	D2D1_COLOR_F reloadColor;

	bool enemyAI;
	
	bool jump;
	bool glide;
	bool cancelJump;
	bool reload;

	bool right;
	bool left;

	float jumpVelocity;

	float playerSpeed;
	int wall;
	int prevWall;
	int direction;
};