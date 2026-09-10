#pragma once

#include <SFML/Graphics.hpp>

class Menu {
private:
	sf::RectangleShape m_continueButton;
	sf::RectangleShape m_exitButton;
public:
	Menu();

	void drawMenu(const sf::Font& font, sf::RenderWindow& window, const sf::Vector2u& windowSize);

	const sf::RectangleShape& getContinue() const;

	const sf::RectangleShape& getExit() const;

};