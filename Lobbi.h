#pragma once
#include <SFML/Graphics.hpp>
#include "settings.h"

class Lobbi {
private:
	sf::Texture texture;
	sf::Sprite sprite;
public:
	Lobbi() {
		texture.loadFromFile(Lobbi_File_Name);
		sprite.setTexture(texture);
		sprite.setPosition(300, 0);
	}
	void update() {}
	sf::Sprite& getSprite() { return sprite; }
};
