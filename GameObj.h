#pragma once
#include"settings.h"
#include<iostream>

class GameObj {
public:
	virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;

	sf::Vector2f getPosition() { return sprite.getPosition(); }
	sf::FloatRect getHitBox() { return sprite.getGlobalBounds(); }
protected:
	sf::Texture texture;
	sf::Sprite sprite;
	float speed, speedx, speedy;
	int damage;
	int rage_bonus_act_time;
	int speed_bonus_act_time;
	bool speed_bonus_act = false;
	bool rage_bonus_act = false;
};