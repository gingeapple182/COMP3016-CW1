// COMP3016 CW1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
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

	// Initialize TTF
	if (!TTF_Init()) {
		std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	TTF_Font* font = TTF_OpenFont("Pixellari.ttf", 24); // Load a font file
	if (!font) {
		std::cerr << "TTF_OpenFont Error: " << SDL_GetError() << std::endl;
		// handle error …
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
	// Calculate delta time
	uint64_t now = SDL_GetTicksNS();
	float dt = float(now) / 1000000000.0f; // 9 '0's
	uint64_t lastTicks = now;
	// Tile size for grid
	int tileSize = 50;
	// Seed random number generator
	srand(static_cast<unsigned int>(time(nullptr)));
	int totalEnemies = 10;
	EnemyPool enemyPool(totalEnemies);
	int round = 1;
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
	const float minSpawnDistance = 400.0f;  // Safe distance away from player

	for (int i = 0; i < totalEnemies; ++i) {
		Enemy* enemy = enemyPool.getEnemy();
		if (enemy) {
			float enemyX, enemyY, dx, dy, distance;
			do {
				enemyX = static_cast<float>(rand() % (mapWidth - 50));
				enemyY = static_cast<float>(rand() % (mapHeight - 50));
				dx = enemyX - startX;
				dy = enemyY - startY;
				distance = std::sqrt(dx * dx + dy * dy);
			} while (distance < minSpawnDistance); // Keep trying until far enough

			enemy->init(enemyX, enemyY, 15.0f, 2, EnemyType::Runner);
		}
	}



	//// -- Game loop --
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
		float dt = float(now) / 100000000000.0f; // 10 '0's
		uint64_t lastTicks = now;

		// -- Update game state --
		player.update(mapWidth, mapHeight, dt);
		enemyPool.updateAll(dt, player.centreWorld()); 
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

		//// -- HUD --
		SDL_Color white = { 255, 255, 255, 255 };

		// ROUND -- top left
		{
			std::string roundText = "ROUND: " + std::to_string(round);

			// Create the actual text surface
			SDL_Surface* surface = TTF_RenderText_Solid(font, roundText.c_str(), roundText.length(), white);
			if (!surface) {
				std::cerr << "TTF_RenderText_Solid Error: " << SDL_GetError() << std::endl;
			}
			else {
				// Create texture from surface
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

				// Render the texture to the screen
				SDL_FRect dstRect = { 10.0f, 10.0f, (float)surface->w, (float)surface->h }; // Top-left corner
				SDL_RenderTexture(renderer, texture, nullptr, &dstRect); // Show on screen

				SDL_DestroyTexture(texture);
				SDL_DestroySurface(surface);
			}
		}
		// Enemies remaining -- top left below ROUND
		{
			std::string enemiesText = "Enemies remaining: " + std::to_string(enemyPool.getActiveCount());

			// Create the actual text surface
			SDL_Surface* surface = TTF_RenderText_Solid(font, enemiesText.c_str(), enemiesText.length(), white);
			if (!surface) {
				std::cerr << "TTF_RenderText_Solid Error: " << SDL_GetError() << std::endl;
			}
			else {
				// Create texture from surface
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

				// Render the texture to the screen
				SDL_FRect dstRect = { 10.0f, 10.0f + 30.0f, (float)surface->w, (float)surface->h }; // Below ROUND
				SDL_RenderTexture(renderer, texture, nullptr, &dstRect); // Show on screen

				SDL_DestroyTexture(texture);
				SDL_DestroySurface(surface);
			}
		}

		// SCORE -- top right
		{
			std::string scoreText = "SCORE: " + std::to_string(score);

			// Create the actual text surface
			SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), scoreText.length(), white);
			if (!surface) {
				std::cerr << "TTF_RenderText_Solid Error: " << SDL_GetError() << std::endl;
			}
			else {
				// Create texture from surface
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

				// Render the texture to the screen
				SDL_FRect dstRect = { (float)(windowWidth - surface->w - 10), 10.0f, (float)surface->w, (float)surface->h }; // Top-right corner
				SDL_RenderTexture(renderer, texture, nullptr, &dstRect); // Show on screen

				SDL_DestroyTexture(texture);
				SDL_DestroySurface(surface);
			}
		}

		// HEALTH -- bottom left
		{
			std::string healthText = "HEALTH: " + std::to_string(player.getHealth());

			// Create the actual text surface
			SDL_Surface* surface = TTF_RenderText_Solid(font, healthText.c_str(), healthText.length(), white);
			if (!surface) {
				std::cerr << "TTF_RenderText_Solid Error: " << SDL_GetError() << std::endl;
			}
			else {
				// Create texture from surface
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

				// Render the texture to the screen
				SDL_FRect dstRect = { 10.0f, (float)(windowHeight - surface->h - 10), (float)surface->w, (float)surface->h }; // Bottom-left corner
				SDL_RenderTexture(renderer, texture, nullptr, &dstRect); // Show on screen

				SDL_DestroyTexture(texture);
				SDL_DestroySurface(surface);
			}
		}

		// Draw everything to the screen
		SDL_RenderPresent(renderer);

		SDL_Delay(16); // ~60 FPS
	}


	//// Cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit(); 
	SDL_Quit();

	std::cout << "Game closed\n";
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu