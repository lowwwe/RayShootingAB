#include "MyLine.h"

MyLine::MyLine()
{
}

MyLine::MyLine(sf::Vector2f t_start, sf::Vector2f t_end)
{
	slope = (t_end.y - t_start.y) / (t_end.x - t_start.x);
	interceptY = t_start.y - slope * t_start.x;
}

sf::Vector2f MyLine::intersection(MyLine t_line)
{
	sf::Vector2f point;
	//ntersectionX = (c2 - c1) / (m1 - m2)
	//		intersectionY = (m1 * c1 - c2 * m2) / m1 - m2 or intersectionY = m1 * intersectionX + c1

	point.x = (t_line.interceptY - interceptY) / (slope - t_line.slope);
	point.y = point.x * slope + interceptY;
	return point;
}
