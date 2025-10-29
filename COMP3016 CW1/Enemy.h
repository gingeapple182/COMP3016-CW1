#pragma once
#include <SDL3/SDL.h>	

enum class EnemyType {
	Runner,
	Shooter,
	Sniper
};
class Enemy {
public:
	Enemy();

	EnemyType type; 

	void init(float startX, float startY, float enemySpeed, int enemyDamage, EnemyType enemyType = EnemyType::Runner);
	void update(float deltaTime, const SDL_FPoint& playerPosition);
	void render(SDL_Renderer* renderer, float cameraX, float cameraY);
	void deactivate();

	bool isActive() const { return active; }

	const SDL_FRect& getRect() const { return rect; }
	int getDamage() const { return damage; } 

private:
	void applyTypeAttributes();

	float x, y;
	float speed;
	bool active;
	int damage;
	SDL_FRect rect;
	SDL_Color colour;
};