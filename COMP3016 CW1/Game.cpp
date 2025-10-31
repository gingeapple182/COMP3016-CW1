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
    player((4000 - 50) / 2.0f, (4000 - 50) / 2.0f, 40.0f, 40.0f, 5.0f),
    score(0),
    round(1),
    running(true),
    cameraX(0.0f),
    cameraY(0.0f),
	state(GameState::START)
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

// Game manager destructor
Game::~Game() = default;

static EnemyType pickEnemyType(int round) {
    if (round < 5)
        return EnemyType::Runner;
    else if (round == 5) {
        int r = rand() % 100;
		if (r < 25) return EnemyType::Shooter;  // 25% chance Shooter
		else return EnemyType::Runner;          // 75% chance Runner
    } else if (round < 10)
        return (rand() % 2 == 0) ? EnemyType::Runner : EnemyType::Shooter;
    else {
        int r = rand() % 100;
		if (r < 25) return EnemyType::Runner;       // 25% chance Runner
		else if (r < 75) return EnemyType::Shooter; // 50% chance Shooter
        else return EnemyType::Shooter; // Will add sniper here once developed
    }
}

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
			EnemyType type = pickEnemyType(round);
            enemy->init(enemyX, enemyY, 15.0f, 2, type);
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
	enemyPool.checkEnemyBulletCollision(player);
    int rescued = survivorPool.checkPlayerCollision(player);

	if (rescued > 0) {
		rescuedSurvivors += rescued;
		std::cout << "Rescued Survivors: " << rescuedSurvivors << " / 5\n";
	}
    int killed = enemyPool.handleEnemyDeath(player.getBulletPool());
    if (killed > 0) {
        score += killed;
        std::cout << "Score: " << score << "\n";
    }
}

