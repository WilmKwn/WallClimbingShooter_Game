#include "Player.h"

Player::Player(const wchar_t* filename, Graphics *graphics, std::pair<int,int> window, bool enemyAI, float playerSpeed)
{
	this->graphics = graphics;
	this->window = window;

	this->enemyAI = enemyAI;

	std::wstring fileNameString = filename;
	std::wstring rightName = fileNameString;
	std::wstring leftName = fileNameString;
	rightName.erase(fileNameString.length()-4, 4);
	leftName.erase(fileNameString.length()-4, 4);
	rightName.append(L"Right.png");
	leftName.append(L"Left.png");

	sprite = new Sprite(filename, graphics, 2.0f, 2.0f, 4, 1);
	spriteRight = new Sprite(rightName.c_str(), graphics, 2.0f, 2.0f, 4, 4);
	spriteLeft = new Sprite(leftName.c_str(), graphics, 2.0f, 2.0f, 4, 4);
	arrow = new Sprite(L"C:\\Users\\willi\\source\\repos\\SmashRipOff\\SmashRipOff\\Sprites\\arrows.png", graphics, 2.0f, 2.0f, 4, 1);

	jumpVelocity = 11.0f;

	// 0: left	1: top	2:right 3: bottom
	wall = 3;
	direction = 1;

	this->playerSpeed = playerSpeed;

	glide = false;
	jump = false;
	cancelJump = false;
	right = left = false;

	healthCount = 5;
	health = healthCount;
	healthBarSize.first = (int)(sprite->GetSize().first * 0.5);
	healthBarSize.second = sprite->GetSize().second / 10;

	reloadBarSize.first = healthBarSize.first;
	reloadBarSize.second = (int)(healthBarSize.second * 0.5);
	reloadColor = D2D1::ColorF(0.0f, 1.0f, 0.0f);
}

Player::~Player()
{
}

void Player::SetDirection(int direction)
{
	this->direction = direction;
}

int Player::GetDirection()
{
	return direction;
}

std::pair<int, int> Player::ArrowPos()
{
	std::pair<int, int> arrowPos;
	if (direction == 0) {
		arrowPos.first = pos.first - arrow->GetSize().first - 5;
		arrowPos.second = pos.second + (sprite->GetSize().second / 2) - (arrow->GetSize().second / 2);
	}
	else if (direction == 1) {
		arrowPos.first = pos.first + (sprite->GetSize().first / 2) - (arrow->GetSize().first / 2);
		arrowPos.second = pos.second - arrow->GetSize().second - 5;
	}
	else if (direction == 2) {
		arrowPos.first = pos.first + sprite->GetSize().first + 5;
		arrowPos.second = pos.second + (sprite->GetSize().second / 2) - (arrow->GetSize().second / 2);
	}
	else if (direction == 3) {
		arrowPos.first = pos.first + (sprite->GetSize().first / 2) - (arrow->GetSize().first / 2);;
		arrowPos.second = pos.second + sprite->GetSize().second + 5;
	}
	else {
		arrowPos.first = window.first;
		arrowPos.second = window.second;
	}
	return arrowPos;
}

void Player::SetGlide(bool glide)
{
	this->glide = glide;
}

bool Player::GetGlide()
{
	return glide;
}

void Player::Glide(float speed)
{
	jump = false;
	switch (wall) {
	case 0:
		MoveX(speed);
		if (pos.first + sprite->GetSize().first > window.first - speed) {
			Goto(window.first - sprite->GetSize().first, pos.second);
			glide = false;
		}
		break;
	case 1:
		MoveY(speed);
		if (pos.second + sprite->GetSize().second > window.second - speed) {
			Goto(pos.first, window.second - sprite->GetSize().second);
			glide = false;
		}
		break;
	case 2:
		MoveX(-speed);
		if (pos.first < speed) {
			Goto(0, pos.second);
			glide = false;
		}
		break;
	case 3:
		MoveY(-speed);
		if (pos.second < speed) {
			Goto(pos.first, 0);
			glide = false;
		}
		break;
	default:
		break;
	}
}

void Player::SetWall(int wall)
{
	this->wall = wall;
}

void Player::SetPrevWall(int prevWall)
{
	this->prevWall = prevWall;
}

int Player::GetWall()
{
	return wall;
}

int Player::GetPrevWall()
{
	return prevWall;
}

void Player::SetCancelJump(bool cancelJump)
{
	this->cancelJump = cancelJump;
}

void Player::SetJump(bool jump)
{
	this->jump = jump;
}

bool Player::GetJump()
{
	return jump;
}

void Player::CancelJump()
{
	if (!cancelJump) {
		jump = false;
		jumpVelocity = 11.0f;
		cancelJump = true;
	}
}

