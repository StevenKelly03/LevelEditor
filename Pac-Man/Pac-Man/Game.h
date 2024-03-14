#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	void setupGame();
	void checkCollisions();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_gameTitle; // text used for message on screen
	sf::Text gameOverMessage;
	sf::Text scoreMessage;
	sf::Text victoryMessage;
	sf::CircleShape player;
	sf::ConvexShape mouth;
	sf::CircleShape dotArray[30];
	sf::CircleShape powerUp;
	sf::RectangleShape topBoundary;
	sf::RectangleShape bottomBoundary;
	sf::RectangleShape enemy;
	sf::Vector2f playerPos{ 50, 250 };
	sf::Vector2f enemyPos{ 700,250 };
	sf::Vector2f dotPosition{ 0,275 };
	int enemyDirection = -1;
	int playerDirection = 1;
	int playerSpeed = 5;
	int enemySpeed = 3;
	int scoreCount = 0;
	int dotCount = 30;
	int levelCount = 1;

	bool arrayEmpty{ false };
	bool playerWon{ false };
	bool playerPoweredUp{false};
	bool enemyVulnerable = false;
	bool m_gameOver;
	bool m_exitGame; // control exiting game
};

#endif // !GAME_HPP
