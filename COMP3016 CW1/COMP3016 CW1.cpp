// COMP3016 CW1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL3/SDL.h>
#include "Player.h"
#include "BulletPool.h"
#include "EnemyPool.h"


int main(int argc, char* argv[])
{
	// Initialize SDL
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Create window and renderer
	int windowWidth = 1000;
	int	windowHeight = 750;

	SDL_Window* window = SDL_CreateWindow("COMP3016 CW1", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
	if (!window) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// Create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) {
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	//// Game loop variables
	uint64_t lastTicks = SDL_GetTicksNS();
	int tileSize = 50;
	srand(static_cast<unsigned int>(time(nullptr)));
	int totalEnemies = 10;
	EnemyPool enemyPool(totalEnemies);
	int score = 0;

	// Map dimensions
	const int mapWidth = 2000;
	const int mapHeight = 2000;

	// Camera position
	float cameraX = 0.0f;
	float cameraY = 0.0f;

	// Create player in the center of the map
	const float pw = 50.0f, ph = 50.0f;
	float startX = (mapWidth - pw) / 2.0f;
	float startY = (mapHeight - ph) / 2.0f;
	Player player(startX, startY, pw, ph, 5.0f);


	// Enemy spawning
	// Spawn enemies at random positions away from the player start area
	for (int i = 0; i < totalEnemies; ++i) {
		Enemy* enemy = enemyPool.getEnemy();
		if (enemy) {
			float enemyX, enemyY;
			do {
				enemyX = static_cast<float>(rand() % (mapWidth - 50));
				enemyY = static_cast<float>(rand() % (mapHeight - 50));
			} while (
				enemyX > startX - 100 && enemyX < startX + 100 &&
				enemyY > startY - 100 && enemyY < startY + 100
			);
			enemy->init(enemyX, enemyY, 0.0f, 1);
		}
	}


	// -- Game loop --
	bool moving = true;
	SDL_Event event;

	while (moving) {
		while (SDL_PollEvent(&event)) {
			// Close window event
			if (event.type == SDL_EVENT_QUIT) {
				moving = false;
			}
		}

		// Handle keyboard input
		SDL_PumpEvents();

		// Calculate delta time
		uint64_t now = SDL_GetTicksNS();
		float dt = (now - lastTicks) / 1000.0f;
		lastTicks = now;

		// -- Update game state --
		player.update(mapWidth, mapHeight, dt);
		enemyPool.updateAll(dt); 
		enemyPool.checkPlayerCollision(player);

		// If player dies, so does the game :(
		if (!player.isAlive()) {
			std::cout << "You died! Final Score: " << score << "\n";
			moving = false;
		}

		// bullet collision with enemies
		int killed = enemyPool.handleEnemyDeath(player.getBulletPool());
		if (killed > 0) {
			score += killed;
			std::cout << "Score: " << score << "\n";
		}

		//center camera on player
		SDL_FPoint playerCentre = player.centreWorld();
		cameraX = playerCentre.x - windowWidth / 2.0f;
		cameraY = playerCentre.y - windowHeight / 2.0f;
		if (cameraX < 0) cameraX = 0;
		if (cameraY < 0) cameraY = 0;
		if (cameraX > mapWidth - windowWidth) cameraX = mapWidth - windowWidth;
		if (cameraY > mapHeight - windowHeight) cameraY = mapHeight - windowHeight;

		// Rendering
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green grid lines
		for (int y = 0; y <= mapHeight; y += tileSize) {
			for (int x = 0; x <= mapWidth; x += tileSize) {
				SDL_FRect tileRect = { x - cameraX, y - cameraY, (float)tileSize, (float)tileSize };
				SDL_RenderRect(renderer, &tileRect);
			}
		}
		
		// Render enemies and player
		enemyPool.renderAll(renderer, cameraX, cameraY);
		player.render(renderer, cameraX, cameraY);

		// Draw everything to the screen
		SDL_RenderPresent(renderer);

		SDL_Delay(16); // ~60 FPS
	}


	// Cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	std::cout << "Game closed\n";
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu