#include "Snake.h"
Snake::Snake(float startX, float startY)
{
    m_Position.x = startX;
	m_Position.y = startY;

	m_Shape.setSize(sf::Vector2f(50, 5));
	m_Shape.setPosition(m_Position);
}

FloatRect Snake::getPosition()
{
    return m_Shape.getGlobalBounds();
}
RectangleShape Snake::getShape()
{
    return m_Shape;
}

void Snake::moveLeft()
{
    m_MovingLeft = true;
}

void Snake::moveRight()
{
    m_MovingRight = true;
}

void Snake::moveUp()
{
    m_MovingUp = true;
}

void Snake::moveDown()
{
    m_MovingDown = true;
}

void Snake::update(Time dt)
{
    if (m_MovingLeft) {
		m_Position.x -= m_Speed * dt.asSeconds();
	}
	if (m_MovingRight) {
		m_Position.x += m_Speed * dt.asSeconds();
	}
    if (m_MovingDown) {
		m_Position.y -= m_Speed * dt.asSeconds();
	}
	if (m_MovingUp) {
		m_Position.y += m_Speed * dt.asSeconds();
	}
	m_Shape.setPosition(m_Position);
}