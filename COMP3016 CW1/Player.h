#pragma once
#include <SDL3/SDL.h>
#include "BulletPool.h"

class Player {
public:
	Player(float x, float y, float w, float h, float speedPerFrame);

	void update(int mapWidth, int mapHeight, float deltaTime);

	void render(SDL_Renderer* renderer, float cameraX, float cameraY);

	SDL_FPoint centreWorld() const;

	float x() const { return rect.x; }
	float y() const { return rect.y; }
	float w() const { return rect.w; }
	float h() const { return rect.h; }

	SDL_FPoint facingVector() const;
	float angleRad = 0.0f;

	BulletPool& getBulletPool() { return bulletPool; }
	void updateBullets(float deltaTime, int mapWidth, int mapHeight) {
		bulletPool.updateAll(deltaTime); 
	}
	void renderBullets(SDL_Renderer* renderer, float cameraX, float cameraY) {
		bulletPool.renderAll(renderer, cameraX, cameraY);
	}

private:
	SDL_FRect rect;
	float speed;
	float angle = 0.0f; // Facing right initially
	SDL_Color colour = { 241, 90, 34, 255 };

	// Bullet pool for the player
	BulletPool bulletPool{ 500 }; 
	float shootCooldown = 0.2f; // 0.2 seconds between shots
};