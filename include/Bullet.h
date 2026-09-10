#pragma once

#include <SFML/Graphics.hpp>

enum class BulletOwner {
	Player,
	Enemy
};

class Bullet {
private:
	BulletOwner m_owner;
	float m_speed;
	sf::Vector2f m_position;
	sf::Sprite m_sprite;
	int m_moveDirection;  // если коеф. положительный, то двигается в положительную сторону, отрицательный - в отрицательную
public:
	Bullet(BulletOwner owner, sf::Vector2f position, float speed, sf::Texture& texture, int moveDirection);

	void draw(sf::RenderWindow& window);

	void move(float deltaTime);

	bool isOutOfScreen(sf::Vector2u windowSize) const;

	sf::FloatRect getGlobalBounds() const;

	BulletOwner getOwner() const;
};