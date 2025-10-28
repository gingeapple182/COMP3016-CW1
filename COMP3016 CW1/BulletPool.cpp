#include "BulletPool.h"

BulletPool::BulletPool(size_t poolSize)
{
	bullets.resize(poolSize);
}

Bullet* BulletPool::getBullet()
{
	for (auto& bullet : bullets) {
		if (!bullet.isActive()) {
			return &bullet;
		}
	}
	return nullptr; // No available bullet
}

void BulletPool::updateAll(float deltaTime)
{
	for (auto& bullet : bullets) {
		if (bullet.isActive()) {
			bullet.update(deltaTime);
		}
	}
}

void BulletPool::renderAll(SDL_Renderer* renderer, float cameraX, float cameraY)
{
	for (auto& bullet : bullets) {
		if (bullet.isActive()) {
			bullet.render(renderer, cameraX, cameraY);
		}
	}
}
