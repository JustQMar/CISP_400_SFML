#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Coin
{
private:
	Vector2f m_Position;
	// A RectangleShape object
	RectangleShape m_Shape;
	float m_Speed = 1000.0f;
public:
	Coin(float startX, float startY);
	FloatRect getPosition();
	RectangleShape getShape();
	void update(Time dt);
};