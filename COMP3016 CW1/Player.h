#pragma once
#include <SDL3/SDL.h>
#include "BulletPool.h"

class Player {
public:
	Player(float x, float y, float w, float h, float speedPerFrame);

	void update(int mapWidth, int mapHeight);

	void render(SDL_Renderer* renderer, float cameraX, float cameraY);

	SDL_FPoint centreWorld() const;

	float x() const { return rect.x; }
	float y() const { return rect.y; }
	float w() const { return rect.w; }
	float h() const { return rect.h; }

private:
	SDL_FRect rect;
	float speed;
	float angle = 0.0f; // Facing right initially
	SDL_Color colour = { 241, 90, 34, 255 };
};