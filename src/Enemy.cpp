#include "Enemy.h"

Enemy::Enemy(sf::Vector2f position, float speed, sf::Texture& texture) :
	m_position(position), m_speed(speed), m_sprite(texture), m_shootCooldown(1.5f + static_cast<float>(rand() % 10000) / 1000) {
	m_sprite.setPosition(m_position);
}

void Enemy::move(float deltaTime) {
	m_position.x += m_moveDirection * m_speed * deltaTime;
	m_sprite.setPosition(m_position);
}

bool Enemy::isOutOfScreen(sf::Vector2u windowSize) const {
	sf::FloatRect bounds = m_sprite.getGlobalBounds();
	return m_position.y + bounds.size.y >= windowSize.y;
}

void Enemy::draw(sf::RenderWindow& window) {
	window.draw(m_sprite);
}

void Enemy::shoot(float deltaTime, sf::Texture& texture, std::vector<Bullet>& bullets) {
	m_shootCooldown -= deltaTime;

	if (m_shootCooldown <= 0.f) {

		sf::Sprite sprite(texture);

		sf::FloatRect enemyBounds = m_sprite.getGlobalBounds();
		sf::FloatRect bulletBounds = sprite.getGlobalBounds();

		sf::Vector2f bulletPosition = { m_position.x + (enemyBounds.size.x - bulletBounds.size.x) / 2, m_position.y + enemyBounds.size.y - bulletBounds.size.y };

		bullets.emplace_back(BulletOwner::Enemy, bulletPosition, 400.f, texture, 1);
		m_shootCooldown = 1.5f + static_cast<float>(rand() % 10000) / 1000;
	}
}

sf::FloatRect Enemy::getGlobalBounds() const {
	return m_sprite.getGlobalBounds();
}

void Enemy::setX(float x) {
	m_position.x = x;
	m_sprite.setPosition(m_position);
}

void Enemy::setY(float y) {
	m_position.y = y;
	m_sprite.setPosition(m_position);
}

void Enemy::addX(float x) {
	m_position.x += x;
	m_sprite.setPosition(m_position);
}

void Enemy::addY(float y) {
	m_position.y += y;
	m_sprite.setPosition(m_position);
}

sf::Vector2f Enemy::getPosition() const {
	return m_position;
}

void Enemy::setMoveDirection(int md) {
	m_moveDirection = md;
}

void Enemy::setSpeed(float s) {
	m_speed = s;
}

float Enemy::getSpeed() const {
	return m_speed;
}