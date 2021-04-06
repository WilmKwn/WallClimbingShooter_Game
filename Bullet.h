#pragma once

#include "Sprite.h"

class Bullet
{
public:
	Bullet(const wchar_t* filename, Graphics *graphics);
	~Bullet();

	void MoveX(float speed);
	void MoveY(float speed);
	void Goto(int x, int y);

	void SetShoot(bool shoot);
	bool GetShoot();
	void Shoot(int direction, float speed, std::pair<int,int> window, std::pair<int, int> player, std::pair<int, int> playerSize);

	void Draw();

	std::pair<int, int> GetPos();
	std::pair<int, int> GetSize();

private:
	Sprite *sprite;

	std::pair<int, int> pos;

	int direction;

	bool shoot;
	bool shot;
	int shootDirection;
};