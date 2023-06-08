#pragma once
#include "SFML/Graphics.hpp"
#include "settings.h"
#include <vector>
#include "Fonobj.h"
#include "Player1.h"
#include "Player2.h"
#include "HpBar_Player_1.h"
#include "HpBar_Player_2.h"
#include "Bullet_Player_1.h"
#include "Bullet_Player_2.h"
#include "textobj_1.h"
#include "textobj_2.h"
#include "Box.h"
#include "bonus.h"
#include "Lobbi.h"

class Game {
private:
	sf::RenderWindow window;
	Fonobj fon;
	Lobbi lobbi;
	Player1 player1;
	Player2 player2;
	HpBar_1 hpBar1;
	HpBar_2 hpBar2;
	std::string LooseText_2 = "Blue  Win";
	std::string LooseText_1 = "Red  Win";
	enum GameStatus { Lobbi, Play, Loose };
	GameStatus game = Lobbi;
	TextObj_Pl_2 Loose_2_text;
	TextObj_Pl_1 Loose_1_text;
	std::vector<Box*> boxes;
	std::list<Bonus*> bonuses;
public:
	void spawnBoxes(size_t n) {
		for (int i = 0; i < n; i++) {
			int num = rand() % 3 + 1;
			std::string fileName = "images\\Boxes_00" + std::to_string(num) + ".png";
			Box::BoxSize size = (Box::BoxSize)(rand() % 3);
			float x = 50.f;
			float PosX[]{ 6 * x ,12 * x,17 * x,23 * x };
			int indexX = rand() % 4;
			float y = 50.f;
			float PosY[]{ 0.f ,y, 2 * y, 3 * y,4 * y ,5 * y,6 * y,12 * y,13 * y,14 * y,
				15 * y,16 * y,17 * y };
			int indexY = rand() % 13;
			Box* m = new Box(fileName, size, sf::Vector2f{ PosX[indexX], PosY[indexY] });
			boxes.push_back(m);
		}
	}
	Game() :Loose_1_text(LooseText_1, Game_text_pos), Loose_2_text(LooseText_2, Game_text_pos) {
		window.create(sf::VideoMode{ (size_t)WINDOW_WIDTH, (size_t)WINDOW_HEIGHT }, WINDOW_TITLE);
		window.setFramerateLimit(FPS);
		spawnBoxes(500);
	}
	void checkEvents() {
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed) window.close();
	}
	void update() {
		switch (game) {
		case Play: {
			for (int i = 0; i < boxes.size(); i++) {
				boxes.at(i)->update();
			}
			fon.update();
			player1.update();
			player2.update();
			hpBar1.update(player1.getHp());
			hpBar2.update(player2.getHp());
			for (auto& bonus : bonuses) {
				bonus->update();
			}
		}
		case Lobbi: {
			lobbi.update();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				game = Play;
			}
		}
		case Loose: {
			hpBar1.update(player1.getHp());
			hpBar2.update(player2.getHp());
		}
		}
	}
	void checkCollisions() {
		bulletPlayer();
		box();
		bonus();
	}
	void bulletPlayer() {
		sf::FloatRect player_1_Bounds = player1.getHitBox();
		sf::FloatRect player_2_Bounds = player2.getHitBox();
		auto bullets_1_Sprites = player1.getBullets();
		for (auto& bullet_1 : *bullets_1_Sprites) {
			sf::FloatRect bullet_1_Bounds = bullet_1->getHitBox();
			if (bullet_1_Bounds.intersects(player_2_Bounds)) {
				player2.decreaseHp(bullet_1->getDamage());
				bullet_1->hit();
			}
		}
		auto bullets_2_Sprites = player2.getBullets();
		for (auto& bullet_2 : *bullets_2_Sprites) {
			sf::FloatRect bullet_2_Bounds = bullet_2->getHitBox();
			if (bullet_2_Bounds.intersects(player_1_Bounds)) {
				player1.decreaseHp(bullet_2->getDamage());
				bullet_2->hit();;
			}
		}
		for (auto& bullet_1 : *bullets_1_Sprites) {
			sf::FloatRect bullet_1_Bounds = bullet_1->getHitBox();
			for (auto& bullet_2 : *bullets_2_Sprites) {
				sf::FloatRect bullet_2_Bounds = bullet_2->getHitBox();
				if (bullet_1_Bounds.intersects(bullet_2_Bounds)) {
					bullet_1->hit();
					bullet_2->hit();
				}
			}
		}
		(*bullets_1_Sprites).remove_if([](Bullet_1* bullet) {return bullet->getHit(); });
		(*bullets_2_Sprites).remove_if([](Bullet_2* bullet) {return bullet->getHit(); });
	}
	void box() {
		sf::FloatRect player_1_Bounds = player1.getHitBox();
		sf::FloatRect player_2_Bounds = player2.getHitBox();
		auto bullets_1_Sprites = player1.getBullets();
		auto bullets_2_Sprites = player2.getBullets();
		for (auto& box : boxes) {
			sf::FloatRect boxBounds = box->getHitBox();
			for (auto& bullet_1 : (*bullets_1_Sprites)) {
				sf::FloatRect bullet_1_Bounds = bullet_1->getHitBox();
				if (boxBounds.intersects(bullet_1_Bounds)) {
					int chance = rand() % 1000;
					box->Delete();
					bullet_1->hit();
					int bonusType = rand() % Bonus::BonusType::MAX_BONUS_TYPE;
					if (chance < 50) {
						Bonus* bonus = new Bonus(box->getPosition(), bonusType);
						bonuses.push_back(bonus);
					}
				}
			}
			for (auto& bullet_2 : (*bullets_2_Sprites)) {
				sf::FloatRect bullet_2_Bounds = bullet_2->getHitBox();
				if (boxBounds.intersects(bullet_2_Bounds)) {
					int chance = rand() % 1000;
					box->Delete();
					bullet_2->hit();
					int bonusType = rand()% Bonus::BonusType::MAX_BONUS_TYPE;
					if (chance < 50) {
						Bonus* bonus = new Bonus(box->getPosition(), bonusType);
						bonuses.push_back(bonus);
					}
				}
			}
		}
		(*bullets_1_Sprites).remove_if([](Bullet_1* bullet) {return bullet->getHit(); });
		(*bullets_2_Sprites).remove_if([](Bullet_2* bullet) {return bullet->getHit(); });
	}
	void bonus() {
		sf::FloatRect player_1_Bounds = player1.getHitBox();
		sf::FloatRect player_2_Bounds = player2.getHitBox();
		for (auto& bonus : bonuses) {
			sf::FloatRect bonusBounds = bonus->getHitBox();
			if (bonusBounds.intersects(player_1_Bounds)) {
				bonus->act_1(player1);
				bonus->setDel();
			}
			if (bonusBounds.intersects(player_2_Bounds)) {
				bonus->act_2(player2);
				bonus->setDel();
			}
		}
		bonuses.remove_if([](Bonus* bonus) {return bonus->isToDel(); });
	}
	void draw() {
		window.clear();
		window.draw(fon.getSprite());
		for (int i = 0; i < boxes.size(); i++) {
			window.draw(boxes.at(i)->getSprite());
		}
		hpBar1.draw(window);
		hpBar2.draw(window);
		player1.draw(window);
		player2.draw(window);
		for (auto& bonus : bonuses) {
			bonus->draw(window);
		}
		if (player2.hp_2 <= 0) {
			game = Loose;
			Loose_2_text.draw(window);
		}
		if (player1.hp_1 <= 0) {
			game = Loose;
			Loose_1_text.draw(window);
		}
		switch (game) {
		case Lobbi: {
			window.draw(lobbi.getSprite());
		}
		}
		window.display();
	}
	void play() {
		while (window.isOpen()) {
			checkEvents();
			update();
			checkCollisions();
			draw();
		}
	}

};