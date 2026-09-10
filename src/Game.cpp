#include "Game.h"

#include <stdexcept>
#include <sstream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>

sf::Texture loadTexture(const std::string& filename) {
	sf::Texture texture;
	if (!texture.loadFromFile(filename)) {
		throw std::runtime_error("Failed to load texture: " + filename);
	}
	return texture;
}

std::string iToStr(int x) {
	std::ostringstream ss;
	ss << x;
	return ss.str();
}

Game::Game() : m_playerTexture(loadTexture("assets/textures/player.png")), m_bulletTexture(loadTexture("assets/textures/bullet.png")), 
			   m_enemyTexture(loadTexture("assets/textures/enemy.png")), m_player({ 450.f, 1100.f }, 300.f, m_playerTexture), 
			   m_background(loadTexture("assets/textures/background.png")) {

	loadFormation();
}

void Game::loadFormation() {

	std::ifstream file("assets/formations/formations.txt");

	if (!file.is_open()) {
		throw std::runtime_error("Failed to open formations.txt");
	}

	std::string line;
	Formation formation;

	int y = 0;

	while (std::getline(file, line)) {
		if (line.empty()) {
			y = 0;
			if (!formation.empty()) {
				m_formations.push_back(formation);
				formation.clear();
			}
		}
		else {
			for (int x = 0; x < line.size(); ++x) {
				if (line[x] == '1') {
					formation.push_back({ x, y });
				}
			}
			++y;
		}
	}

	if (!formation.empty()) {
		m_formations.push_back(formation);
	}
}

void Game::update(float deltaTime, const sf::Vector2u& windowSize) {

	bool changeDirection = false;
	int newDirection = 0;

	m_player.move(deltaTime, windowSize);
	m_player.shoot(deltaTime, m_bulletTexture, m_bullets);

	for (auto& e : m_enemies) {
		e.shoot(deltaTime, m_bulletTexture, m_bullets);
	}

	for (auto& bullet : m_bullets) {
		bullet.move(deltaTime);
	}

	for (auto& enemy : m_enemies) {
		if (enemy.getPosition().x <= 0.f) {
			enemy.setX(0.f);
			changeDirection = true;
			newDirection = 1;
		}
		if (enemy.getPosition().x + enemy.getGlobalBounds().size.x >= windowSize.x) {
			enemy.setX(windowSize.x - enemy.getGlobalBounds().size.x);
			changeDirection = true;
			newDirection = -1;
		}
	}


	if (changeDirection) {
		for (auto& enemy : m_enemies) {
			enemy.addY(enemy.getGlobalBounds().size.y);
			enemy.setMoveDirection(newDirection);
		}
	}

	for (auto& enemy : m_enemies) {
		enemy.move(deltaTime);
	}

	for (int i = 0; i < m_bullets.size(); ++i) { // столкновение пули игрока с врагом
		if (m_bullets[i].getOwner() == BulletOwner::Enemy) {
			continue;
		}

		for (int j = 0; j < m_enemies.size(); ++j) {
			sf::FloatRect bulletBounds = m_bullets[i].getGlobalBounds(); // границы пули
			sf::FloatRect enemyBounds = m_enemies[j].getGlobalBounds(); // границы врага
			if (enemyBounds.findIntersection(bulletBounds)) { // проверка на пересечение
				m_bullets.erase(m_bullets.begin() + i);

				// компенсируем последующее ++i в цикле, так как после удаления пули
				// элементы вектора сдвинутся на 1 позицию влево и можно пропустить элемент
				--i;

				m_enemies.erase(m_enemies.begin() + j);

				addScore(100);

				for (auto& enemy : m_enemies) {
					enemy.setSpeed(enemy.getSpeed() + 10.f);
				}

				break;
			}
		}

	}

	for (int i = 0; i < m_bullets.size(); ++i) { // столкновение пули врага с игроком
		if (m_bullets[i].getOwner() == BulletOwner::Player) {
			continue;
		}

		sf::FloatRect bulletBounds = m_bullets[i].getGlobalBounds(); // границы пули
		sf::FloatRect playerBounds = m_player.getGlobalBounds(); // границы игрока

		if (playerBounds.findIntersection(bulletBounds)) { // проверка на пересечение
			m_bullets.erase(m_bullets.begin() + i);

			// компенсируем последующее ++i в цикле, так как после удаления пули
			// элементы вектора сдвинутся на 1 позицию влево и можно пропустить элемент
			--i;

			loseLive();
		}
	}

	for (int i = 0; i < m_enemies.size(); ) {
		sf::FloatRect enemyBounds = m_enemies[i].getGlobalBounds(); // границы врага
		sf::FloatRect playerBounds = m_player.getGlobalBounds(); // границы игрока
		if (enemyBounds.findIntersection(playerBounds)) {
			m_enemies.erase(m_enemies.begin() + i);
			loseLive();
		}
		else {
			++i;
		}
	}

	for (auto it = m_bullets.begin(); it != m_bullets.end(); ) {
		if (it->isOutOfScreen(windowSize)) {
			it = m_bullets.erase(it);
		}
		else {
			++it;
		}
	}

	for (int i = 0; i < m_enemies.size(); ) {
		if (m_enemies[i].isOutOfScreen(windowSize)) {
			m_enemies.erase(m_enemies.begin() + i);
			loseLive();
		}
		else {
			++i;
		}
	}

	if (m_enemies.empty() && !m_newWave) {
		++m_wave;
		m_newWave = true;
	}
}

