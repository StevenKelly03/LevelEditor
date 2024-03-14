#include "Game.h"
#include <iostream>

Game::Game() :
    m_window{ sf::VideoMode{ 1000U, 750U, 32U }, "Level Editor" },
    m_exitGame{ false },
    m_animationSpeed{ 1.0f },
    m_defaultFrameDuration{ sf::seconds(1.0f / 12.0f) },
    m_frameDuration{ m_defaultFrameDuration }
{
    initText();
    initGraphics();
}

Game::~Game()
{
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    const float fps{ 60.0f };
    sf::Time timePerFrame = sf::seconds(1.0f / fps);
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
        if (sf::Event::MouseButtonPressed == newEvent.type)
        {
            processMouseClicks();
        }
    }
}

void Game::processKeys(sf::Event t_event)
{
    if (sf::Keyboard::Escape == t_event.key.code)
    {
        m_exitGame = true;
    }

    if (sf::Keyboard::M == t_event.key.code && menuOpen == false && gameOver == false)
    {
        menuOpen = true;
    }
    else if (sf::Keyboard::M == t_event.key.code && menuOpen == true && gameOver == false)
    {
        menuOpen = false;
    }

    if (sf::Keyboard::C == t_event.key.code && controlDisplay == false && gameOver == false)
    {
        controlDisplay = true;
    }
    else if (sf::Keyboard::C == t_event.key.code && controlDisplay == true && gameOver == false)
    {
        controlDisplay = false;
    }

    if (sf::Keyboard::Right == t_event.key.code && gameOver == false)
    {
        m_playerSprite.setTexture(m_playerTexture);
        m_playerSprite.setTextureRect(sf::IntRect{ 0,408,72,64 });
        m_playerSprite.move(4.0, 0.0);
        if (!isJumping) 
        {
            animate();
        }
    }
    if (sf::Keyboard::Left == t_event.key.code && gameOver == false)
    {
        m_playerSprite.setTexture(m_playerTextureLeft);
        m_playerSprite.setTextureRect(sf::IntRect{ 1022, 0, 72, 64 });
        m_playerSprite.move(-4.0, 0.0);
        animateLeft();
    }
    if (sf::Keyboard::Space == t_event.key.code)
    {
        jump();
    }
    if (sf::Keyboard::F == t_event.key.code && gameOver == false)
    {
        fill();
    }
    if (sf::Keyboard::R == t_event.key.code)
    {
        gameOver = false;
        resetLevel();
        initGraphics();
    }
}

void Game::update(sf::Time t_deltaTime)
{
    if (m_exitGame)
    {
        m_window.close();
    }

    if (gameOver)
    {
        return;
    }

    if (isJumping)
    {
        jumpVelocity += gravity;
        m_playerSprite.move(0.0f, jumpVelocity);

        if (jumpVelocity >= 0) {
            m_playerSprite.setTexture(m_jumpTexture);
        }
    }

    if (m_playerSprite.getPosition().y >= 535)
    {
        m_playerSprite.setPosition(m_playerSprite.getPosition().x, 535);
        isJumping = false;
    }

    if (isJumping)
    {
        jumpVelocity += gravity;
        m_playerSprite.move(0.0f, jumpVelocity);

        if (jumpVelocity >= 0) {
            m_playerSprite.setTexture(m_jumpTexture);
        }
    }
    if (!isJumping)
    {
        m_playerSprite.setTexture(m_playerTexture);
    }



    checkCollisions();
}

void Game::render()
{
    m_window.clear(sf::Color::Black);
    m_window.draw(m_welcomeMessage);
    m_window.draw(controlMenuText);
    m_window.draw(m_textBorder);
    m_window.draw(m_playerSprite);

    if (controlDisplay == true)
    {
        m_window.draw(instructions);
    }

    if (menuOpen == true)
    {
        m_window.draw(m_lavaSprite);
        m_window.draw(m_grassSprite);
        m_window.draw(m_brickSprite);
        m_window.draw(m_questionSprite);
        m_window.draw(menuBorderRight);
        m_window.draw(menuBorderBottom);
    }

    m_window.draw(m_goalSprite);

    for (int i = 0; i < 60; i++)
    {
        m_window.draw(levelGrid[i]);
        m_window.draw(levelBlocks[i]);
    }
    for (int i = 0; i < 14; i++)
    {
        m_window.draw(stairGrid[i]);
        m_window.draw(stairBlocks[i]);
    }

    if (gameOver == true)
    {
        m_window.draw(gameOverMessage);
    }

    m_window.display();
}

