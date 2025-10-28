#pragma once
#include <vector>
#include "Bullet.h"

class BulletPool {
public:
	BulletPool(size_t poolSize);
	Bullet* getBullet();
	void updateAll(float deltaTime);
	void renderAll(SDL_Renderer* renderer, float cameraX, float cameraY);

private:
	std::vector<Bullet> bullets;
};