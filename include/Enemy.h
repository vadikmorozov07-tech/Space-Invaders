#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Bullet.h"

class Enemy {
private:
	float m_speed;
	sf::Vector2f m_position;
	sf::Sprite m_sprite;
	float m_shootCooldown = 0.f;
	int m_moveDirection = 1;  // если коеф. положительный, то двигается в положительную сторону, отрицательный - в отрицательную
public:
	Enemy(sf::Vector2f position, float speed, sf::Texture& texture);

	void move(float deltaTime);

	bool isOutOfScreen(sf::Vector2u windowSize) const;

	void draw(sf::RenderWindow& window);

	void shoot(float deltaTime, sf::Texture& texture, std::vector<Bullet>& bullets);

	sf::FloatRect getGlobalBounds() const;

	void setX(float x);

	void setY(float y);

	void addX(float x);

	void addY(float y);

	sf::Vector2f getPosition() const;

	void setMoveDirection(int md);

	void setSpeed(float s);

	float getSpeed() const;
};