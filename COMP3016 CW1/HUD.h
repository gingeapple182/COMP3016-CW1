// HUD.h
#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class EnemyPool;
class SurvivorPool;
class Player;

void DrawHUD(SDL_Renderer* renderer,
    TTF_Font* font,
    int round,
    int score,
    const EnemyPool& enemyPool,
	const SurvivorPool& survivorPool,
    const Player& player,
    int windowWidth,
    int windowHeight);
