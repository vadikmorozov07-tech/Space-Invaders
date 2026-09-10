#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"

enum class GameStatus {
	Playing,
	Menu
};

class Game {
private:
	sf::Texture m_playerTexture;
	sf::Texture m_bulletTexture;
	sf::Texture m_enemyTexture;
	sf::Texture m_background;

	Player m_player;
	std::vector<Enemy> m_enemies;

	std::vector<Bullet> m_bullets;

	using Formation = std::vector<std::pair<int, int>>;
	std::vector<Formation> m_formations; // вектор состоит из векторов, которые хранят координаты для отдельного построения

	int m_lives = 3;
	int m_score = 0;
	int m_wave = 1;
	float m_waveSpeed = 50.f;

	bool m_newWave = true; // Сейчас нужно показать и создать новую волну?

	GameStatus m_game_status = GameStatus::Playing;

	void loadFormation();

public:
	Game();

	void update(float deltaTime, const sf::Vector2u& windowSize);

	int getWave() const;

	bool isNewWave() const;

	void createWave();

	void draw(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2u& windowSize);

	void drawGameOver(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2u& windowSize);

	void drawVictory(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2u& windowSize);

	void showWave(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2u& windowSize);

	void loseLive();

	int getLives() const;

	int getScore() const;

	void addScore(int score);

	bool isEnemiesEmpty() const;

	void setStatus(GameStatus status);

	GameStatus getStatus() const;
};