void Game::initText()
{
    if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
    {
        std::cout << "problem loading arial black font" << std::endl;
    }
    m_welcomeMessage.setFont(m_ArialBlackfont);
    m_welcomeMessage.setString("Level Editor");
    m_welcomeMessage.setPosition(350.0f, 10.0f);
    m_welcomeMessage.setCharacterSize(45U);
    m_welcomeMessage.setOutlineColor(sf::Color::Cyan);
    m_welcomeMessage.setFillColor(sf::Color::Red);
    m_welcomeMessage.setOutlineThickness(2.0f);

    gameOverMessage.setFont(m_ArialBlackfont);
    gameOverMessage.setString("Congratulations!\n      You Won!!!");
    gameOverMessage.setPosition(200, 250);
    gameOverMessage.setCharacterSize(65U);
    gameOverMessage.setFillColor(sf::Color::Green);
    gameOverMessage.setOutlineColor(sf::Color::White);
    gameOverMessage.setOutlineThickness(2.0f);

    instructions.setFont(m_ArialBlackfont);
    instructions.setString("                       Controls:\n-Use the M key to open and close the tile menu\n\n-Click the tile you would like to place, and after you\nhave selected it, close the menu and click the area\nof the grid where you would like to place it\n\n-Use the F key to auto-fill level with the\n current selected block\n\n-Use the R key to empty the level grid, removing\nall previously placed blocks");
    instructions.setPosition(250, 100.0f);
    instructions.setCharacterSize(20U);
    instructions.setFillColor(sf::Color::White);

    controlMenuText.setFont(m_ArialBlackfont);
    controlMenuText.setString("Press the C key to view game controls");
    controlMenuText.setPosition(675.0f, 20.0f);
    controlMenuText.setCharacterSize(15U);
    controlMenuText.setFillColor(sf::Color::White);
}

void Game::initGraphics()
{
    if (!m_lavaTexture.loadFromFile("C:\\Motion Graphics\\LevelEditor\\assets\\LavaBlock.jpg"))
    {
        std::cout << "Error Loading Lava Block";
    }
    m_lavaSprite.setTexture(m_lavaTexture);
    m_lavaSprite.setPosition(20, 100);

    if (!m_grassTexture.loadFromFile("C:\\Motion Graphics\\LevelEditor\\assets\\GrassBlock.jpg"))
    {
        std::cout << "Error Loading Grass Block";
    }
    m_grassSprite.setTexture(m_grassTexture);
    m_grassSprite.setPosition(20, 175);

    if (!m_bricksTexture.loadFromFile("C:\\Motion Graphics\\LevelEditor\\assets\\Bricks.jpg"))
    {
        std::cout << "Error Loading Brick Block";
    }
    m_brickSprite.setTexture(m_bricksTexture);
    m_brickSprite.setPosition(120, 100);

    if (!m_questionTexture.loadFromFile("C:\\Motion Graphics\\LevelEditor\\assets\\QuestionBlock.png"))
    {
        std::cout << "Error Loading Question Block";
    }
    m_questionSprite.setTexture(m_questionTexture);
    m_questionSprite.setPosition(120, 175);

    if (!m_playerTexture.loadFromFile("C:\\Motion Graphics\\LevelEditor\\assets\\spritesheet.png"))
    {
        std::cout << "Error Loading player spritesheet";
    }

    if (!m_playerTextureLeft.loadFromFile("C:\\Motion Graphics\\LevelEditor\\assets\\spritesheetLeft.png"))
    {
        std::cout << "Error Loading left facing player spritesheet";
    }

    if (!m_jumpTexture.loadFromFile("C:\\Motion Graphics\\LevelEditor\\assets\\jump.png"))
    {
        std::cout << "Error Loading jumping player texture";
    }

    if (!m_goalTexture.loadFromFile("C:\\Motion Graphics\\LevelEditor\\assets\\goal.png"))
    {
        std::cout << "Error Loading end goal texture";
    }

    if (!m_blankTexture.loadFromFile("C:\\Motion Graphics\\LevelEditor\\assets\\blank.png"))
    {
        std::cout << "Error loading blank texture" << std::endl;
    }

    m_goalSprite.setTexture(m_goalTexture);
    m_goalSprite.setScale(0.35, 0.45);
    m_goalSprite.setPosition(925, 155);

    m_playerSprite.setTexture(m_playerTexture);
    m_playerSprite.setPosition(0, 535);
    m_playerSprite.setTextureRect(sf::IntRect{ 0,408,72,64 });

    m_textBorder.setFillColor(sf::Color::Cyan);
    m_textBorder.setSize(sf::Vector2f{ 1000,2 });
    m_textBorder.setPosition(0.0f, 65.0f);

    menuBorderRight.setSize(sf::Vector2f{ 2,200 });
    menuBorderRight.setFillColor(sf::Color::Cyan);
    menuBorderRight.setPosition(198, 65);

    menuBorderBottom.setSize(sf::Vector2f{ 200,2 });
    menuBorderBottom.setFillColor(sf::Color::Cyan);
    menuBorderBottom.setPosition(0, 265);

    int step = 0;
    int a = 0;

    for (int i = 0; i < 60; i++)
    {
        levelGrid[i].setOutlineColor(sf::Color::White);
        levelGrid[i].setOutlineThickness(0.75f);
        levelGrid[i].setFillColor(sf::Color::Transparent);
        levelGrid[i].setSize(sf::Vector2f{ 49,50 });

        if (i < 20)
        {
            levelGrid[i].setPosition(1 + (i * 50), 599);
            levelBlocks[i].setPosition(1 + (i * 50), 599);
        }
        else if (i >= 20 && i < 40)
        {
            levelGrid[i].setPosition(1 + (step * 50), 649);
            levelBlocks[i].setPosition(1 + (step * 50), 649);
            step++;
        }
        else if (i >= 40 && i < 60)
        {
            levelGrid[i].setPosition(1 + (a * 50), 699);
            levelBlocks[i].setPosition(1 + (a * 50), 699);
            a++;
        }
    }

    int b = 0;
    int c = 0;
    int d = 0;

    for (int i = 0; i < 14; i++)
    {
        stairGrid[i].setOutlineColor(sf::Color::White);
        stairGrid[i].setFillColor(sf::Color::Transparent);
        stairGrid[i].setOutlineThickness(0.75f);
        stairGrid[i].setSize(sf::Vector2f{ 50,50 });

        if (i < 5)
        {
            stairGrid[i].setPosition(750 + (i * 50), 550);
            stairBlocks[i].setPosition(750 + (i * 50), 550);
        }
        else if (i >= 5 && i < 9)
        {
            stairGrid[i].setPosition(800 + (b * 50), 500);
            stairBlocks[i].setPosition(800 + (b * 50), 500);
            b++;
        }
        else if (i >= 9 && i < 12)
        {
            stairGrid[i].setPosition(850 + (c * 50), 450);
            stairBlocks[i].setPosition(850 + (c * 50), 450);
            c++;
        }
        else if (i >= 12 && i < 14)
        {
            stairGrid[i].setPosition(900 + (d * 50), 400);
            stairBlocks[i].setPosition(900 + (d * 50), 400);
            d++;
        }
    }
}

