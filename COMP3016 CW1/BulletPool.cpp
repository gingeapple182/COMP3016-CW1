#include "BulletPool.h"

BulletPool::BulletPool(size_t poolSize)
{
	bullets.resize(poolSize);
}

// Get an inactive bullet from the pool
Bullet* BulletPool::getBullet()
{
	for (auto& bullet : bullets) {
		if (!bullet.isActive()) {
			return &bullet;
		}
	}
	return nullptr; // No available bullet
}

// Update all active bullets
void BulletPool::updateAll(float deltaTime)
{
	for (auto& bullet : bullets) {
		if (bullet.isActive()) {
			bullet.update(deltaTime);
		}
	}
}

// Render all active bullets
void BulletPool::renderAll(SDL_Renderer* renderer, float cameraX, float cameraY)
{
	for (auto& bullet : bullets) {
		if (bullet.isActive()) {
			bullet.render(renderer, cameraX, cameraY);
		}
	}
}