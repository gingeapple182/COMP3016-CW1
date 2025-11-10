#include "SurvivorPool.h"
#include "Player.h"
#include "Game.h"
#include <SDL3/SDL.h>
#include <iostream>

SurvivorPool::SurvivorPool(size_t poolSize) {
	survivors.resize(poolSize);
}

// Get an inactive survivor from the pool
Survivor* SurvivorPool::getSurvivor() {
	for (auto& survivor : survivors) {
		if (!survivor.isActive()) {
			return &survivor;
		}
	}
	return nullptr; // No available survivor
}

// Update all active survivors
void SurvivorPool::updateAll(float deltaTime) {
	for (auto& survivor : survivors) {
		if (survivor.isActive()) {
			survivor.update(deltaTime);
		}
	}
}

// Render all active survivors
void SurvivorPool::renderAll(SDL_Renderer* renderer, float cameraX, float cameraY) {
	for (auto& survivor : survivors) {
		if (survivor.isActive()) {
			survivor.render(renderer, cameraX, cameraY);
		}
	}
}

// Handle player rescuing survivors
int SurvivorPool::checkPlayerCollision(Player& player, int healAmount, float sizeIncrease) {
	int rescuedCount = 0;

	for (auto& survivor : survivors) {

		// Skip inactive survivors
		if (!survivor.isActive()) continue;

		// Axis-aligned bounding box (AABB) collision detection
		if (SDL_HasRectIntersectionFloat(&survivor.getRect(), &player.getRect())) {
			std::cout << "Player rescued a survivor!" << std::endl;
			player.health += healAmount;
			player.increaseSize(sizeIncrease); 
			rescuedCount++;
			// Deactivate survivor
			survivor.deactivate();
		}
	}
	return rescuedCount;
}

// Get count of active survivors
int SurvivorPool::getActiveCount() const {
	int count = 0;
	for (const auto& survivor : survivors) {
		if (survivor.isActive()) {
			++count;
		}
	}
	return count;
}