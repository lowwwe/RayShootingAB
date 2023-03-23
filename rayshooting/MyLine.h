#pragma once

#include <SFML/Graphics.hpp>

class MyLine
{
public:
	float slope{0.0f};
	float interceptY{0.0f};
	
	MyLine();
	MyLine(sf::Vector2f t_start, sf::Vector2f t_end);
	sf::Vector2f intersection(MyLine t_line);
};