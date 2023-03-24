#include "MyLine.h"

MyLine::MyLine()
{
}

/// <summary>
/// generate equation of line
/// </summary>
/// <param name="t_start">x1,y1 first point</param>
/// <param name="t_end">x2,y2 second point</param>
MyLine::MyLine(sf::Vector2f t_start, sf::Vector2f t_end)
{
	slope = (t_end.y - t_start.y) / (t_end.x - t_start.x);
	interceptY = t_start.y - slope * t_start.x;
	A = t_end.y - t_start.y;
	B = t_start.x - t_end.x;
	C = A * t_start.x + B * t_start.y;
}

/// <summary>
/// get intersection point between two lines
/// </summary>
/// <param name="t_line">other line</param>
/// <returns>point of intersection</returns>
sf::Vector2f MyLine::intersection(MyLine t_line)
{
	sf::Vector2f point;
	point.x = (t_line.interceptY - interceptY) / (slope - t_line.slope);
	point.y = point.x * slope + interceptY;

	float determinant = A * t_line.B - t_line.A * B;
	if (determinant != 0)
	{
		point.x = (t_line.B * C - B * t_line.C) / determinant;
		point.y = (A * t_line.C - t_line.A * C) / determinant;
	}
	else
	{
		return sf::Vector2f{ -1.0f,-1.0f };
	}
	return point;
}


/// <summary>
/// solve line equation for x
/// </summary>
/// <param name="t_x">x coordinate</param>
/// <returns>y coordinate</returns>
float MyLine::findY(float t_x)
{
	return slope * t_x + interceptY;
}

/// <summary>
/// solve line equation for x
/// </summary>
/// <param name="t_y">y coordinate</param>
/// <returns>x coordinate</returns>
float MyLine::findX(float t_y)
{
	return (t_y - interceptY) / slope;
}
