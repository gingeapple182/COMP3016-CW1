#include "Enemy.h"

Enemy::Enemy()
// Default inactive enemy
	: x(0), y(0), speed(0), active(false)
{
	rect = { 0, 0, 32, 32 }; // Enemy size 32x32
}

void Enemy::init(float startX, float startY, float enemySpeed)
{
	x = startX;
	y = startY;
	speed = enemySpeed;
	active = true;

	rect.x = x - rect.w / 2;
	rect.y = y - rect.h / 2;
}

void Enemy::update(float deltaTime)
{
	// If not active, do nothing
	if (!active) return;

	// Going to add enemy movement logic here later
}

void Enemy::render(SDL_Renderer* renderer, float cameraX, float cameraY)
{
	// Dont render if inactive
	if (!active) return;

	// Move enemy onto the screen
	SDL_FRect screenRect = {
		rect.x - cameraX,
		rect.y - cameraY,
		rect.w,
		rect.h
	};

	// Draw enemy
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &screenRect);
}

void Enemy::deactivate()
{
	active = false;
}

