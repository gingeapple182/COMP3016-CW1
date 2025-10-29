#pragma once
#include <SDL3/SDL.h>	

class Enemy {
public:
	Enemy();

	void init(float startX, float startY, float enemySpeed);
	void update(float deltaTime);
	void render(SDL_Renderer* renderer, float cameraX, float cameraY);
	void deactivate();

	bool isActive() const { return active; }

	const SDL_FRect& getRect() const { return rect; }

private:
	float x, y;
	float speed;
	bool active;
	SDL_FRect rect;
};