#include "Bullet.h"
#include <iostream>

Bullet::Bullet()
// Default inactive bullet
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
	// If not active, do nothing
	if (!active) return;

	// Move the bullet
	x += dx * speed * deltaTime;
	y += dy * speed * deltaTime;
	rect.x = x - rect.w / 2;
	rect.y = y - rect.h / 2;

	// Deactivate when out of bounds -- ensure map dimensions match actual map!!
	if (x < 0 || x > 4000 || y < 0 || y > 4000) {
		std::cout << "Bullet deactivated for leaving bounds at (" << x << ", " << y << ")\n";
		deactivate();
	}

}

void Bullet::render(SDL_Renderer* renderer, float cameraX, float cameraY)
{
	// Dont render if inactive
	if (!active) return;

	// Move bullet onto the screen
	SDL_FRect screenRect = {
		rect.x - cameraX,
		rect.y - cameraY,
		rect.w,
		rect.h
	};

	// Draw bullet
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &screenRect);
}


void Bullet::deactivate()
{
	active = false;
}