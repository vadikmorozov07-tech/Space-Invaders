#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>

#include "Game.h"
#include "Menu.h"



int main() {
	srand(static_cast<unsigned>(time(nullptr)));
	sf::Vector2u windowSize = { 1000, 1200 };
	sf::RenderWindow window(sf::VideoMode(windowSize), "Space Invaders");
	const sf::Font font("assets/fonts/arial.ttf");

	try {
		Game game;
		Menu menu;

		sf::Clock clock;

		while (window.isOpen()) {
			while (const std::optional event = window.pollEvent()) {
				if (event->is<sf::Event::Closed>()) {
					window.close();
				}

				if (event->is<sf::Event::KeyPressed>()) {
					auto key = event->getIf<sf::Event::KeyPressed>();
					if (key->code == sf::Keyboard::Key::Escape) {
						if (game.getStatus() == GameStatus::Playing) {
							game.setStatus(GameStatus::Menu);
						}
						else {
							game.setStatus(GameStatus::Playing);
						}
					}
				}

				if (game.getStatus() == GameStatus::Menu && event->is<sf::Event::MouseButtonPressed>()) {
					auto mouse = event->getIf<sf::Event::MouseButtonPressed>();

					if (mouse->button == sf::Mouse::Button::Left) {
						sf::Vector2f mousePosition(mouse->position);

						if (menu.getContinue().getGlobalBounds().contains(mousePosition)) {
							game.setStatus(GameStatus::Playing);
						}

						if (menu.getExit().getGlobalBounds().contains(mousePosition)) {
							window.close();
						}
					}
				}
			}

			float deltaTime = clock.restart().asSeconds();

			window.clear(sf::Color::White);

			if (game.isNewWave()) {
				game.showWave(window, font, windowSize);
				game.createWave();

				clock.restart();
				deltaTime = 0.f;
			}

			if (game.getStatus() == GameStatus::Playing) {
				game.update(deltaTime, windowSize);
			}

			if (game.getLives() == 0) {
				game.drawGameOver(window, font, windowSize);
			}

			else if (game.isEnemiesEmpty() && game.getLives() > 0 && game.getWave() == 11) {
				game.drawVictory(window, font, windowSize);
			}

			else {
				game.draw(window, font, windowSize);

				if (game.getStatus() == GameStatus::Menu) {
					menu.drawMenu(font, window, windowSize);
				}
			}
			
			window.display();
		}
	}

	catch (const std::exception& e) {
		std::cerr << e.what() << "\n";
	}

	return 0;
}