// Updates game state
void Game::update(float dt)
{
    const bool* keys = SDL_GetKeyboardState(nullptr);

    switch (state)
    {
    case GameState::START:
        // Press SPACE to go to instructions
        if (keys[SDL_SCANCODE_SPACE])
            state = GameState::INSTRUCTIONS;
	        std::cout << "Game State: INSTRUCTIONS" << std::endl;
        return;

    case GameState::INSTRUCTIONS:
        // Press ENTER to begin game
        if (keys[SDL_SCANCODE_RETURN]) {
            // reset key variables and start first round
            round = 1;
            score = 0;
            rescuedSurvivors = 0;

            enemyPool = EnemyPool(100);
            survivorPool = SurvivorPool(20);

            spawnEnemies();
            spawnSurvivors();
            player.reset();
            state = GameState::PLAY;
			std::cout << "Game State: PLAY" << std::endl;
        }
        return;

    case GameState::PLAY:
        player.update(mapWidth, mapHeight, dt);
        enemyPool.updateAll(dt, player.centreWorld());
        enemyPool.updateEnemyBullets(dt);
        survivorPool.updateAll(dt);

        handleCollisions();

		// Game over if player is dead
        if (!player.isAlive()) {
            state = GameState::GAMEOVER;
			std::cout << "Game State: GAMEOVER" << std::endl;
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

        checkRoundProgression();
        return;

    case GameState::GAMEOVER:
        // press R to return to start
        if (keys[SDL_SCANCODE_R]) {
            state = GameState::START;
			std::cout << "Game State: START" << std::endl;
        }
        return;
    }
}

void Game::render()
{
	// Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    switch (state)
    {
    case GameState::START:
        renderStartScreen();
        break;

    case GameState::INSTRUCTIONS:
        renderInstructionsScreen();
        break;

    case GameState::PLAY:
		// Draw map grid
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (int y = 0; y <= mapHeight; y += tileSize)
            for (int x = 0; x <= mapWidth; x += tileSize) {
                SDL_FRect tileRect = { x - cameraX, y - cameraY, (float)tileSize, (float)tileSize };
                SDL_RenderRect(renderer, &tileRect);
            }
		// Draw game objects
        enemyPool.renderAll(renderer, cameraX, cameraY);
        enemyPool.renderEnemyBullets(renderer, cameraX, cameraY);
        survivorPool.renderAll(renderer, cameraX, cameraY);
        player.render(renderer, cameraX, cameraY);
        DrawHUD(renderer, font, round, score, enemyPool, survivorPool, player, windowWidth, windowHeight);
        break;

    case GameState::GAMEOVER:
        renderGameOverScreen();
        break;
    }

    SDL_RenderPresent(renderer);
}


void Game::startNewRound() {
    std::cout << "\n=== Starting Round " << round << " ===\n";

    // Reset counters and pools
    rescuedSurvivors = 0;
    enemyPool = EnemyPool(100);
    survivorPool = SurvivorPool(20);

    // Spawn enemies (10 + 5 per round)
	int numEnemies = 10 + ((round - 1) * 5);
    const float minSpawnDistance = 400.0f;
    SDL_FPoint playerPos = player.centreWorld();

    for (int i = 0; i < numEnemies; ++i) {
        Enemy* enemy = enemyPool.getEnemy();
        if (!enemy) continue;

		float enemyX, enemyY, dx, dy, distance;
		do {
			enemyX = static_cast<float>(rand() % (mapWidth - 50));
			enemyY = static_cast<float>(rand() % (mapHeight - 50));
			dx = enemyX - playerPos.x;
			dy = enemyY - playerPos.y;
			distance = std::sqrt(dx * dx + dy * dy);
		} while (distance < minSpawnDistance);

		EnemyType type = pickEnemyType(round);
		enemy->init(enemyX, enemyY, 15.0f, 2, type);
    }

    // Spawn survivors (always 5)
    for (int i = 0; i < 5; ++i) {
        Survivor* survivor = survivorPool.getSurvivor();
        if (survivor) {
            float sx = static_cast<float>(rand() % (mapWidth - 50));
            float sy = static_cast<float>(rand() % (mapHeight - 50));
            survivor->init(sx, sy);
        }
    }

    roundInProgress = true;
}

void Game::endRound() {
	std::cout << "=== Round " << round << " Complete! ===\n";
	std::cout << "Rescued Survivors: " << rescuedSurvivors << " / 5\n";
    roundInProgress = false;
	roundEndTime = SDL_GetTicksNS();
}

void Game::checkRoundProgression() {
	if (roundInProgress) {
		// Check if all enemies are defeated
		if (enemyPool.getActiveCount() == 0) {
			endRound();
		}
	}
	else {
		// Check if delay has passed to start new round
		uint64_t currentTime = SDL_GetTicksNS();
		if (currentTime - roundEndTime >= roundDelay) {
			round++;
			startNewRound();
		}
	}
}

void Game::drawText(const char* msg, float x, float y, SDL_Color colour)
{
    if (!font) return;

    SDL_Surface* surface = TTF_RenderText_Solid(font, msg, (int)std::strlen(msg), colour);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FRect dst = { x, y, (float)surface->w, (float)surface->h };
    SDL_RenderTexture(renderer, texture, nullptr, &dst);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

void Game::renderStartScreen()
{
    SDL_Color white = { 255,255,255,255 };
    drawText("TOP-DOWN SURVIVOR", 80, 120, white);
    drawText("Press SPACE to view controls", 80, 180, white);
}

void Game::renderInstructionsScreen()
{
    SDL_Color white = { 255,255,255,255 };
    drawText("CONTROLS:", 80, 80, white);
    drawText("WASD / Arrows: move", 80, 120, white);
    drawText("Mouse aims, SPACE shoots", 80, 150, white);
    drawText("Rescue PURPLE survivors to gain health/size", 80, 190, white);
    drawText("GREEN = runners, BLUE = shooters", 80, 220, white);
    drawText("Press ENTER to begin", 80, 270, white);
}

void Game::renderGameOverScreen()
{
    SDL_Color white = { 255,255,255,255 };
    drawText("GAME OVER", 80, 120, white);

    std::string scoreLine = "Final score: " + std::to_string(score);
    drawText(scoreLine.c_str(), 80, 160, white);

    drawText("Press R to return to start", 80, 200, white);
}
