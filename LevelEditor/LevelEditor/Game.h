/// <summary>
/// author Pete Lowe May 2019
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>

enum class SelectedBlock
{
	LAVA,
	GRASS,
	QUESTION,
	BRICKS,
	DEFAULT
};

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	
	void initText();
	void initGraphics();
	void animate();
	void animateLeft();
	void processMouseClicks();
	void checkCollisions();
	void jump();
	void fill();
	void resetLevel();

	bool isJumping = false; // Flag to track if the player is currently jumping
	float jumpVelocity = 0.0f; // Initial jump velocity
	const float gravity = 0.3f; // Gravity value

	int frameCount{ 8 };
	int currentFrame{ 0 };
	int currentLeftFrame{ 8 };
	sf::Vector2f frameSize{ 80,66 };
	sf::Vector2f leftFrameSize{ 72,66 };
	float m_animationSpeed;
	sf::Time m_defaultFrameDuration;
	sf::Time m_frameDuration;
	float elapsed = 0.0f;

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen

	sf::RectangleShape m_textBorder;

	sf::RectangleShape levelGrid[60];
	sf::Sprite levelBlocks[60];

	sf::RectangleShape stairGrid[14];
	sf::Sprite stairBlocks[14];

	sf::RectangleShape menuBorderRight;
	sf::RectangleShape menuBorderBottom;

	sf::Text instructions;
	sf::Text controlMenuText;

	sf::Texture m_lavaTexture;
	sf::Texture m_grassTexture;
	sf::Texture m_bricksTexture;
	sf::Texture m_questionTexture;
	sf::Texture m_playerTexture;
	sf::Texture m_playerTextureLeft;
	sf::Texture m_jumpTexture;
	sf::Texture m_goalTexture;
	sf::Texture m_blankTexture;

	sf::Sprite m_lavaSprite;
	sf::Sprite m_grassSprite;
	sf::Sprite m_brickSprite;
	sf::Sprite m_questionSprite;
	sf::Sprite m_playerSprite;
	sf::Sprite m_goalSprite;

	std::vector<int> rowFrameCount = { 10, 12, 11, 12, 11, 12, 12, 10,  8, 12 , 10, 7};
	sf::Clock frameClock;

	SelectedBlock m_selection{ SelectedBlock::DEFAULT };
	sf::Vector2i mousePosition;

	sf::Text gameOverMessage;

	bool menuOpen{ false };
	bool controlDisplay{ false };
	bool gameOver{ false };
	bool m_exitGame; // control exiting game
};

#endif // !GAME_HPP

