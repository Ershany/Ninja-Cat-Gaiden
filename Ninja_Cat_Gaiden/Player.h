#ifndef PLAYER_H
#define PLAYER_H

#include <SFML\Graphics.hpp>

#include "Updateable.h"
#include "Renderable.h"

// Forward declaration to get around the circular reference
class GamestateManager;

class Player : public Updateable, public Renderable {
public:
	Player(sf::Vector2f &position, GamestateManager &gsm);
	~Player();

	void update(const sf::Time &deltaTime) override;

	sf::Vector2f getVelocity();
	sf::Vector2u getSize();

	bool upHeld, downHeld, leftHeld, rightHeld;
	bool leftMouseButtonPressed;
	bool isDead;
	bool isInvincible;
	sf::Vector2i mousePos;
	bool facingRight;
	sf::Vector2f collisionPoints[10]; // Used for collision detection
private:
	void updateVelocity(const sf::Time &deltaTime);
	void updatePosition(const sf::Time &deltaTime);
	void updateCollisionPoints();
	void move(float x, float y);
	void checkProjectileShoot(const sf::Time &deltaTime);
	void checkTileDamage(const sf::Time &deltaTime);
	void takeDamage(int amount, const sf::Time &deltaTime);
	/* Determines if the player can jump, and sets the boolean "canJump" accordingly */
	void checkJump();
	/* Default velocityDrag provided : Vector2f(1.0f, 1.0f) */
	void shootProjectile(sf::Vector2u &size, sf::Vector2f &velocity, sf::Vector2f &velocityDrag = sf::Vector2f(1.0f, 1.0f));

	int maxHealth, currentHealth;

	sf::Vector2f speed;
	sf::Vector2f velocity;
	sf::Vector2u size;

	int collisionTune;
	sf::Vector2f velocityDrag;
	float gravitySpeed, fallRate, currentFallRate;
	bool jumping, canJump;
	float jumpPower;
	sf::Vector2i lastWallCollision;

	sf::Time projectileFreq, currentProjectileFreq;
	sf::Vector2f projectileSpeed;

	sf::Time invincibilityTime, currentInvincibilityTime;


	GamestateManager &gsm;
};

#endif