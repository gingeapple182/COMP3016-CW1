#pragma once
#include <SDL3/SDL.h>

class Bullet {
public:
	Bullet();
	void init(float startX, float startY, float directionX, float directionY, float bulletSpeed);
	void update(float deltaTime);
	void render(SDL_Renderer* renderer, float cameraX, float cameraY);
	void deactivate();

	bool isActive() const { return active; }

private:
	float x, y;
	float dx, dy;
	float speed;
	bool active;
	SDL_FRect rect;
};