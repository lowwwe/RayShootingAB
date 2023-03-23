/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		switch (newEvent.type)
		{
		case sf::Event::Closed:
			m_exitGame = true;
			break;
		case sf::Event::KeyPressed:
			processKeys(newEvent);
			break;
		case sf::Event::MouseButtonPressed:
			processMouseDown(newEvent);
			break;
		case sf::Event::MouseButtonReleased:
			processMouseUp(newEvent);
			break;
		case sf::Event::MouseMoved:
			processMouseMove(newEvent);
			break;
		case sf::Event::MouseWheelMoved:
			processMouseWheel(newEvent);
			break;
		default:
			break;
		}
		

	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

void Game::processMouseUp(sf::Event t_event)
{
	m_dragging = false;
}

void Game::processMouseDown(sf::Event t_event)
{
	sf::Vertex point;
	if (sf::Mouse::Button::Left == t_event.mouseButton.button)
	{
		m_lineStart.x = static_cast<float>(t_event.mouseButton.x);
		m_lineStart.y = static_cast<float>(t_event.mouseButton.y);
		m_lineEnd.x = static_cast<float>(t_event.mouseButton.x);
		m_lineEnd.y = static_cast<float>(t_event.mouseButton.y);
		m_line.clear();
		point.color = sf::Color::Black;
		point.position = m_lineStart;
		m_line.append(point);
		point.position = m_lineEnd;
		m_line.append(point);
		m_dragging = true;
	}

}

void Game::processMouseMove(sf::Event t_event)
{
	if(m_dragging)
	{
		m_lineEnd.x = static_cast<float>(t_event.mouseMove.x);
		m_lineEnd.y = static_cast<float>(t_event.mouseMove.y);
		m_line[1].position = m_lineEnd;
	}
}

void Game::processMouseWheel(sf::Event t_event)
{

}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	m_window.draw(m_instructionsMessage);
	m_window.draw(m_targetSprite);
	m_window.draw(m_line);
	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_instructionsMessage.setFont(m_ArialBlackfont);
	m_instructionsMessage.setString("left click and drag for shooting line\nright click to move target\nwheel to rotate");

	m_instructionsMessage.setPosition(40.0f, 40.0f);
	m_instructionsMessage.setCharacterSize(20U);
	m_instructionsMessage.setOutlineColor(sf::Color::Red);
	m_instructionsMessage.setFillColor(sf::Color::Black);
	m_instructionsMessage.setOutlineThickness(1.0f);

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_targetTexture.loadFromFile("ASSETS\\IMAGES\\target.jpg"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading logo" << std::endl;
	}
	m_targetSprite.setTexture(m_targetTexture);
	m_targetSprite.setPosition(400.0f, 280.0f);
	m_targetSprite.setScale(0.5f, 0.5f);
}
