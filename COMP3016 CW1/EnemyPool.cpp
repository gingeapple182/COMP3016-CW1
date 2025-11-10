#include "Player.h"
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

// Update all active enemies
void EnemyPool::updateAll(float deltaTime, const SDL_FPoint& playerPosition)
{
	for (auto& enemy : enemies) {
		if (enemy.isActive()) {
			enemy.update(deltaTime, playerPosition, enemyBullets);
		}
	}
}

// Render all active enemies
void EnemyPool::renderAll(SDL_Renderer* renderer, float cameraX, float cameraY)
{
	for (auto& enemy : enemies) {
		if (enemy.isActive()) {
			enemy.render(renderer, cameraX, cameraY);
		}
	}
}

// Handle enemy deaths due to player bullets
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

// Check for collisions between enemies and the player
bool EnemyPool::checkPlayerCollision(Player& player) {
	bool collisionDetected = false;

	for (auto& enemy : enemies) {

		// Skip inactive enemies
		if (!enemy.isActive()) continue;

		// Axis-aligned bounding box (AABB) collision detection
		if (SDL_HasRectIntersectionFloat(&enemy.getRect(), &player.getRect())) {
			std::cout << "Player hit!" << std::endl;
			// Inflict damage to player
			player.takeDamage(enemy.getDamage());
			// Deactivate enemy
			enemy.deactivate();
			collisionDetected = true;
		}
	}
	return collisionDetected;
}

// Get the count of active enemies
int EnemyPool::getActiveCount() const
{
	int count = 0;
	for (const auto& enemy : enemies) {
		if (enemy.isActive()) {
			count++;
		}
	}
	return count;
}

// Enemy bullet management
void EnemyPool::updateEnemyBullets(float deltaTime)
{
	enemyBullets.updateAll(deltaTime);
}

// Render enemy bullets
void EnemyPool::renderEnemyBullets(SDL_Renderer* renderer, float cameraX, float cameraY)
{
	enemyBullets.renderAll(renderer, cameraX, cameraY);
}

// Check for collisions between enemy bullets and the player
bool EnemyPool::checkEnemyBulletCollision(Player& player)
{
	bool hit = false;

	for (auto& bullet : enemyBullets.getBullets()) {

		// Skip inactive bullets
		if (!bullet.isActive()) continue;

		// Axis-aligned bounding box (AABB) collision detection
		if (SDL_HasRectIntersectionFloat(&bullet.getRect(), &player.getRect())) {
			std::cout << "Player hit by enemy bullet!" << std::endl;
			// Inflict damage to player
			player.takeDamage(1);
			// Deactivate bullet
			bullet.deactivate();
			hit = true;
		}
	}
	return hit;
}