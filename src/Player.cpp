#include "Player.h"

Player::Player(sf::Vector2f position, float speed, sf::Texture& texture) : m_position(position), m_speed(speed), m_sprite(texture) {
	m_sprite.setPosition(m_position);
}

void Player::move(float deltaTime, sf::Vector2u windowSize) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		m_position.x -= m_speed * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		m_position.x += m_speed * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		m_position.y -= m_speed * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		m_position.y += m_speed * deltaTime;
	}

	sf::FloatRect bounds = m_sprite.getGlobalBounds();

	if (m_position.x < 0) {
		m_position.x = 0;
	}
	if (m_position.x + bounds.size.x > windowSize.x) {
		m_position.x = windowSize.x - bounds.size.x;
	}
	if (m_position.y < 0) {
		m_position.y = 0;
	}
	if (m_position.y + bounds.size.y > windowSize.y) {
		m_position.y = windowSize.y - bounds.size.y;
	}

	m_sprite.setPosition(m_position);
}

void Player::draw(sf::RenderWindow& window) {
	window.draw(m_sprite);
}

void Player::shoot(float deltaTime, sf::Texture& texture, std::vector<Bullet>& bullets) {

	m_shootCooldown -= deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {


		if (m_shootCooldown <= 0.f) {

			sf::Sprite sprite(texture);

			sf::FloatRect playerBounds = m_sprite.getGlobalBounds();
			sf::FloatRect bulletBounds = sprite.getGlobalBounds();

			sf::Vector2f bulletPosition = { m_position.x + (playerBounds.size.x - bulletBounds.size.x) / 2, m_position.y - bulletBounds.size.y };

			bullets.emplace_back(BulletOwner::Player, bulletPosition, 400.f, texture, -1);
			m_shootCooldown = 0.5f;
		}
	}
}

sf::FloatRect Player::getGlobalBounds() const {
	return m_sprite.getGlobalBounds();
}