int Game::getWave() const {
	return m_wave;
}

bool Game::isNewWave() const {
	return m_newWave;
}

void Game::createWave() {
	m_bullets.clear();

	int formation = rand() % m_formations.size();

	for (auto& coordinats : m_formations[formation]) {

		float x = 10.f + coordinats.first * 100.f;
		float y = 120.f + coordinats.second * 90.f;

		m_enemies.emplace_back(sf::Vector2f({ x, y }), m_waveSpeed * (1.f + (m_wave - 1) * 0.2f), m_enemyTexture);
	}
}

void Game::draw(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2u& windowSize) {
	sf::Sprite background(m_background);
	window.draw(background);
	m_player.draw(window);
	for (auto& enemy : m_enemies) {
		enemy.draw(window);
	}
	for (auto& bullet : m_bullets) {
		bullet.draw(window);
	}

	sf::Text score(font, "SCORE: " + iToStr(getScore()));
	score.setCharacterSize(35);
	score.setFillColor(sf::Color::White);
	score.setPosition({ 30.f, 30.f });
	window.draw(score);

	sf::Text live(font, "LIVES: " + iToStr(getLives()));
	live.setCharacterSize(35);
	live.setFillColor(sf::Color::White);
	live.setPosition({ windowSize.x - live.getGlobalBounds().size.x - 30.f,
	30.f });
	window.draw(live);
}

void Game::drawGameOver(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2u& windowSize) {
	window.clear(sf::Color(35, 5, 10));

	sf::Text lose(font, "GAME OVER");
	lose.setCharacterSize(30);
	lose.setFillColor(sf::Color(255, 60, 60));
	float x = (windowSize.x - lose.getGlobalBounds().size.x) / 2.f;
	float y = (windowSize.y - lose.getGlobalBounds().size.y) / 2.f;

	lose.setPosition({ x, y });
	window.draw(lose);
	window.display();

	std::this_thread::sleep_for(std::chrono::seconds(3));

	window.close();
}

void Game::drawVictory(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2u& windowSize) {
	window.clear(sf::Color(5, 30, 15));

	sf::Text win(font, "VICTORY");
	win.setCharacterSize(30);
	win.setFillColor(sf::Color(50, 255, 80));
	float x = (windowSize.x - win.getGlobalBounds().size.x) / 2.f;
	float y = (windowSize.y - win.getGlobalBounds().size.y) / 2.f;

	win.setPosition({ x, y });
	window.draw(win);
	window.display();

	std::this_thread::sleep_for(std::chrono::seconds(3));

	window.close();
}

void Game::showWave(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2u& windowSize) {
	window.clear(sf::Color(5, 10, 30));

	sf::Text wave(font, "WAVE " + iToStr(m_wave));
	wave.setCharacterSize(50);
	wave.setFillColor(sf::Color::White);
	float x = (windowSize.x - wave.getGlobalBounds().size.x) / 2.f;
	float y = (windowSize.y - wave.getGlobalBounds().size.y) / 2.f;

	wave.setPosition({ x, y });

	window.draw(wave);
	window.display();

	std::this_thread::sleep_for(std::chrono::seconds(2));

	m_newWave = false;
}

void Game::loseLive() {
	if (m_lives > 0) {
		--m_lives;
	}
}

int Game::getLives() const {
	return m_lives;
}

int Game::getScore() const {
	return m_score;
}

void Game::addScore(int score) {
	m_score += score;
}

bool Game::isEnemiesEmpty() const {
	return m_enemies.empty();
}

void Game::setStatus(GameStatus status) {
	m_game_status = status;
}

GameStatus Game::getStatus() const {
	return m_game_status;
}