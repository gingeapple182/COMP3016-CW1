#include "Bullet.h"

Bullet::Bullet()
	: x(0), y(0), dx(0), dy(0), speed(0), active(false)
{
	rect = { 0, 0, 8, 8 }; // Bullet size 8x8
}

void Bullet::init(float startX, float startY, float directionX, float directionY, float bulletSpeed)
{
	x = startX;
	y = startY;
	dx = directionX;
	dy = directionY;
	speed = bulletSpeed;
	active = true;
}

void Bullet::update(float deltaTime)
{
	if (!active) return;

	x += dx * speed * deltaTime;
	y += dy * speed * deltaTime;

	rect.x = x - rect.w / 2;
	rect.y = y - rect.h / 2;

	if (x < 0 || x > 2000 || y < 0 || y > 2000) // Assuming map size
	{
		deactivate();
	}
}

void Bullet::render(SDL_Renderer* renderer, float cameraX, float cameraY)
{
	if (!active) return;

	SDL_FRect screenRect = {
		rect.x - cameraX,
		rect.y - cameraY,
		rect.w,
		rect.h
	};

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &screenRect);
}


void Bullet::deactivate()
{
	active = false;
}