#include "HUD.h"

HUD::HUD(Model* m, sf::RenderWindow* w, TextureManager* t) {
	this->model = m;
	this->window = w;
	this->textureManager = t;

	centuryGothic.loadFromFile("Resources/HUD/fonts/century_gothic.ttf");

	hudMain.setTexture(textureManager->getTexture("Resources/HUD/hud-main.png"));

	health.setFillColor(sf::Color::Red);
	health.setPosition(sf::Vector2f(145, 590));

	stamina.setFillColor(sf::Color::Green);
	stamina.setPosition(sf::Vector2f(142, 615));

	score.setFont(centuryGothic);
	score.setFillColor(sf::Color::White);
	score.setCharacterSize(20);
	score.setPosition(sf::Vector2f(250, 560));

	hiddenInfo.setFont(centuryGothic);
	hiddenInfo.setFillColor(sf::Color::White);
	hiddenInfo.setCharacterSize(20);
	hiddenInfo.setPosition(sf::Vector2f(1100, 25));

	//item.setScale(sf::Vector2f(3, 3));
	item.setPosition(sf::Vector2f(45, 570));
}

void HUD::draw() {
	this->setStats();

	window->draw(item);
	window->draw(health);
	window->draw(stamina);
	window->draw(score);
	window->draw(hudMain);

	if (model->player->hidden) {
		window->draw(hiddenInfo);
	}
}

void HUD::setStats() {
	item.setTexture(textureManager->getTexture("Resources/Player/craftables/shuriken.png"));
	score.setString(std::to_string(model->score));
	hiddenInfo.setString("Hidden");
	health.setSize(sf::Vector2f(280 * ((float)model->player->getCurrHealth() / (float)model->player->getMaxHealth()), 25));
	stamina.setSize(sf::Vector2f(255 * ((float)model->player->getCurrStamina() / (float)model->player->getMaxStamina()), 15));
}

HUD::~HUD() {}