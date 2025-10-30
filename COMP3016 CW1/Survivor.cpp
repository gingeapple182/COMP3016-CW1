#include "Survivor.h"

Survivor::Survivor()
	: x(0), y(0), active(false)
{
	rect = { x, y, 32.0f, 32.0f };
	colour = { 255, 255, 0, 255 }; // Yellow color
}

void Survivor::init(float startX, float startY)
{
	x = startX;
	y = startY;
	active = true;

	rect.x = x - rect.w / 2;
	rect.y = y - rect.h / 2;
}

void Survivor::update(float deltaTime)
{
	// If not active, do nothing
	if (!active) return;

	// Survivors are stationary for now
}

void Survivor::render(SDL_Renderer* renderer, float cameraX, float cameraY)
{
	// Dont render if inactive
	if (!active) return;

	// Move survivor onto the screen
	SDL_FRect screenRect = {
		rect.x - cameraX,
		rect.y - cameraY,
		rect.w,
		rect.h
	};

	// Draw survivor
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(renderer, &screenRect);
}

void Survivor::deactivate()
{
	active = false;
}