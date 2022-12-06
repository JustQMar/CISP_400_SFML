#include "Coin.h"

// This the constructor function
Coin::Coin(float startX, float startY)
{
	m_Position.x = startX;
	m_Position.y = startY;

	m_Shape.setSize(sf::Vector2f(100, 100));
	m_Shape.setPosition(m_Position);
}

FloatRect Coin::getPosition()
{
	return m_Shape.getGlobalBounds();
}

RectangleShape Coin::getShape()
{
	return m_Shape;
}

float Coin::getXVelocity()
{
	return m_DirectionX;
}

void Coin::reboundSides()
{
	m_DirectionX = -m_DirectionX;
}

void Coin::reboundBatOrTop()
{
	m_DirectionY = -m_DirectionY;

}

void Coin::reboundBottom()
{
	m_Position.y = 0;
	m_Position.x = 500;
	m_DirectionY = -m_DirectionY;
}

void Coin::update(Time dt)
{
	// Update the ball position variables
	m_Position.y += m_DirectionY * m_Speed * dt.asSeconds();
	m_Position.x += m_DirectionX * m_Speed * dt.asSeconds();

	// Move the ball and the bat
	m_Shape.setPosition(m_Position);
}