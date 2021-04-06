#include "Bullet.h"

Bullet::Bullet(const wchar_t* filename, Graphics *graphics)
{
	sprite = new Sprite(filename, graphics, 1.0f, 1.0f, 1, 1);

	shoot = shot = false;
	shootDirection = 5;

	pos.first = 1000;
	pos.second = 1000;
}

Bullet::~Bullet()
{
}

void Bullet::MoveX(float speed)
{
	pos.first += (int)speed;
}

void Bullet::MoveY(float speed)
{
	pos.second += (int)speed;
}

void Bullet::Goto(int x, int y)
{
	pos.first = x;
	pos.second = y;
}

void Bullet::SetShoot(bool shoot)
{
	this->shoot = shoot;
}

bool Bullet::GetShoot()
{
	return shoot;
}

void Bullet::Shoot(int direction, float speed, std::pair<int,int> window, std::pair<int, int> player, std::pair<int, int> playerSize)
{
	if (!shot) {
		int x = player.first + (playerSize.first / 2) - (sprite->GetSize().first / 2);
		int y = player.second + (playerSize.second / 2) - (sprite->GetSize().second / 2);
		Goto(x,y);

		shootDirection = direction;
		shot = true;
	}

	if (shootDirection == 0) {
		MoveX(-speed);
	}
	else if (shootDirection == 1) {
		MoveY(-speed);
	}
	else if (shootDirection == 2) {
		MoveX(speed);
	}
	else if (shootDirection == 3) {
		MoveY(speed);
	}
	
	if (pos.first + sprite->GetSize().first > window.first || pos.first < 0 || pos.second + sprite->GetSize().second > window.second || pos.second < 0) {
		pos.first = window.first + 10;
		pos.second = window.second + 10;
		shoot = shot = false;
	}
}

void Bullet::Draw()
{
	if (shoot) {
		sprite->Draw(0, 0, pos.first, pos.second, 0.3f, 0.3f);
	}
}

std::pair<int, int> Bullet::GetPos()
{
	return pos;
}

std::pair<int, int> Bullet::GetSize()
{
	return sprite->GetSize();
}