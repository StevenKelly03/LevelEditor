#include "Game.h"
#include <iostream>

Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "Pac-Man Project" },
	m_exitGame{false}
{
	setupGame();
}

Game::~Game()
{
}

void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update(timePerFrame);
		}

		render();  // Ensure render is called here.
	}
}

void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}

void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}

	if (sf::Keyboard::Space == t_event.key.code)
	{
		playerDirection = -playerDirection;  // Change direction immediately

		if (playerDirection < 0)
		{
			mouth.setPoint(1, sf::Vector2f(-12.5, 25 * std::tan(37.5 * 3.14159265 / 180)));
			mouth.setPoint(2, sf::Vector2f(-12.5, -25 * std::tan(37.5 * 3.14159265 / 180)));
		}
		else
		{
			mouth.setPoint(1, sf::Vector2f(50, 25 * std::tan(37.5 * 3.14159265 / 180)));
			mouth.setPoint(2, sf::Vector2f(50, -25 * std::tan(37.5 * 3.14159265 / 180)));
		}
	}
}


void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	playerPos.x += (playerSpeed * playerDirection);
    if (playerPos.x > 800)
    {
        playerPos.x = -50;
    }
    else if (playerPos.x < -50)
    {
        playerPos.x = 800;
    }

	enemyPos.x += (enemyDirection * enemySpeed);
	if (enemyPos.x <= 0)
	{
		enemyDirection = enemyDirection * -1;
	}
	else if (enemyPos.x >= 750)
	{
		enemyDirection = enemyDirection * -1;
	}

	if (arrayEmpty == true && playerPoweredUp == false)
	{
		enemySpeed += 1;
		playerSpeed += 1;
		levelCount++;

		dotPosition.x = 0;
		dotPosition.y = 275;

		for (int i = 0; i < 30; i++)
		{
			dotArray[i].setPosition((dotPosition.x + (28 * i)), dotPosition.y);
		}

		dotCount = 30;
		arrayEmpty = false;
	}
	
	if (playerWon == true)
	{
		enemySpeed = 0;
		enemy.setFillColor(sf::Color::Cyan);
		enemy.setPosition(-200, -200);
	}

	if (playerPoweredUp == true)
	{
		player.setFillColor(sf::Color::Red);
		enemy.setFillColor(sf::Color::Cyan);
		arrayEmpty == true;
		dotCount = 0;
	}

	checkCollisions();
	enemy.setPosition(enemyPos);
	player.setPosition(playerPos);
	mouth.setPosition(playerPos.x, playerPos.y + 25);
}

void Game::render()
{
	m_window.clear(sf::Color::Black);

	for (int i = 0; i < 30; i++)
	{
		m_window.draw(dotArray[i]);
	}

	m_window.draw(m_gameTitle);
	m_window.draw(player);
	m_window.draw(mouth);


	if (levelCount == 10)
	{
		m_window.draw(powerUp);
	}
	
	

	if (playerWon == false)
	{
		m_window.draw(enemy);
	}
	else if (playerWon == true)
	{
		m_window.draw(victoryMessage);
	}
		
	
	m_window.draw(topBoundary);
	m_window.draw(bottomBoundary);

	if (m_gameOver == true && playerPoweredUp == false)
	{
		m_window.draw(gameOverMessage);
	}

	scoreMessage.setString("Score: " + std::to_string(scoreCount));
	m_window.draw(scoreMessage);
	m_window.display();
}

void Game::setupGame()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_gameTitle.setFont(m_ArialBlackfont);
	m_gameTitle.setString("Pac-Man");
	m_gameTitle.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_gameTitle.setPosition(220.0f, 40.0f);
	m_gameTitle.setCharacterSize(80U);
	m_gameTitle.setOutlineColor(sf::Color::White);
	m_gameTitle.setFillColor(sf::Color::Yellow);
	m_gameTitle.setOutlineThickness(3.0f);

	gameOverMessage.setFont(m_ArialBlackfont);
	gameOverMessage.setString("Game Over");
	gameOverMessage.setStyle(sf::Text::Bold);
	gameOverMessage.setPosition(165.0f, 225.0f);
	gameOverMessage.setCharacterSize(80U);
	gameOverMessage.setOutlineColor(sf::Color::White);
	gameOverMessage.setFillColor(sf::Color::Red);
	gameOverMessage.setOutlineThickness(3.0f);

	scoreMessage.setFont(m_ArialBlackfont);
	scoreMessage.setStyle(sf::Text::Bold);
	scoreMessage.setPosition(350, 400.0f);
	scoreMessage.setCharacterSize(30U);
	scoreMessage.setFillColor(sf::Color::White);

	victoryMessage.setFont(m_ArialBlackfont);
	victoryMessage.setString("Congratulations, You Won!");
	victoryMessage.setStyle(sf::Text::Bold);
	victoryMessage.setPosition(40, 250);
	victoryMessage.setCharacterSize(50U);
	victoryMessage.setFillColor(sf::Color::Green);
	victoryMessage.setOutlineColor(sf::Color::White);
	victoryMessage.setOutlineThickness(2.0f);


	for (int i = 0; i < 30; i++)
	{
		dotArray[i].setFillColor(sf::Color::White);
		dotArray[i].setRadius(3);
		dotArray[i].setPosition((dotPosition.x + (28 * i)), dotPosition.y);
	}

	player.setRadius(25);
	player.setPosition(playerPos);
	player.setFillColor(sf::Color::Yellow);

	powerUp.setRadius(10);
	powerUp.setPosition(400, 265);
	powerUp.setFillColor(sf::Color::Red);

	mouth.setPointCount(3);
	mouth.setPoint(0, sf::Vector2f(25, 0));
	mouth.setPoint(1, sf::Vector2f(50, -25 * std::tan(37.5 * 3.14159265 / 180)));
	mouth.setPoint(2, sf::Vector2f(50, 25 * std::tan(37.5 * 3.14159265 / 180)));
	mouth.setPosition(playerPos.x, playerPos.y);
	mouth.setFillColor(sf::Color::Black);

	sf::Vector2f enemySize{ 50, 50 };
	enemy.setSize(enemySize);
	enemy.setPosition(enemyPos);
	enemy.setFillColor(sf::Color::Magenta);

	sf::Vector2f boundarySize{ 800, 5 };
	topBoundary.setSize(boundarySize);
	topBoundary.setPosition(0, 200);
	topBoundary.setFillColor(sf::Color::Blue);

	bottomBoundary.setSize(boundarySize);
	bottomBoundary.setPosition(0, 350);
	bottomBoundary.setFillColor(sf::Color::Blue);
}

void Game::checkCollisions()
{
	if ((player.getGlobalBounds()).intersects(enemy.getGlobalBounds()))
	{
		if (playerPoweredUp == false)
		{
			enemySpeed = 0;
			playerSpeed = 0;

			m_gameOver = true;
		}
		else
		{
			playerSpeed = 0;
			playerWon = true;
		}
			
	}


	for (int i = 0; i < 30; i++)
	{
		if ((player.getGlobalBounds()).intersects(dotArray[i].getGlobalBounds()))
		{
			dotArray[i].setPosition(-50, -50);

			scoreCount += 10;

			dotCount--;
			if (dotCount == 0)
			{
				arrayEmpty = true;
			}
		}
	}

	if ((player.getGlobalBounds()).intersects(powerUp.getGlobalBounds()) && levelCount == 10)
	{
		playerPoweredUp = true;
		powerUp.setPosition(-100, -100);
	}
}