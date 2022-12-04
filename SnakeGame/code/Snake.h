#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Snake
{
private:
	Vector2f m_Position;
	// A RectangleShape object
	RectangleShape m_Shape;
	float m_Speed = 1000.0f;
	bool m_MovingRight = false;
	bool m_MovingLeft = false;
    bool m_MovingUp = false;
	bool m_MovingDown = false;
public:
	Snake(float startX, float startY);
	FloatRect getPosition();
	RectangleShape getShape();
	void moveLeft();
	void moveRight();
    void moveUp();
    void moveDown();
	void update(Time dt);
};