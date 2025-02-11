#include "Enemy.h"

Enemy::Enemy(sf::Vector2f &position, GamestateManager &gsm, Player *player)
	: gsm(gsm)
{
	this->player = player;
	this->position = position;
	this->shouldRemove = false;
	this->facingRight = true;
	this->alertOfPlayer = false;
	this->currentMovement = 0.0f;
	this->maxMovement = 5000.0f;
	this->maxSightRange = 300.0f;
	this->reducedSightRange = 250.0f;
	this->sightRange = maxSightRange;
}

Enemy::~Enemy() {

}

void Enemy::update(const sf::Time &deltaTime) {

}