void Player::Jump(float gravity)
{
	//glide = false;
	if (wall == 0) {
		MoveX(jumpVelocity);
		jumpVelocity -= gravity;
		if (pos.first < 0) {
			Goto(1, pos.second);
			jumpVelocity = 11.0f;
			SetJump(false);
		}
	}
	if (wall == 1) {
		MoveY(jumpVelocity);
		jumpVelocity -= gravity;
		if (pos.second < 0) {
			Goto(pos.first, 1);
			jumpVelocity = 11.0f;
			SetJump(false);
		}
	}
	if (wall == 2) {
		MoveX(-jumpVelocity);
		jumpVelocity -= gravity;
		if (pos.first + sprite->GetSize().first > window.first) {
			Goto(window.first - sprite->GetSize().first, pos.second);
			jumpVelocity = 11.0f;
			SetJump(false);
		}
	}
	if (wall == 3) {
		MoveY(-jumpVelocity);
		jumpVelocity -= gravity;
		if (pos.second + sprite->GetSize().second > window.second) {
			Goto(pos.first, window.second - sprite->GetSize().second);
			jumpVelocity = 11.0f;
			SetJump(false);
		}
	}
}

void Player::SetHealth(int health)
{
	this->health = health;
}

int Player::GetHealth()
{
	return health;
}

void Player::SetReloadLength(float reloadLength)
{
	this->reloadLength = reloadLength;
}

void Player::SetReload(bool reload)
{
	this->reload = reload;
}

bool Player::GetReload()
{
	return reload;
}

float Player::GetReloadLength()
{
	return reloadLength;
}

void Player::ReloadBar(float velocity)
{
	reloadLength += velocity;
	reloadColor = D2D1::ColorF(1.0f, 1.0f, 1.0f);
	if (reloadLength > reloadBarSize.first) {
		reloadColor = D2D1::ColorF(0.0f, 1.0f, 0.0f);
		reload = false;
	}
}

void Player::SetRightOrLeft(int rightOrLeft)
{
	if (rightOrLeft == 0) {
		left = true;
		right = false;
	}
	else if (rightOrLeft == 1) {
		right = true;
		left = false;
	}
	else {
		right = left = false;
	}
}

bool Player::GetRight()
{
	return right;
}

bool Player::GetLeft()
{
	return left;
}

void Player::MoveX(float speed)
{
	pos.first += (int)speed;
}

void Player::MoveY(float speed)
{
	pos.second += (int)speed;
}

void Player::Goto(int x, int y)
{
	pos.first = x;
	pos.second = y;
}

void Player::DrawPlayerInfo()
{
	// HEALTH BAR
	int x = pos.first + (GetSize().first/2) - (healthBarSize.first/2);
	int y = (wall != 1) ? pos.second - healthBarSize.second - reloadBarSize.second * 2 : pos.second + GetSize().second + reloadBarSize.second * 2;
	graphics->DrawRect(x, y, x + healthBarSize.first, y + healthBarSize.second, 1.0f, 1.0f, 1.0f);

	int offset = 1;
	int segment = (healthBarSize.first - offset*2) / healthCount;
	graphics->DrawRect(x+offset, y+offset, x + (segment*health) + offset, y + healthBarSize.second - offset, 1.0f, 0.0f, 0.0f);

	// RELOAD BAR
	y = y + healthBarSize.second + reloadBarSize.second;
	graphics->DrawRect(x, y, x + reloadLength, y + reloadBarSize.second, reloadColor.r, reloadColor.g, reloadColor.b);
}

void Player::Draw(int counter)
{
	int frame = counter / (25.0f / playerSpeed);
	if (jump) {
		frame = 0;
	}
	if (right) {
		spriteRight->Draw(frame % 4, wall, pos.first, pos.second, 2.0f, 2.0f);
	}
	else if (left) {
		spriteLeft->Draw(frame % 4, wall, pos.first, pos.second, 2.0f, 2.0f);
	}
	else {
		sprite->Draw(wall, 0, pos.first, pos.second, 2.0f, 2.0f);
	}
	arrowPos = ArrowPos();
	if (!enemyAI) {
		arrow->Draw(direction, 0, arrowPos.first, arrowPos.second, 0.5f, 0.5f);
	}

	DrawPlayerInfo();
}

std::pair<int,int> Player::GetPos()
{
	return pos;
}

std::pair<int, int> Player::GetSize()
{
	return sprite->GetSize();
}

std::pair<int, int> Player::GetArrowPos()
{
	return arrowPos;
}

std::pair<int, int> Player::GetReloadSize()
{
	return reloadBarSize;
}