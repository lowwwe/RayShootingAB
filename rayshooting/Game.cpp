/// <summary>
/// @author Peter Lowe
/// @date MArch 2023
///
/// change the if statement in processMouseUp method
/// to check out the different methods
/// 
/// uses y = mx + c
/// for line equation but has problems with vertical lines where m is infinite
/// 
/// a better solution would be to use
/// ax + by = c
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup the image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 1100U, 900U, 32U }, "SFML ray firing" },
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
	m_aiming = false;
	m_dragging = false;
	if (sf::Mouse::Button::Left == t_event.mouseButton.button)
	{
		m_ray = MyLine{ m_lineStart, m_lineEnd };
	

		/*
			choose one if statment
			don't rotate sprite with first two
			don't NOT rotate sprite with last one
		*/

		if (simpleRayCheck(m_ray, m_targetSprite))
		//if(orthogonalIntersectionCheck(m_ray, m_targetSprite))
		//if(rotatedIntersectionCheck(m_ray, m_targetSprite))
		{
			m_instructionsMessage.setOutlineColor(sf::Color::Red);
		}
		else
		{
			m_instructionsMessage.setOutlineColor(sf::Color::Green);
		}
	}
}

void Game::processMouseDown(sf::Event t_event)
{
	sf::Vertex point;
	m_lineStart.x = static_cast<float>(t_event.mouseButton.x);
	m_lineStart.y = static_cast<float>(t_event.mouseButton.y);
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
		m_aiming = true;
	}
	if (sf::Mouse::Button::Right == t_event.mouseButton.button)
	{
		if (m_targetSprite.getGlobalBounds().contains(m_lineStart))
		{
			m_dragOffset = m_targetSprite.getPosition() - m_lineStart;
			m_dragging = true;
		}
	}

}

void Game::processMouseMove(sf::Event t_event)
{
	if(m_aiming)
	{
		m_lineEnd.x = static_cast<float>(t_event.mouseMove.x);
		m_lineEnd.y = static_cast<float>(t_event.mouseMove.y);
		m_line[1].position = m_lineEnd;
	}
	if (m_dragging)
	{
		m_targetSprite.setPosition(static_cast<float>(t_event.mouseMove.x) + m_dragOffset.x,
			static_cast<float>(t_event.mouseMove.y + m_dragOffset.y));
	}
}

void Game::processMouseWheel(sf::Event t_event)
{
	if (t_event.mouseWheel.delta > 0)
	{
		m_targetSprite.setRotation(m_targetSprite.getRotation() + 2.0f);
	}
	else
	{
		m_targetSprite.setRotation(m_targetSprite.getRotation() - 2.0f);
	}
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
	m_window.draw(m_checkpoints);
	m_window.draw(m_intersection);
	m_window.display();
}


/// <summary>
/// take a number of samples inside the x range
/// and check if the point is inside the box
/// </summary>
/// <param name="t_line">ray cast line</param>
/// <param name="t_target">sprite target</param>
/// <returns>true if hit</returns>
bool Game::simpleRayCheck(MyLine t_line, sf::Sprite t_target)
{
	bool result = false;
	int steps{ 50 };
	sf::FloatRect target;
	target = t_target.getGlobalBounds();
	float startX = target.left;
	float endX = startX + target.width;
	float minY = target.top;
	float maxY = minY + target.height;
	float increment = target.width / steps; // precision
	float y;
	float x;

	sf::Vertex point;
	point.color = sf::Color::Red;
	m_checkpoints.clear();
	for (int i = 0; i < steps; i++)
	{
		x = startX + (i * increment);
		y = t_line.findY(x);
		point.position.x = x;
		point.position.y = y;
		m_checkpoints.append(point);
		if( y >= minY && y <= maxY)
		{
			result = true;
		}
	}

	return result;
}

