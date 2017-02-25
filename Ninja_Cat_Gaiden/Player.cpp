#include "Player.h"
#include "GamestateManager.h"

#include <iostream>
#include <cmath>

// TODO: Make movement framerate independant
Player::Player(sf::Vector2f &pos, GamestateManager &gsm) 
	: gsm(gsm)
{
	this->position = pos;
	this->size.x = 56;
	this->size.y = 68;
	this->jumping = false; this->canJump = false;
	this->currentFallRate = 1.0f; // Don't touch this, keeps track of the falling speed

	// Tweakable variables
	this->speed.x = 64.0f; // Horizontal Speed
	this->speed.y = 86.0f; // Vertical Speed (Jump Speed)
	this->velocityDrag.x = 0.90f; // The rate at which velocity dissipates (Lower the faster the player stops moving)
	this->velocityDrag.y = 0.95;
	this->gravitySpeed = 0.25f; // The rate at which you fall due to gravity (Higher = faster)
	this->collisionTune = 20; // Higher the value, more accurate collision is (NOTE: This may need to be turned up if the movement is fast)
	this->jumpPower = 0.25f;
	this->fallRate = 0.1f; // Higher the value, faster the player falls

	// Initialize required variables
	downHeld = false; upHeld = false; rightHeld = false; leftHeld = false;
	updateCollisionPoints();
}

Player::~Player() {

}

void Player::update(const sf::Time &deltaTime) {
	// Make sure a gamestate is currently being played, log it if there is no gamestate on the stack
	if (gsm.getCurrentState() == NULL) {
		std::cout << "Player Update Cancelled: No Gamestate On The Stack" << std::endl;
		return;
	}
	
	// Make sure the player can jump if they are able to
	checkJump();

	// Update the player's velocity and position
	updateVelocity(deltaTime);
	updatePosition(deltaTime);

	// Update the sprites position (screen position)
	sprite.setPosition(position - gsm.getCurrentState()->getTilemap()->getOffset());
}

void Player::updateVelocity(const sf::Time &deltaTime) {
	// Get the horizontal input from the player
	float xChange = 0.0f;
	if (leftHeld) xChange -= 1.0f;
	if (rightHeld) xChange += 1.0f;
	if (upHeld && canJump) {
		velocity.y = -jumpPower;
		jumping = true;
	}

	// Velocity Affected By Input
	velocity.x += deltaTime.asSeconds() * xChange;

	// Gravity
	if (velocity.y > 0) { currentFallRate += fallRate; }
	else { currentFallRate = 1.0f; }
	velocity.y += deltaTime.asSeconds() * gravitySpeed * currentFallRate;

	// Velocity Drag
	velocity.x *= velocityDrag.x;
	velocity.y *= velocityDrag.y;
	if (abs(velocity.x) < 0.001) velocity.x = 0;
	if (abs(velocity.y) < 0.001) velocity.y = 0;
}

void Player::updatePosition(const sf::Time &deltaTime) {
	// Update the player position relative to the velocity and speed
	move(velocity.x * speed.x, velocity.y * speed.y);
}

void Player::updateCollisionPoints() {
	collisionPoints[0] = sf::Vector2f(position.x, position.y + size.y - 1); // Bottom left
	collisionPoints[1] = sf::Vector2f(position.x + size.x - 1, position.y + size.y - 1); // Bottom Right
	collisionPoints[2] = sf::Vector2f(position.x, position.y); // Top Left
	collisionPoints[3] = sf::Vector2f(position.x + size.x - 1, position.y); // Top Right
	collisionPoints[4] = sf::Vector2f(position.x + size.x - 1, position.y + (size.y / 4) + 1); // High Middle Right
	collisionPoints[5] = sf::Vector2f(position.x, position.y + (size.y / 4) + 1); // High Middle Left
	collisionPoints[6] = sf::Vector2f(position.x + (size.x / 2), position.y); // Top Middle
	collisionPoints[7] = sf::Vector2f(position.x + (size.x / 2), position.y + size.y - 1); // Bottom Middle
	collisionPoints[8] = sf::Vector2f(position.x + size.x - 1, position.y + (size.y * 0.75) - 1); // High Middle Right
	collisionPoints[9] = sf::Vector2f(position.x, position.y + (size.y * 0.75) - 1); // High Middle Left
}

