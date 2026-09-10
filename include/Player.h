#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Bullet.h"

class Player {
private:
	sf::Vector2f m_position;
	float m_speed;
	sf::Sprite m_sprite;
	float m_shootCooldown = 0.f;
public:
	Player(sf::Vector2f position, float speed, sf::Texture& texture);

	void move(float deltaTime, sf::Vector2u windowSize);

	void draw(sf::RenderWindow& window);

	void shoot(float deltaTime, sf::Texture& texture, std::vector<Bullet>& bullets);

	sf::FloatRect getGlobalBounds() const;
};