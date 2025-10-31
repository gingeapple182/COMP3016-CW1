#pragma once
#include <vector>
#include "Enemy.h"
#include "BulletPool.h"

class Player; // Forward declaration

class EnemyPool {
public:
	EnemyPool(size_t poolSize);
	Enemy* getEnemy();
	void updateAll(float deltaTime, const SDL_FPoint& playerPosition);
	void renderAll(SDL_Renderer* renderer, float cameraX, float cameraY);
	bool checkPlayerCollision(Player& player);
	int handleEnemyDeath(BulletPool& bulletPool);
	int getActiveCount() const;
	void updateEnemyBullets(float deltaTime);
	void renderEnemyBullets(SDL_Renderer* renderer, float cameraX, float cameraY);
	bool checkEnemyBulletCollision(Player& player);

private:
	std::vector<Enemy> enemies;
	BulletPool enemyBullets{ 200 };
};