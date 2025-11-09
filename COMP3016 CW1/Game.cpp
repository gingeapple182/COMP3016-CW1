#include "Game.h"
#include "HUD.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>

std::vector<HighScores> highScores;
const int MAX_HIGH_SCORES = 5;

// Game manager constructor
Game::Game(SDL_Renderer* renderer, TTF_Font* font, int windowWidth, int windowHeight)
    : renderer(renderer),
    font(font),
    windowWidth(windowWidth),
    windowHeight(windowHeight),
    enemyPool(10),
    survivorPool(5),
    player(0, 0, 40.0f, 40.0f, 5.0f),
    score(0),
    round(1),
    running(true),
    cameraX(0.0f),
    cameraY(0.0f),
	state(GameState::START)
{
    srand(static_cast<unsigned int>(time(nullptr)));

	loadConfig();
    loadScores();

	player.setWorldBounds(mapWidth, mapHeight);
	float startX = (mapWidth - playerStartWidth) / 2.0f;
	float startY = (mapHeight - playerStartHeight) / 2.0f;
    player.applyConfig(startX, startY, playerStartWidth, playerStartHeight, playerStartSpeed, playerStartHealth);
}

// Game manager destructor
Game::~Game() = default;

// Loading config from file
void Game::loadConfig() {
	std::ifstream configFile("config.txt");
	if (!configFile.is_open()) {
		std::cerr << "Failed to open config.txt\n";
		return;
	}

	std::cout << "Loading config from config.txt\n";

    std::string line, section;
    while (std::getline(configFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        if (line.front() == '[' && line.back() == ']') {
            section = line.substr(1, line.size() - 2);
            std::cout << "[CONFIG] Section: " << section << "\n";
            continue;
        }

        if (line.rfind('--', 0) == 0) {
			std::cout << "[CONFIG] Stop marker found, ending parse.\n";
            break;
        }

        std::istringstream iss(line);
        std::string key, value;

        // Parse cofig settings
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
			// Parse PLAYER section
            if (section == "PLAYER") {
                std::cout << "Key = " << key << ", Value=" << value << "\n";
                if (key == "start_health")
                    playerStartHealth = std::stoi(value);
                else if (key == "start_width")
                    playerStartWidth = std::stof(value);
                else if (key == "start_height")
                    playerStartHeight = std::stof(value);
                else if (key == "start_speed")
                    playerStartSpeed = std::stof(value);
            
		    // Parse ENEMY section
            }
            else if (section == "ENEMY") {
                std::cout << "Key = " << key << ", Value=" << value << "\n";
                if (key == "runner_speed")
                    enemyRunnerSpeed = std::stof(value);
                else if (key == "runner_damage")
                    enemyRunnerDamage = std::stoi(value);
                else if (key == "shooter_speed")
                    enemyShooterSpeed = std::stof(value);
                else if (key == "shooter_damage")
                    enemyShooterDamage = std::stoi(value);
                // Parse WORLD section
            }
			else if (section == "WORLD") {
				std::cout << "Key = " << key << ", Value=" << value << "\n";
				if (key == "map_width")
					mapWidth = std::stoi(value);
				else if (key == "map_height")
					mapHeight = std::stoi(value);
				else if (key == "tile_size")
					tileSize = std::stoi(value);
				else if (key == "survivor_spawn_count")
					survivorSpawnCount = std::stoi(value);
				else if (key == "survivor_heal_amount")
					survivorHealAmount = std::stoi(value);
				else if (key == "survivor_size_increase")
					survivorSizeIncrease = std::stof(value);
				else if (key == "round_enemy_increase")
					roundEnemyIncrease = std::stoi(value);
			}
        }
    }

    std::cout << "[CONFIG] Final values:"
        << " Health=" << playerStartHealth
        << " Width=" << playerStartWidth
        << " Height=" << playerStartHeight
        << " Speed=" << playerStartSpeed << "\n";

	std::cout << "[CONFIG] Enemy values:"
		<< " Runner Speed=" << enemyRunnerSpeed
		<< " Runner Damage=" << enemyRunnerDamage
		<< " Shooter Speed=" << enemyShooterSpeed
		<< " Shooter Damage=" << enemyShooterDamage << "\n";

	std::cout << "[CONFIG] World values:"
		<< " Map Width=" << mapWidth
		<< " Map Height=" << mapHeight
		<< " Tile Size=" << tileSize
		<< " Survivor Spawn Count=" << survivorSpawnCount
		<< " Survivor Heal Amount=" << survivorHealAmount
		<< " Survivor Size Increase=" << survivorSizeIncrease
		<< " Round Enemy Increase=" << roundEnemyIncrease << "\n";
}

