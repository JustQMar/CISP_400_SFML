#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Bomb
{
private:
	Vector2f m_Position;
	// A RectangleShape object
	RectangleShape m_Shape;
	float m_Speed = 1000.0f;
public:
	Bomb(float startX, float startY);
	FloatRect getPosition();
	RectangleShape getShape();
	void update(Time dt);
};
