#include "Menu.h"

Menu::Menu() {
	m_continueButton.setSize({ 200.f, 100.f });
	m_continueButton.setFillColor(sf::Color(20, 70, 40));
	m_continueButton.setOutlineThickness(2.f);
	m_continueButton.setOutlineColor(sf::Color(50, 180, 90));

	m_exitButton.setSize({ 200.f, 100.f });
	m_exitButton.setFillColor(sf::Color(80, 25, 25));
	m_exitButton.setOutlineThickness(2.f);
	m_exitButton.setOutlineColor(sf::Color(200, 60, 60));
}

void Menu::drawMenu(const sf::Font& font, sf::RenderWindow& window, const sf::Vector2u& windowSize) {
	
	m_continueButton.setPosition({ (windowSize.x - 200.f) / 2.f, windowSize.y * 0.35f });
	m_exitButton.setPosition({ (windowSize.x - 200.f) / 2.f, windowSize.y * 0.55f });
	
	sf::RectangleShape background;
	background.setSize(sf::Vector2f(window.getSize()));
	background.setFillColor(sf::Color(0, 0, 0, 180));
	window.draw(background);

	sf::Text pause(font, "PAUSE");
	pause.setCharacterSize(50);
	pause.setFillColor(sf::Color::White);
	float pauseX = (windowSize.x - pause.getGlobalBounds().size.x) / 2.f;

	float pauseY = windowSize.y * 0.17f - pause.getGlobalBounds().position.y;

	pause.setPosition({ pauseX, pauseY });;
	window.draw(pause);

	window.draw(m_continueButton);
	window.draw(m_exitButton);

	sf::Text continueText(font, "Continue");
	continueText.setCharacterSize(30);
	continueText.setFillColor(sf::Color::White);
	float continueX = m_continueButton.getPosition().x + (m_continueButton.getSize().x - continueText.getGlobalBounds().size.x) / 2.f;

	float continueY = m_continueButton.getPosition().y + (m_continueButton.getSize().y - continueText.getGlobalBounds().size.y) / 2.f - continueText.getGlobalBounds().position.y;

	continueText.setPosition({ continueX, continueY });
	window.draw(continueText);

	sf::Text exitText(font, "Exit");
	exitText.setCharacterSize(30);
	exitText.setFillColor(sf::Color::White);
	float exitX = m_exitButton.getPosition().x + (m_exitButton.getSize().x - exitText.getGlobalBounds().size.x) / 2.f;

	float exitY = m_exitButton.getPosition().y + (m_exitButton.getSize().y - exitText.getGlobalBounds().size.y) / 2.f - exitText.getGlobalBounds().position.y;

	exitText.setPosition({ exitX, exitY });
	window.draw(exitText);
}

const sf::RectangleShape& Menu::getContinue() const {
	return m_continueButton;
}

const sf::RectangleShape& Menu::getExit() const {
	return m_exitButton;
}