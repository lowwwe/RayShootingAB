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
#include "MyLine.h"

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
	void processMouseUp(sf::Event t_event);
	void processMouseDown(sf::Event t_event);
	void processMouseMove(sf::Event t_event);
	void processMouseWheel(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	bool simpleRayCheck(MyLine t_line, sf::Sprite t_target);
	bool orthogonalIntersectionCheck(MyLine t_line, sf::Sprite t_target);
	bool rotatedIntersectionCheck(MyLine t_line, sf::Sprite t_target);
	void setupFontAndText();
	void setupSprite();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_instructionsMessage; // text used for message on screen
	sf::Texture m_targetTexture; // texture used for sfml logo
	sf::Sprite m_targetSprite; // sprite used for sfml logo
	bool m_exitGame; // control exiting game

	sf::Vector2f m_lineStart;
	sf::Vector2f m_lineEnd;
	bool m_aiming{ false };
	bool m_dragging{ false };
	sf::Vector2f m_dragOffset;

	sf::VertexArray m_line{sf::Lines};

	sf::VertexArray m_checkpoints{ sf::Lines };
	MyLine m_ray;

	sf::CircleShape m_intersection{ 5.0f };
};

#endif // !GAME_HPP

