#pragma once
#include <vector>
#include "Enemy.h"
#include "BulletPool.h"

class EnemyPool {
public:
	EnemyPool(size_t poolSize);
	Enemy* getEnemy();
	void updateAll(float deltaTime);
	void renderAll(SDL_Renderer* renderer, float cameraX, float cameraY);

	int handleEnemyDeath(BulletPool& bulletPool);

private:
	std::vector<Enemy> enemies;
};