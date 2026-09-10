#include "Bullet.h"

Bullet::Bullet(BulletOwner owner, sf::Vector2f position, float speed, sf::Texture& texture, int moveDirection) :
	m_owner(owner), m_position(position), m_speed(speed), m_sprite(texture), m_moveDirection(moveDirection) {}

void Bullet::draw(sf::RenderWindow& window) {
	window.draw(m_sprite);
}

void Bullet::move(float deltaTime) {
	m_position.y += m_moveDirection * m_speed * deltaTime;
	m_sprite.setPosition(m_position);
}

bool Bullet::isOutOfScreen(sf::Vector2u windowSize) const {
	sf::FloatRect bounds = m_sprite.getGlobalBounds();
	return m_position.y + bounds.size.y < 0 || m_position.y > windowSize.y;
}

sf::FloatRect Bullet::getGlobalBounds() const {
	return m_sprite.getGlobalBounds();
}

BulletOwner Bullet::getOwner() const {
	return m_owner;
}