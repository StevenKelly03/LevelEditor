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
	void scrollObstacles();
	void fire();
	void enemyFire();
	bool isBulletActive(int index);
	void activateBullet(int index);
	void resetGame();

	sf::RenderWindow m_window;
	sf::Font m_ArialBlackfont; 
	sf::CircleShape player;
	sf::CircleShape enemy;
	sf::RectangleShape playerBullets[10];
	sf::RectangleShape enemyBullets[100];
	bool playerBulletsActive[100];
	bool enemyBulletsActive[10];
	sf::Vector2f playerBulletPositions[10];
	sf::Vector2f enemyBulletPositions[100];
	sf::RectangleShape obstacles[100];
	sf::Vector2f obstacleSize{ 20,10 };
	sf::Vector2f obstaclePositions[100];
	sf::Vector2f bulletSize{ 5, 5 };
	sf::Text healthMessage;
	sf::Text enemyHealthMessage;
	sf::Text gameOverMessage;
	sf::Text playerWonMessage;
	sf::RectangleShape textBackground;
	sf::RectangleShape enemyTextBackground;
	int healthCount = 3;
	int enemyHealth = 5;

	bool gameOver{ false };
	bool playerWon{ false };
	bool enemyAlive{ true };
	bool m_exitGame; 

};

#endif

