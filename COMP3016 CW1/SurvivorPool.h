#pragma once
#include <vector>
#include "Survivor.h"

class Player; // Forward declaration

class SurvivorPool {
public:
	SurvivorPool(size_t poolSize);
	Survivor* getSurvivor();
	void updateAll(float deltaTime);
	void renderAll(SDL_Renderer* renderer, float cameraX, float cameraY);
	bool checkPlayerCollision(Player& player);
	int getActiveCount() const;

private:
	std::vector<Survivor> survivors;
};