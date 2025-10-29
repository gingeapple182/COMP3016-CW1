#include "EnemyPool.h"
#include "BulletPool.h"
#include <SDL3/SDL.h>
#include <iostream>

EnemyPool::EnemyPool(size_t poolSize)
{
	enemies.resize(poolSize);
}

Enemy* EnemyPool::getEnemy() // Get an inactive enemy from the pool
{
	for (auto& enemy : enemies) {
		if (!enemy.isActive()) {
			return &enemy;
		}
	}
	return nullptr; // No available enemy
}

void EnemyPool::updateAll(float deltaTime)
{
	for (auto& enemy : enemies) {
		if (enemy.isActive()) {
			enemy.update(deltaTime);
		}
	}
}

void EnemyPool::renderAll(SDL_Renderer* renderer, float cameraX, float cameraY)
{
	for (auto& enemy : enemies) {
		if (enemy.isActive()) {
			enemy.render(renderer, cameraX, cameraY);
		}
	}
}

int EnemyPool::handleEnemyDeath(BulletPool& bulletPool)
{
	int destroyedEnemies = 0;

	for (auto& enemy : enemies) {

		// Skip inactive enemies
		if (!enemy.isActive()) continue;

		for (auto& bullet : bulletPool.getBullets()) {

			// Skip inactive bullets
			if (!bullet.isActive()) continue;

			// Axis-aligned bounding box (AABB) collision detection
			if (SDL_HasRectIntersectionFloat(&enemy.getRect(), &bullet.getRect())) {
				std::cout << "Enemy hit!" << std::endl;
				// Destroy both enemy and bullet
				enemy.deactivate();
				bullet.deactivate();
				// Count destroyed enemy
				destroyedEnemies++;
				break;
			}
		}
	}
	// Return the number of destroyed enemies
	return destroyedEnemies;
}