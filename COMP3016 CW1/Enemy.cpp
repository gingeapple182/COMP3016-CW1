#include "Enemy.h"
#include <cmath>

Enemy::Enemy()
// Default inactive enemy
	: x(0), y(0), speed(0), active(false), damage(1), type(EnemyType::Runner)
{
	rect = { 0, 0, 32, 32 }; // Enemy size 32x32
	colour = { 0, 0, 255, 255 }; // blue
}

void Enemy::init(float startX, float startY, float enemySpeed, int enemyDamage, EnemyType enemyType)
{
	x = startX;
	y = startY;
	speed = enemySpeed;
	damage = enemyDamage;
	active = true;

	applyTypeAttributes();

	rect.x = x - rect.w / 2;
	rect.y = y - rect.h / 2;
}

void Enemy::update(float deltaTime, const SDL_FPoint& playerPosition)
{
	// If not active, do nothing
	if (!active) return;

	if (type == EnemyType::Runner) {
		float dx = playerPosition.x - x;
		float dy = playerPosition.y - y;
		float distance = std::sqrt(dx * dx + dy * dy);
		if (distance != 0) {
			dx /= distance;
			dy /= distance;
		}

		x += dx * speed * deltaTime;
		y += dy * speed * deltaTime;
		rect.x = x - rect.w / 2;
		rect.y = y - rect.h / 2;
	}
	
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

void Enemy::applyTypeAttributes()
{
	switch (type) {
	case EnemyType::Runner:
		if (speed <= 0) speed = 1.0f;
		if (damage <= 0) damage = 2;
		colour = { 0, 255, 0, 255 }; // green
		break;
	case EnemyType::Shooter:
		if (speed <= 0) speed = 1.0f;
		if (damage <= 0) damage = 1;
		colour = { 255, 0, 0, 255 }; // red
		break;
	case EnemyType::Sniper:
		if (speed <= 0) speed = 1.0f;
		if (damage <= 0) damage = 2;
		colour = { 0, 0, 255, 255 }; // blue
		break;
	}
}