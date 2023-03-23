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
	bool m_dragging{ false };

	sf::VertexArray m_line{sf::Lines};
};

#endif // !GAME_HPP

