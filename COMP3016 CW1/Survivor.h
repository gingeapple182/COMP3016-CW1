#pragma once
#include <SDL3/SDL.h>

class Survivor {
public:
	Survivor();
	void init(float startX, float startY);
	void update(float deltaTime);
	void render(SDL_Renderer* renderer, float cameraX, float cameraY);
	void deactivate();

	bool isActive() const { return active; }

	const SDL_FRect& getRect() const { return rect; }

private:
	float x, y;
	bool active;
	SDL_FRect rect;
	SDL_Color colour;
};