// Loading high scores from file
void Game::loadScores() {
    std::ifstream scoreFile("scores.txt");
    if (!scoreFile.is_open()) {
        std::cerr << "Failed to open scores.txt\n";
        return;
    }

    std::cout << "Loading high scores from scores.txt\n";

	highScores.clear();
	HighScores scoreEntry;
	while (scoreFile >> scoreEntry.name >> scoreEntry.round >> scoreEntry.kills >> scoreEntry.survivors >> scoreEntry.score) {
		highScores.push_back(scoreEntry);
		if (highScores.size() >= MAX_HIGH_SCORES) {
			break;
		}
	}

	scoreFile.close();
	std::cout << "Loaded " << highScores.size() << " high scores.\n";
	
}

// Adding a new score entry
void Game::addScores(const std::string& name, int round, int kills, int survivors, int score) {
	// Create new score entry
    HighScores newScoreEntry{ name, round, kills, survivors, score };
	// Add to high scores list
    highScores.push_back(newScoreEntry);
	// Sort high scores in descending order
	std::sort(highScores.begin(), highScores.end(), [](const HighScores& a, const HighScores& b) {
		return a.score > b.score;
	});
	// Trim to max high scores
	if (highScores.size() > MAX_HIGH_SCORES) {
		highScores.resize(MAX_HIGH_SCORES);
	}
	// Save updated scores to file
    saveScore();
}

// Saving high scores to file
void Game::saveScore() {
	// Open file for writing
	std::ofstream scoreFile("scores.txt", std::ios::trunc);
	// Error check
	if (!scoreFile.is_open()) {
		std::cerr << "Failed to open scores.txt for writing\n";
		return;
	}

	// Write each high score entry
	for (const auto& scoreEntry : highScores) {
		scoreFile << scoreEntry.name << " "
			<< scoreEntry.round << " "
			<< scoreEntry.kills << " "
			<< scoreEntry.survivors << " "
			<< scoreEntry.score << "\n";
	}

	scoreFile.close();
	std::cout << "High scores saved to scores.txt\n";
}

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
			if (type == EnemyType::Runner) {
				enemy->init(enemyX, enemyY, enemyRunnerSpeed, enemyRunnerDamage, type); 
			}
			else if (type == EnemyType::Shooter) {
				enemy->init(enemyX, enemyY, enemyShooterSpeed, enemyShooterDamage, type);
            }
            else {
                enemy->init(enemyX, enemyY, enemyShooterSpeed, enemyShooterDamage, type);
            }
        }
    }
}

