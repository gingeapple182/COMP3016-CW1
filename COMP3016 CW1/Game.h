#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Player.h"
#include "EnemyPool.h"
#include "SurvivorPool.h"

class Game {
public:
    Game(SDL_Renderer* renderer, TTF_Font* font, int windowWidth, int windowHeight);
    ~Game();

    void update(float dt);
    void render();
    bool isRunning() const { return running; }

private:
    void spawnEnemies();
    void spawnSurvivors();
    void handleCollisions();

    SDL_Renderer* renderer;
    TTF_Font* font;
    int windowWidth;
    int windowHeight;

    const int mapWidth = 4000;
    const int mapHeight = 4000;
    int tileSize = 50;

    Player player;
    EnemyPool enemyPool;
    SurvivorPool survivorPool;

    int score;
    int round;
    bool running;

    float cameraX;
    float cameraY;
};
