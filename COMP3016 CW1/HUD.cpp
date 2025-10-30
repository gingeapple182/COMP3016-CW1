#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <iostream>
#include "HUD.h"
#include "EnemyPool.h"
#include "SurvivorPool.h"
#include "Player.h"


void DrawHUD(SDL_Renderer* renderer, TTF_Font* font, int round, int score,
	const EnemyPool& enemyPool, const SurvivorPool& survivorPool, const Player& player,
	int windowWidth, int windowHeight) {
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
	// Survivors remaining -- top left below ENEMIES
	{
		std::string survivorsText = "Survivors remaining: " + std::to_string(survivorPool.getActiveCount());

		SDL_Surface* surface = TTF_RenderText_Solid(font, survivorsText.c_str(), survivorsText.length(), white);
		if (!surface) {
			std::cerr << "TTF_RenderText_Solid Error: " << SDL_GetError() << std::endl;
		}
		else {
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

			SDL_FRect dstRect = { 10.0f, 10.0f + 60.0f, (float)surface->w, (float)surface->h }; // Below ENEMIES
			SDL_RenderTexture(renderer, texture, nullptr, &dstRect);

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
}