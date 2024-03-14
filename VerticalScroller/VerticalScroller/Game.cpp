#include "Game.h"
#include <iostream>
#include <time.h>

Game::Game() :
    m_window{ sf::VideoMode{ 650U, 800U, 32U }, "Square-Race" },
    m_exitGame{ false }
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
        render();
    }
}

void Game::processEvents()
{
    sf::Event newEvent;
    while (m_window.pollEvent(newEvent))
    {
        if (sf::Event::Closed == newEvent.type)
        {
            m_exitGame = true;
        }
        if (sf::Event::KeyPressed == newEvent.type)
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
    if (sf::Keyboard::Left == t_event.key.code && playerWon == false && gameOver == false)
    {
        player.move(-8, 0);
    }
    if (sf::Keyboard::Right == t_event.key.code && playerWon == false && gameOver == false)
    {
        player.move(8, 0);
    }
    if (sf::Keyboard::Up == t_event.key.code && playerWon == false && gameOver == false)
    {
        player.move(0, -8);
    }
    if (sf::Keyboard::Down == t_event.key.code && playerWon == false && gameOver == false)
    {
        player.move(0, 8);
    }
    if (sf::Keyboard::Space == t_event.key.code && playerWon == false && gameOver == false)
    {
        for (int i = 0; i < 10; i++)
        {
            playerBullets[i].setPosition(player.getPosition());
        }

        fire();
    }
    if (sf::Keyboard::Enter == t_event.key.code)
    {
        resetGame();
    }
}

void Game::update(sf::Time t_deltaTime)
{
    if (m_exitGame)
    {
        m_window.close();
    }

    sf::Vector2f playerPosition = player.getPosition();
    sf::Vector2f enemyPosition = enemy.getPosition();
    sf::Vector2f direction = playerPosition - enemyPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance > 0)
    {
        direction /= distance;

        if (playerWon == false && gameOver == false && enemyAlive == true)
        {
            enemy.move(direction);
        }
    }

    static sf::Clock enemyFireClock;
    if (enemyFireClock.getElapsedTime().asSeconds() >= 1.5f && playerWon == false && gameOver == false)
    {
        enemyFire();
        enemyFireClock.restart();
    }

    for (int i = 0; i < 10; i++)
    {
        if (isBulletActive(i))
        {
            playerBullets[i].move(0, -8);
            if (playerBullets[i].getPosition().y < 0)
            {
                playerBulletsActive[i] = false;
            }
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (enemyBulletsActive[i])
        {
            enemyBullets[i].move(0, 8);
        }
    }

    if (healthCount == 0)
    {
        gameOver = true;
    }

    if (enemyHealth == 0)
    {
        enemyAlive = false;

        enemy.setPosition(-100, -100);
    }

    if (player.getPosition().y <= -20)
    {
        playerWon = true;
    }

    healthMessage.setString("Player Health: " + std::to_string(healthCount));
    enemyHealthMessage.setString("Enemy Health: " + std::to_string(enemyHealth));

    if (playerWon == false && gameOver == false)
    {
        scrollObstacles();
    }

    checkCollisions();
}

void Game::render()
{
    m_window.clear(sf::Color::Black);
    m_window.draw(player);

    for (int i = 0; i < 10; i++)
    {
        if (enemyBulletsActive[i])
        {
            m_window.draw(enemyBullets[i]);
        }
    }

    for (int i = 0; i < 100; i++)
    {
        m_window.draw(obstacles[i]);
    }

    if (enemyAlive == true)
    {
        m_window.draw(enemy);
    }

    for (int i = 0; i < 10; i++)
    {
        if (playerBulletsActive[i])
        {
            m_window.draw(playerBullets[i]);
        }
    }

    m_window.draw(textBackground);
    m_window.draw(healthMessage);

    m_window.draw(enemyTextBackground);
    m_window.draw(enemyHealthMessage);

    if (gameOver == true)
    {
        m_window.draw(gameOverMessage);
    }

    if (playerWon == true)
    {
        m_window.draw(playerWonMessage);
    }

    m_window.display();
}


void Game::setupGame()
{
    srand(time(NULL));

    player.setRadius(7.5);
    player.setPosition(325, 750);
    player.setFillColor(sf::Color::Green);

    enemy.setRadius(7.5);
    enemy.setPosition(rand() % 600 + 1, 50);
    enemy.setFillColor(sf::Color::Red);

    if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
    {
        std::cout << "problem loading arial black font" << std::endl;
    }
    healthMessage.setFont(m_ArialBlackfont);
    healthMessage.setFillColor(sf::Color::Green);
    healthMessage.setOutlineColor(sf::Color::White);
    healthMessage.setOutlineThickness(1.0f);
    healthMessage.setCharacterSize(25u);
    healthMessage.setPosition(15, 740);

    textBackground.setSize(sf::Vector2f{ 250, 60 });
    textBackground.setPosition(1, 729);
    textBackground.setFillColor(sf::Color::Black);
    textBackground.setOutlineColor(sf::Color::White);
    textBackground.setOutlineThickness(1.0f);

    gameOverMessage.setFont(m_ArialBlackfont);
    gameOverMessage.setString("         Game Over\nBetter Luck Next Time!\n\n Press Enter to restart");
    gameOverMessage.setFillColor(sf::Color::Red);
    gameOverMessage.setOutlineColor(sf::Color::White);
    gameOverMessage.setOutlineThickness(1.0f);
    gameOverMessage.setCharacterSize(39u);
    gameOverMessage.setPosition(75, 210);

    playerWonMessage.setFont(m_ArialBlackfont);
    playerWonMessage.setString("    Congratulations!\n  Care To Try Again?\n\n Press Enter to restart");
    playerWonMessage.setFillColor(sf::Color::Green);
    playerWonMessage.setOutlineColor(sf::Color::White);
    playerWonMessage.setOutlineThickness(1.0f);
    playerWonMessage.setCharacterSize(40u);
    playerWonMessage.setPosition(85, 210);

    enemyHealthMessage.setFont(m_ArialBlackfont);
    enemyHealthMessage.setFillColor(sf::Color::Red);
    enemyHealthMessage.setOutlineColor(sf::Color::White);
    enemyHealthMessage.setOutlineThickness(1.0f);
    enemyHealthMessage.setCharacterSize(24u);
    enemyHealthMessage.setPosition(15, 15);

    enemyTextBackground.setSize(sf::Vector2f{ 250, 60 });
    enemyTextBackground.setPosition(1, 2);
    enemyTextBackground.setFillColor(sf::Color::Black);
    enemyTextBackground.setOutlineColor(sf::Color::White);
    enemyTextBackground.setOutlineThickness(1.0f);

    for (int i = 0; i < 10; i++)
    {
        playerBullets[i].setSize(bulletSize);
        playerBullets[i].setFillColor(sf::Color::Yellow);
        playerBullets[i].setPosition(-100, -100);
    }

    for (int i = 0; i < 10; i++)
    {
        enemyBullets[i].setSize(bulletSize);
        enemyBullets[i].setFillColor(sf::Color::White);
        enemyBullets[i].setPosition(-100, -100);
        enemyBulletsActive[i] = false;
    }

    const int gridWidth = 13;
    const int gridHeight = 16;
    const float cellWidth = 50.0f;
    const float cellHeight = 50.0f;

    for (int i = 0; i < 100; i++)
    {
        int cellX = rand() % gridWidth;
        int cellY = rand() % (gridHeight * 2) - gridHeight;

        obstaclePositions[i].x = cellX * cellWidth;
        obstaclePositions[i].y = cellY * cellHeight;

        obstacles[i].setOutlineColor(sf::Color::White);
        obstacles[i].setOutlineThickness(1.0f);
        obstacles[i].setFillColor(sf::Color::Transparent);
        obstacles[i].setSize(obstacleSize);
        obstacles[i].setPosition(obstaclePositions[i]);
    }
}

void Game::checkCollisions()
{
    if (player.getPosition().x <= 0)
    {
        player.setPosition(1, 750);
    }
    else if (player.getPosition().x >= 600)
    {
        player.setPosition(599, 750);
    }

    if (player.getGlobalBounds().intersects(enemy.getGlobalBounds()))
    {
        healthCount = 0;
    }

    for (int i = 0; i < 100; i++)
    {
        if (player.getGlobalBounds().intersects(obstacles[i].getGlobalBounds()))
        {
            player.move(0, 20);
            healthCount--;
        }
    }

    for (int i = 0; i < 100; i++)
    {
        if (enemy.getGlobalBounds().intersects(obstacles[i].getGlobalBounds()))
        {
            enemy.move(0, -20);
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (playerBullets[i].getGlobalBounds().intersects(enemy.getGlobalBounds()))
        {
            playerBullets[i].setPosition(-50, -50);
            enemyHealth--;
        }
        else
        {
            for (int j = 0; j < 75; j++)
            {
                if (playerBullets[i].getGlobalBounds().intersects(obstacles[j].getGlobalBounds()))
                {
                    playerBullets[i].setPosition(-50, -50);
                }
            }
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (enemyBullets[i].getGlobalBounds().intersects(player.getGlobalBounds()))
        {
            enemyBullets[i].setPosition(-50, -50);
            healthCount--;
        }
        else
        {
            for (int a = 0; a < 75; a++)
            {
                if (enemyBullets[i].getGlobalBounds().intersects(obstacles[a].getGlobalBounds()))
                {
                    enemyBullets[i].setPosition(-50, -50);
                }
            }
        }
    }

    if (player.getGlobalBounds().intersects(textBackground.getGlobalBounds()))
    {
        player.move(20, 0);
    }

    if (player.getGlobalBounds().intersects(enemyTextBackground.getGlobalBounds()))
    {
        player.move(20, 20);
    }
}

void Game::scrollObstacles()
{
    const float scrollSpeed = 0.5;

    for (int i = 0; i < 100; i++)
    {
        sf::FloatRect obstacleBounds = obstacles[i].getGlobalBounds();

        obstacles[i].setPosition(obstacles[i].getPosition().x, obstacles[i].getPosition().y + scrollSpeed);

        if (obstacleBounds.top + obstacleBounds.height > m_window.getSize().y)
        {
            obstacles[i].setPosition(obstaclePositions[i].x, -10);
        }
    }
}

void Game::fire()
{
    static int bulletIndex = 0;
    if (bulletIndex >= 10)
        return;

    playerBulletPositions[bulletIndex] = player.getPosition();
    playerBullets[bulletIndex].setPosition(playerBulletPositions[bulletIndex]);
    activateBullet(bulletIndex);
    playerBullets[bulletIndex].move(0, -8);

    bulletIndex++;
}


void Game::enemyFire()
{
    for (int i = 0; i < 10; i++)
    {
        if (!enemyBulletsActive[i])
        {
            enemyBulletPositions[i] = enemy.getPosition();
            enemyBullets[i].setPosition(enemyBulletPositions[i]);
            enemyBulletsActive[i] = true;
            break;
        }
    }
}

bool Game::isBulletActive(int index)
{
    return playerBulletsActive[index];
}

void Game::activateBullet(int index)
{
    playerBulletsActive[index] = true;
}

void Game::resetGame()
{
    healthCount = 3;
    enemyHealth = 3;
    gameOver = false;
    playerWon = false;
    enemyAlive = true;

    player.setPosition(325, 750);
    enemy.setPosition(rand() % 600 + 1, 50);

    for (int i = 0; i < 10; i++)
    {
        playerBulletsActive[i] = false;
        playerBullets[i].setPosition(-100, -100);
    }

    for (int i = 0; i < 10; i++)
    {
        enemyBulletsActive[i] = false;
        enemyBullets[i].setPosition(-100, -100);
    }

    for (int i = 0; i < 100; i++)
    {
        obstacles[i].setPosition(obstaclePositions[i]);
    }
}