// Spawns survivors at random positions on the map
void Game::spawnSurvivors() {
    const float minSpawnDistance = 400.0f;
    SDL_FPoint playerPos = player.centreWorld();
	int numSurvivors = 10 + ((round - 1) * roundEnemyIncrease);

    for (int i = 0; i < survivorSpawnCount; ++i) {
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
	int rescued = survivorPool.checkPlayerCollision(player, survivorHealAmount, survivorSizeIncrease);

	if (rescued > 0) {
		rescuedSurvivors += rescued;
		std::cout << "Rescued Survivors: " << rescuedSurvivors << " / 5\n";
	}
    int killed = enemyPool.handleEnemyDeath(player.getBulletPool());
    if (killed > 0) {
        score += killed;
		roundKills += killed;
		totalKills += killed;
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
        // Press SPACE to go to instructions, H to go to scores
        if (keys[SDL_SCANCODE_SPACE])
            state = GameState::INSTRUCTIONS;
		else if (keys[SDL_SCANCODE_H])
			state = GameState::SCORES;
        return;
    case GameState::SCORES:
		// Press ENTER to return to start
		if (keys[SDL_SCANCODE_RETURN]) {
			state = GameState::START;
		}
		return;
    case GameState::INSTRUCTIONS:
        // Press ENTER to begin game
        if (keys[SDL_SCANCODE_RETURN]) {
            // reset key variables and start first round
            round = 1;
            score = 0;
            rescuedSurvivors = 0;
			totalKills = 0;

            enemyPool = EnemyPool(100);
            survivorPool = SurvivorPool(20);

            spawnEnemies();
            spawnSurvivors();
            player.reset();
            state = GameState::PLAY;
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
			addScores("PLAYER", round - 1, totalKills, rescuedSurvivors, score + round - 1);
            state = GameState::GAMEOVER;
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
        // press ENTER to return to start
        if (keys[SDL_SCANCODE_RETURN]) {
            state = GameState::START;
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

	case GameState::SCORES:
		renderScoresScreen();
		break;

    case GameState::INSTRUCTIONS:
        renderInstructionsScreen();
        break;

    case GameState::PLAY:
		// Draw map grid
        SDL_SetRenderDrawColor(renderer, 50, 100, 50, 255);
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
		if (!roundInProgress)
			renderRoundSummaryOverlay();
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
	roundKills = 0;
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
        if (type == EnemyType::Runner) {
			enemy->init(enemyX, enemyY, enemyRunnerSpeed, enemyRunnerDamage, type);
		}
        else if (type == EnemyType::Shooter) {
			enemy->init(enemyX, enemyY, enemyShooterSpeed, enemyShooterDamage, type);
        }
		else {
			enemy->init(enemyX, enemyY, enemyShooterSpeed, enemyShooterDamage, type);
		}
    }

    // Spawn survivors 
    for (int i = 0; i < survivorSpawnCount; ++i) {
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
	SDL_Color grey = { 200, 200, 200, 255 };
	SDL_Color cyan = { 0, 255, 200, 255 };
    SDL_Color box = { 50, 50, 50, 255 };
    float widthAllign = windowWidth * 0.25f;
    SDL_FRect boxRect = {
        widthAllign - 30.0f, windowHeight * 0.30f - 10.0f, 500.0f, 80.0f
    };
    SDL_SetRenderDrawColor(renderer, box.r, box.g, box.b, box.a);
    SDL_RenderFillRect(renderer, &boxRect);

    drawText("SURVIVOR PROTOCOL", widthAllign, windowHeight * 0.33f, cyan);
    drawText("Recover survivors. Avoid extermination.", widthAllign, windowHeight * 0.43f, white);
    drawText("Press SPACE to view mission briefing", widthAllign, windowHeight * 0.66f, grey);
	drawText("Press H to view high scores", widthAllign, windowHeight * 0.73f, grey);
}

void Game::renderScoresScreen() {
	SDL_Color white = { 255,255,255,255 };
	SDL_Color grey = { 200, 200, 200, 255 };
	SDL_Color cyan = { 0, 255, 200, 255 };
	SDL_Color yellow = { 255, 210, 0, 255 };

	float widthAlign = windowWidth * 0.2f;
	float startY = windowHeight * 0.2f;
	float lineHeight = 30.0f;

	drawText("HIGH SCORES:", widthAlign, startY, white);
    drawText("NAME     | ROUND | KILLS | SURVIVORS | SCORE", widthAlign, startY + 40, white);


    for (size_t i = 0; i < highScores.size(); ++i) {
        const HighScores& hs = highScores[i];
        SDL_Color rowColor = (i == 0) ? yellow : grey;

        std::string entry =
            hs.name + "        |   " +
            std::to_string(hs.round) + "   |   " +
            std::to_string(hs.kills) + "   |     " +
            std::to_string(hs.survivors) + "       |   " +
            std::to_string(hs.score);

        drawText(entry.c_str(), widthAlign, startY + 80 + (lineHeight * i), rowColor);
    }

	drawText("Press ENTER to return to main menu", widthAlign, windowHeight * 0.80f, grey);
}

void Game::renderInstructionsScreen()
{
    SDL_Color white = { 255,255,255,255 };
	SDL_Color grey = { 200, 200, 200, 255 };
	SDL_Color teal = { 0, 200, 200, 255 };
    float widthAlign = windowWidth * 0.20f;
    float iconAlign = widthAlign - 60.0f;
    float conceptY = windowHeight * 0.15f;
    float detailsY = windowHeight * 0.35f;
    float rectSize = 30.0f;

    drawText("MISSION BRIEFING:", widthAlign, conceptY, white);
    drawText("You are the last active rescue unit.", widthAlign, conceptY + 30, teal);
    drawText("Recover survivors and neutralise threats.", widthAlign, conceptY + 60, teal);
    drawText("Each round intensifies — stay operational.", widthAlign, conceptY + 90, teal);

    drawText("CONTROLS:", widthAlign, detailsY, white);
    drawText("WASD / Arrows - Move", widthAlign, detailsY + 30, white);
    drawText("Mouse aims, SPACE shoots", widthAlign, detailsY + 60, white);

    drawText("ENTITY GUIDE:", widthAlign, detailsY + 110, white);
    drawText("[Survivor] - Rescue to gain hp but increase in size", widthAlign, detailsY + 140, white);
    SDL_SetRenderDrawColor(renderer, 200, 0, 200, 255); // purple
    SDL_FRect survivorRect = { iconAlign, detailsY + 140, rectSize, rectSize };
    SDL_RenderFillRect(renderer, &survivorRect);
    drawText("[Runner]   - Charges at you to melee attack", widthAlign, detailsY + 180, white);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
    SDL_FRect runnerRect = { iconAlign, detailsY + 180, rectSize, rectSize };
    SDL_RenderFillRect(renderer, &runnerRect);
    drawText("[Shooter]  - Seeks you out and fires from a distance", widthAlign, detailsY + 220, white);
    SDL_SetRenderDrawColor(renderer, 0, 120, 255, 255); // blue
    SDL_FRect shooterRect = { iconAlign, detailsY + 220, rectSize, rectSize };
    SDL_RenderFillRect(renderer, &shooterRect);

    drawText("Press ENTER to begin protocol", widthAlign, windowHeight * 0.80f, grey);
}

void Game::renderRoundSummaryOverlay()
{
    if (roundInProgress) return; // only show between rounds

    // Calculate time left until next round
    uint64_t currentTime = SDL_GetTicksNS();
    float secondsLeft = (float)(roundDelay - (currentTime - roundEndTime)) / 1000000000.0f;
    if (secondsLeft < 0) secondsLeft = 0;

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color cyan = { 0, 255, 200, 255 };
	SDL_Color yellow = { 255, 210, 0, 255 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);

    // Draw box
    SDL_FRect box = {
        windowWidth * 0.25f, 
        windowHeight * 0.15f,
        windowWidth * 0.5f,
        windowHeight * 0.25f
    };
    SDL_RenderFillRect(renderer, &box);

    float xAlign = windowWidth * 0.27f;
    float y = windowHeight * 0.22f;

    std::string line0 = "SYSTEM REBOOT IN PROGRESS...";
    std::string line1 = "SIMULATION " + std::to_string(round) + " COMPLETE";
    std::string line2 = "Enemies defeated: " + std::to_string(roundKills);
    std::string line3 = "Survivors rescued: " + std::to_string(rescuedSurvivors);
    std::string line4 = "Next simulation starting in: " + std::to_string((int)ceil(secondsLeft));

    // Draw text
	drawText(line0.c_str(), xAlign, y - 30, cyan);
    drawText(line1.c_str(), xAlign, y, white);
    drawText(line2.c_str(), xAlign, y + 30, white);
    drawText(line3.c_str(), xAlign, y + 60, white);
    drawText(line4.c_str(), xAlign, y + 90, yellow);
}


void Game::renderGameOverScreen()
{
    SDL_Color white = { 255,255,255,255 };
	SDL_Color grey = { 200, 200, 200, 255 };
	SDL_Color red = { 255, 0, 0, 255 };
    float widthAllign = windowWidth * 0.25f;
    float heightCenter = windowHeight * 0.5f;

    drawText("PROTOCOL FAILURE", widthAllign, windowHeight * 0.26f, red);
	drawText("Entity destroyed. Simulation over.", widthAllign, windowHeight * 0.33f, white);

    std::string roundLine = "Rounds survived: " + std::to_string(round - 1);
    drawText(roundLine.c_str(), widthAllign, heightCenter - 40, white);
    std::string killLine = "Total enemies defeated: " + std::to_string(totalKills);
    drawText(killLine.c_str(), widthAllign, heightCenter, white);
    std::string scoreLine = "Final score: " + std::to_string(score + round - 1);
    drawText(scoreLine.c_str(), widthAllign, heightCenter + 40, white);

    drawText("Press ENTER to reboot system", widthAllign, windowHeight * 0.66f, grey);
}