/// <summary>
/// orthogonal check
/// 
/// don't shoot exactly vertically
/// </summary>
/// <param name="t_line">shooting line</param>
/// <param name="t_target">target sprite</param>
/// <returns>true if hit</returns>
bool Game::orthogonalIntersectionCheck(MyLine t_line, sf::Sprite t_target)
{
	bool result = false;
	sf::FloatRect target = t_target.getGlobalBounds();
	sf::Vector2f start;
	sf::Vector2f end;
	sf::Vector2f crossOver;
	
	start.x = target.left;
	start.y = target.top;
	end.x = start.x + target.width;
	end.y = start.y + target.height;
	crossOver.y = start.y;
	crossOver.x = t_line.findX(start.y);
	m_intersection.setPosition(crossOver);
	if (crossOver.x >= start.x && crossOver.x <= end.x) // top edge
	{
		return true;
	}
	
	crossOver.y = end.y;
	crossOver.x = t_line.findX(end.y);
	m_intersection.setPosition(crossOver);
	if (crossOver.x >= start.x && crossOver.x <= end.x) // bottom edge
	{
		return true;
	}
	
	start.y = target.top;
	crossOver = sf::Vector2f(end.x, end.x * t_line.slope + t_line.interceptY);
	m_intersection.setPosition(crossOver);
	if (crossOver.y >= start.y && crossOver.y <= end.y) // right edge
	{
		return true;
	}

	
	crossOver = sf::Vector2f(start.x, t_line.findY(start.x));
	m_intersection.setPosition(crossOver);
	if (crossOver.y >= start.y && crossOver.y <= end.y) // left edge
	{
		return true;
	}
	
	return result;
}


/// <summary>
/// check rotated sprite must be skew
/// only check 3 side, what goes in must come out
/// </summary>
/// <param name="t_line">ray cast line</param>
/// <param name="t_target">target sprite</param>
/// <returns>true if hit</returns>
bool Game::rotatedIntersectionCheck(MyLine t_line, sf::Sprite t_target)
{
	float angle = t_target.getRotation();
	float angleRad = angle / 180  * 3.14159265359f;
	bool result = false;
	sf::FloatRect target = t_target.getLocalBounds();
	sf::Vector2f start;
	sf::Vector2f end;
	sf::Vector2f crossOver;
	sf::Vector2f lastEnd;
	
	float hypothenus;
	
	start = t_target.getPosition();
	end.x = start.x + target.width * std::cos(angleRad);
	end.y = start.y + target.width * std::sin(angleRad);
	MyLine line{ start,end };
	crossOver = line.intersection(t_line);
	m_intersection.setPosition(crossOver);
	if (crossOver.x >= start.x && crossOver.x <= end.x) // top edge
	{
		return true;
	}

	
	hypothenus = std::sqrt(target.height * target.height + target.width * target.width);
	float diagonalAngle = std::atan2(target.height, target.width);

	end.x = start.x + target.height * std::cos(angleRad + M_PI_2);
	end.y = start.y + target.height * std::sin(angleRad + M_PI_2);
	line =  MyLine{ start,end };
	crossOver = line.intersection(t_line);
	m_intersection.setPosition(crossOver);
	if (crossOver.y >= start.y && crossOver.y <= end.y) // bottom edge
	{
		return true;
	}
	lastEnd = end;
	end.x = start.x + hypothenus * std::cos(angleRad + diagonalAngle);
	end.y = start.y + hypothenus * std::sin(angleRad + diagonalAngle);
	start = lastEnd;
	line = MyLine{ start,end };
	crossOver = line.intersection(t_line);
	m_intersection.setPosition(crossOver);
	if (crossOver.x >= start.x && crossOver.x <= end.x) // bottom edge
	{
		return true;
	}
	return false;
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
	m_instructionsMessage.setString("left click and drag for shooting line\nright click to move target\nwheel to rotate\n green for a miss , red for a hit");

	m_instructionsMessage.setPosition(40.0f, 40.0f);
	m_instructionsMessage.setCharacterSize(20U);
	m_instructionsMessage.setOutlineColor(sf::Color::Green);
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

	m_intersection.setFillColor(sf::Color::Red);
	m_intersection.setOrigin(5.0f, 5.0f);
}
