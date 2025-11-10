#pragma once
#include <SDL3/SDL.h>	

class BulletPool;

enum class EnemyType {
	Runner,
	Shooter,
	Sniper
};

class Enemy {
public:
	Enemy();

	EnemyType type; 

	void init(float startX, float startY, float enemySpeed, int enemyDamage, EnemyType enemyType);
	void update(float deltaTime, const SDL_FPoint& playerPosition, BulletPool& enemyBullets);
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
	//extras for shooter
	float engageDistance = 400.0f;
	float baseSpeed = 10.0f;
	bool isShooterShooting = false;
	float shooterCooldown = 0.0f;
	float shooterCooldownReset = 15.0f;
	float ShooterShootySpeed = 50.0f;
};