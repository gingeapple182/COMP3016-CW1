#include "SurvivorPool.h"
#include "Player.h"
#include <SDL3/SDL.h>
#include <iostream>

SurvivorPool::SurvivorPool(size_t poolSize) {
	survivors.resize(poolSize);
}

Survivor* SurvivorPool::getSurvivor() {
	for (auto& survivor : survivors) {
		if (!survivor.isActive()) {
			return &survivor;
		}
	}
	return nullptr; // No available survivor
}

void SurvivorPool::updateAll(float deltaTime) {
	for (auto& survivor : survivors) {
		if (survivor.isActive()) {
			survivor.update(deltaTime);
		}
	}
}

void SurvivorPool::renderAll(SDL_Renderer* renderer, float cameraX, float cameraY) {
	for (auto& survivor : survivors) {
		if (survivor.isActive()) {
			survivor.render(renderer, cameraX, cameraY);
		}
	}
}

bool SurvivorPool::checkPlayerCollision(Player& player) {
	bool collisionDetected = false;

	for (auto& survivor : survivors) {

		// Skip inactive survivors
		if (!survivor.isActive()) continue;

		// Axis-aligned bounding box (AABB) collision detection
		if (SDL_HasRectIntersectionFloat(&survivor.getRect(), &player.getRect())) {
			std::cout << "Player rescued a survivor!" << std::endl;
			player.health++;
			// Deactivate survivor
			survivor.deactivate();
			collisionDetected = true;
		}
	}
	return collisionDetected;
}

int SurvivorPool::getActiveCount() const {
	int count = 0;
	for (const auto& survivor : survivors) {
		if (survivor.isActive()) {
			++count;
		}
	}
	return count;
}