void Game::animate()
{
    elapsed += m_frameDuration.asSeconds();
    while (elapsed >= m_frameDuration.asSeconds())
    {
        elapsed -= m_frameDuration.asSeconds();
        currentFrame = (currentFrame + 1) % frameCount;
    }

    int column = currentFrame % 12;
    int row = 0;
    int totalFrames = 0;
    for (int i = 0; i < rowFrameCount.size(); ++i)
    {
        totalFrames += rowFrameCount[i];
        if (currentFrame < totalFrames)
        {
            row = i;
            break;
        }
    }

    m_playerSprite.setTextureRect(sf::IntRect(column * frameSize.x, row * frameSize.y, frameSize.x, frameSize.y));
}

void Game::processMouseClicks()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        mousePosition = sf::Mouse::getPosition(m_window);

        sf::Vector2i mousePosInt(mousePosition);

        if (m_lavaSprite.getGlobalBounds().contains(mousePosInt.x, mousePosInt.y) && menuOpen == true)
        {
            m_selection = SelectedBlock::LAVA;
            std::cout << "\n\nLava Block Selected";
        }
        else if (m_grassSprite.getGlobalBounds().contains(mousePosInt.x, mousePosInt.y) && menuOpen == true)
        {
            m_selection = SelectedBlock::GRASS;
            std::cout << "\n\nGrass Block Selected";
        }
        else if (m_brickSprite.getGlobalBounds().contains(mousePosInt.x, mousePosInt.y) && menuOpen == true)
        {
            m_selection = SelectedBlock::BRICKS;
            std::cout << "\n\nBrick Block Selected";
        }
        else if (m_questionSprite.getGlobalBounds().contains(mousePosInt.x, mousePosInt.y) && menuOpen == true)
        {
            m_selection = SelectedBlock::QUESTION;
            std::cout << "\n\nQuestion Block Selected";
        }
    }

    for (int i = 0; i < 60; i++)
    {
        sf::Vector2i gridPosInt(levelGrid[i].getPosition());

        if (m_selection == SelectedBlock::LAVA && levelGrid[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            levelBlocks[i].setTexture(m_lavaTexture);
        }
        else if (m_selection == SelectedBlock::GRASS && levelGrid[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            levelBlocks[i].setTexture(m_grassTexture);
        }
        else if (m_selection == SelectedBlock::BRICKS && levelGrid[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            levelBlocks[i].setTexture(m_bricksTexture);
        }
        else if (m_selection == SelectedBlock::QUESTION && levelGrid[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            levelBlocks[i].setTexture(m_questionTexture);
        }
    }
    for (int i = 0; i < 14; i++)
    {
        sf::Vector2i stairPosInt(stairGrid[i].getPosition());

        if (m_selection == SelectedBlock::LAVA && stairGrid[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            stairBlocks[i].setTexture(m_lavaTexture);
        }
        else if (m_selection == SelectedBlock::GRASS && stairGrid[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            stairBlocks[i].setTexture(m_grassTexture);
        }
        else if (m_selection == SelectedBlock::BRICKS && stairGrid[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            stairBlocks[i].setTexture(m_bricksTexture);
        }
        else if (m_selection == SelectedBlock::QUESTION && stairGrid[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            stairBlocks[i].setTexture(m_questionTexture);
        }
    }
}

void Game::checkCollisions()
{
    for (int i = 0; i < 14; i++)
    {
        if (m_playerSprite.getGlobalBounds().intersects(stairGrid[i].getGlobalBounds()))
        {
            m_playerSprite.setPosition(m_playerSprite.getPosition().x, stairGrid[i].getPosition().y - m_playerSprite.getGlobalBounds().height);
            jumpVelocity = 0.0f;
            isJumping = false;
        }
    }

    if (m_playerSprite.getGlobalBounds().intersects(m_goalSprite.getGlobalBounds()))
    {
        gameOver = true;
    }

}

void Game::jump()
{
    if (!isJumping)
    {
        isJumping = true;
        jumpVelocity = -15.0f;
    }
}

void Game::fill()
{
    for (int i = 0; i < 60; i++)
    {
        if (m_selection == SelectedBlock::LAVA)
        {
            levelBlocks[i].setTexture(m_lavaTexture);
        }
        else if (m_selection == SelectedBlock::GRASS)
        {
            levelBlocks[i].setTexture(m_grassTexture);
        }
        else if (m_selection == SelectedBlock::QUESTION)
        {
            levelBlocks[i].setTexture(m_questionTexture);
        }
        else if (m_selection == SelectedBlock::BRICKS)
        {
            levelBlocks[i].setTexture(m_bricksTexture);
        }
    }

    for (int i = 0; i < 14; i++)
    {
        if (m_selection == SelectedBlock::LAVA)
        {
            stairBlocks[i].setTexture(m_lavaTexture);
        }
        else if (m_selection == SelectedBlock::GRASS)
        {
            stairBlocks[i].setTexture(m_grassTexture);
        }
        else if (m_selection == SelectedBlock::QUESTION)
        {
            stairBlocks[i].setTexture(m_questionTexture);
        }
        else if (m_selection == SelectedBlock::BRICKS)
        {
            stairBlocks[i].setTexture(m_bricksTexture);
        }
    }

    if (m_selection == SelectedBlock::LAVA)
    {
        std::cout << "\nGrid filled with Lava Block" << std::endl;
    }
    else if (m_selection == SelectedBlock::GRASS)
    {
        std::cout << "\nGrid filled with Grass Block" << std::endl;
    }
    else if (m_selection == SelectedBlock::QUESTION)
    {
        std::cout << "\nGrid filled with Question Block" << std::endl;
    }
    else if (m_selection == SelectedBlock::BRICKS)
    {
        std::cout << "\nGrid filled with Brick Block" << std::endl;
    }
}

void Game::animateLeft()
{
    elapsed += m_frameDuration.asSeconds();
    while (elapsed >= m_frameDuration.asSeconds())
    {
        elapsed -= m_frameDuration.asSeconds();
        currentLeftFrame = (currentLeftFrame - 1 + frameCount) % frameCount;

        if (currentLeftFrame < 0)
        {
            currentLeftFrame = frameCount - 1;
        }
    }

    int column = currentLeftFrame % 12;
    int row = 0;
    int totalFrames = 0;
    for (int i = 0; i < rowFrameCount.size(); ++i)
    {
        totalFrames += rowFrameCount[i];
        if (currentLeftFrame < totalFrames)
        {
            row = i;
            break;
        }
    }

    m_playerSprite.setTextureRect(sf::IntRect(column * leftFrameSize.x, row * leftFrameSize.y, leftFrameSize.x, leftFrameSize.y));
}

void Game::resetLevel()
{
    for (int i = 0; i < 60; i++)
    {
        levelBlocks[i].setTexture(m_blankTexture);
    }
    for (int i = 0; i < 14; i++)
    {
        stairBlocks[i].setTexture(m_blankTexture);
    }

    std::cout << "\nLevel grid emptied, all blocks removed" << std::endl;
}