void Player::move(float x, float y) {
	// Check all edges of the player to see what tile they are on
	Tilemap *map = gsm.getCurrentState()->getTilemap();

	// Calculate how much to move the player
	float xChange = x / collisionTune;
	float yChange = y / collisionTune;

	for (int i = 0; i < collisionTune; ++i) {
		// Get the which tile all of the collision points are on
		Tile *downwardsLeftFootCheck = map->getTileByCoordinates(collisionPoints[0] + sf::Vector2f(0.0f, 1.0f));
		Tile *downwardsRightFootCheck = map->getTileByCoordinates(collisionPoints[1] + sf::Vector2f(0.0f, 1.0f));
		Tile *downwardsMiddleFootCheck = map->getTileByCoordinates(collisionPoints[7] + sf::Vector2f(0.0f, 1.0f));
		Tile *upwardsLeftHeadCheck = map->getTileByCoordinates(collisionPoints[2] + sf::Vector2f(0.0f, -1.0f));
		Tile *upwardsRightHeadCheck = map->getTileByCoordinates(collisionPoints[3] + sf::Vector2f(0.0f, -1.0f));
		Tile *upwardsMiddleHeadCheck = map->getTileByCoordinates(collisionPoints[6] + sf::Vector2f(0.0f, -1.0f));
		Tile *leftwardsLeftHeadCheck = map->getTileByCoordinates(collisionPoints[2] + sf::Vector2f(-1.0f, 0.0f));
		Tile *leftwardsLeftHighMiddleCheck = map->getTileByCoordinates(collisionPoints[5] + sf::Vector2f(-1.0f, 0.0f));
		Tile *leftwardsLeftLowMiddleCheck = map->getTileByCoordinates(collisionPoints[9] + sf::Vector2f(-1.0f, 0.0f));
		Tile *leftwardsLeftFootCheck = map->getTileByCoordinates(collisionPoints[0] + sf::Vector2f(-1.0f, 0.0f));
		Tile *rightwardsRightHeadCheck = map->getTileByCoordinates(collisionPoints[3] + sf::Vector2f(1.0f, 0.0f));
		Tile *rightwardsRightHighMiddleCheck = map->getTileByCoordinates(collisionPoints[4] + sf::Vector2f(1.0f, 0.0f));
		Tile *rightwardsRightLowMiddleCheck = map->getTileByCoordinates(collisionPoints[8] + sf::Vector2f(1.0f, 0.0f));
		Tile *rightwardsRightFootCheck = map->getTileByCoordinates(collisionPoints[1] + sf::Vector2f(1.0f, 0.0f));

		// Check if the player is out of the map, to avoid a null pointer exception
		if (downwardsLeftFootCheck == NULL || downwardsRightFootCheck == NULL || upwardsLeftHeadCheck == NULL || upwardsRightHeadCheck == NULL ||
			leftwardsLeftFootCheck == NULL || leftwardsLeftHeadCheck == NULL || rightwardsRightFootCheck == NULL || rightwardsRightHeadCheck == NULL) {
			// Log it
			std::cout << "Player Out Of The Map: Can't Move The Player" << std::endl;
			return;
		}
		
		// Determine what direction needs to be checked vertically
		if (yChange > 0) { // Downwards
			if (!downwardsLeftFootCheck->getSolid() && !downwardsRightFootCheck->getSolid() && !downwardsMiddleFootCheck->getSolid()) {
				position.y += yChange;
			}
			else { // If the player is falling really fast they might get stuck in a tile, so set the player's position
				position.y = downwardsMiddleFootCheck->getPosition().y - size.y;
				velocity.y = 0;
			}
		}
		else if (yChange < 0) { // Upwards
			if (!upwardsLeftHeadCheck->getSolid() && !upwardsRightHeadCheck->getSolid() && !upwardsMiddleHeadCheck->getSolid()) {
				position.y += yChange;
			}
			else { // When the player jumps and hits a tile, reset the player's vertical velocity
				velocity.y = 0; 
			}
		}
		// Determine what direction needs to be checked horizontally
		if (xChange > 0) { // Right
			if (!rightwardsRightHeadCheck->getSolid() && !rightwardsRightHighMiddleCheck->getSolid() && !rightwardsRightFootCheck->getSolid() && !rightwardsRightLowMiddleCheck->getSolid()) {
				position.x += xChange;
			}
		}
		else if (xChange < 0) { // Left
			if (!leftwardsLeftHeadCheck->getSolid() && !leftwardsLeftHighMiddleCheck->getSolid() && !leftwardsLeftFootCheck->getSolid() && !leftwardsLeftLowMiddleCheck->getSolid()) {
				position.x += xChange;
			}
		}

		// Lastly, update the collision points after the movement
		updateCollisionPoints();
	}
}

void Player::checkJump() {
	// Get the tiles right below the feet of the player
	Tilemap *map = gsm.getCurrentState()->getTilemap();
	Tile *leftFootTile = map->getTileByCoordinates(collisionPoints[0] + sf::Vector2f(0.0f, 1.0f));
	Tile *rightFootTile = map->getTileByCoordinates(collisionPoints[1] + sf::Vector2f(0.0f, 1.0f));
	Tile *middleFootTile = map->getTileByCoordinates(collisionPoints[7] + sf::Vector2f(0.0f, 1.0f));

	// If one of them are solid, the player can jump
	if (leftFootTile->getSolid() || rightFootTile->getSolid() || middleFootTile->getSolid()) {
		canJump = true;
		jumping = false;
	}
	else {
		canJump = false;
	}
}

sf::Vector2u Player::getSize() {
	return size;
}

sf::Vector2f Player::getVelocity() {
	return velocity;
}