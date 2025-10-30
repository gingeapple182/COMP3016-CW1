#include "Game.h"
#include "HUD.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>


// Game manager constructor
Game::Game(SDL_Renderer* renderer, TTF_Font* font, int windowWidth, int windowHeight)
    : renderer(renderer),
    font(font),
    windowWidth(windowWidth),
    windowHeight(windowHeight),
    enemyPool(10),
    survivorPool(5),
    player((4000 - 50) / 2.0f, (4000 - 50) / 2.0f, 50.0f, 50.0f, 5.0f),
    score(0),
    round(1),
    running(true),
    cameraX(0.0f),
    cameraY(0.0f)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    spawnEnemies();
    spawnSurvivors();
}

// Game manager destructor
Game::~Game() = default;

// Spawns enemies at random positions on the map
void Game::spawnEnemies() {
    const float minSpawnDistance = 400.0f;
    SDL_FPoint playerPos = player.centreWorld();

    for (int i = 0; i < 10; ++i) {
        Enemy* enemy = enemyPool.getEnemy();
        if (enemy) {
            float enemyX, enemyY, dx, dy, distance;
            do {
                enemyX = static_cast<float>(rand() % (mapWidth - 50));
                enemyY = static_cast<float>(rand() % (mapHeight - 50));
                dx = enemyX - playerPos.x;
                dy = enemyY - playerPos.y;
                distance = std::sqrt(dx * dx + dy * dy);
            } while (distance < minSpawnDistance);

            enemy->init(enemyX, enemyY, 15.0f, 2, EnemyType::Runner);
        }
    }
}

// Spawns survivors at random positions on the map
void Game::spawnSurvivors() {
    const float minSpawnDistance = 400.0f;
    SDL_FPoint playerPos = player.centreWorld();

    for (int i = 0; i < 5; ++i) {
        Survivor* survivor = survivorPool.getSurvivor();
        if (survivor) {
            float survivorX, survivorY, dx, dy, distance;
            do {
                survivorX = static_cast<float>(rand() % (mapWidth - 50));
                survivorY = static_cast<float>(rand() % (mapHeight - 50));
                dx = survivorX - playerPos.x;
                dy = survivorY - playerPos.y;
                distance = std::sqrt(dx * dx + dy * dy);
            } while (distance < minSpawnDistance);

            survivor->init(survivorX, survivorY);
        }
    }
}

// Handles collisions between player, enemies, and survivors
void Game::handleCollisions() {
    enemyPool.checkPlayerCollision(player);
    survivorPool.checkPlayerCollision(player);

    int killed = enemyPool.handleEnemyDeath(player.getBulletPool());
    if (killed > 0) {
        score += killed;
        std::cout << "Score: " << score << "\n";
    }
}

// Updates game state
void Game::update(float dt) {
    player.update(mapWidth, mapHeight, dt);
    enemyPool.updateAll(dt, player.centreWorld());
    survivorPool.updateAll(dt);

    handleCollisions();

	// Game over if player is dead
    if (!player.isAlive()) {
        std::cout << "You died! Final Score: " << score << "\n";
        running = false;
        return;
    }

    // Update camera
    SDL_FPoint playerCentre = player.centreWorld();
    cameraX = playerCentre.x - windowWidth / 2.0f;
    cameraY = playerCentre.y - windowHeight / 2.0f;
    if (cameraX < 0) cameraX = 0;
    if (cameraY < 0) cameraY = 0;
    if (cameraX > mapWidth - windowWidth) cameraX = mapWidth - windowWidth;
    if (cameraY > mapHeight - windowHeight) cameraY = mapHeight - windowHeight;
}

void Game::render() {
	// Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
	// Draw map grid
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int y = 0; y <= mapHeight; y += tileSize) {
        for (int x = 0; x <= mapWidth; x += tileSize) {
            SDL_FRect tileRect = { x - cameraX, y - cameraY, (float)tileSize, (float)tileSize };
            SDL_RenderRect(renderer, &tileRect);
        }
    }
	// Draw game objects
    enemyPool.renderAll(renderer, cameraX, cameraY);
    survivorPool.renderAll(renderer, cameraX, cameraY);
    player.render(renderer, cameraX, cameraY);

    DrawHUD(renderer, font, round, score, enemyPool, player, windowWidth, windowHeight);

    SDL_RenderPresent(renderer);
}
