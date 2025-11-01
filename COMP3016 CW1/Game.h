#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "Player.h"
#include "EnemyPool.h"
#include "SurvivorPool.h"

enum class GameState {
    START,
    INSTRUCTIONS,
    PLAY,
    GAMEOVER
};

class Game {
public:
    Game(SDL_Renderer* renderer, TTF_Font* font, int windowWidth, int windowHeight);
    ~Game();

    void update(float dt);
    void render();
    bool isRunning() const { return running; }
	void startNewRound();
	void endRound();
	void checkRoundProgression();

	// Config parameters
    // Player config
    int playerStartHealth;
    float playerStartWidth;
    float playerStartHeight;
    float playerStartSpeed;
	// Enemy config
    float enemyRunnerSpeed;
    int enemyRunnerDamage;
	float enemyShooterSpeed;
	int enemyShooterDamage;
	// World config
    int mapWidth;
	int mapHeight;
	int tileSize;
	int survivorSpawnCount;
	int survivorHealAmount;
    float survivorSizeIncrease;
    int roundEnemyIncrease;

private:
	GameState state = GameState::START;

    void spawnEnemies();
    void spawnSurvivors();
    void handleCollisions();

	void renderStartScreen();
	void renderInstructionsScreen();
	void renderRoundSummaryOverlay();
	void renderGameOverScreen();
    void drawText(const char* text, float x, float y, SDL_Color color);

    SDL_Renderer* renderer;
    TTF_Font* font;
    int windowWidth;
    int windowHeight;

    //const int mapWidth = 4000;
    //const int mapHeight = 4000;
    //int tileSize = 50;

    Player player;
    EnemyPool enemyPool;
    SurvivorPool survivorPool;

    int score;
    int round = 1;
	bool roundInProgress = true;
	uint64_t roundEndTime = 0;
	const uint64_t roundDelay = 5000000000; // 5 seconds in nanoseconds
	int roundKills = 0;
	int totalKills = 0;
	int rescuedSurvivors = 0;
    bool running;

    float cameraX;
    float cameraY;

    void loadConfig();
};