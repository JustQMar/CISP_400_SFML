#include "Snake.h"

#include "Snake.h"
#include "TextureHolder.h"

Snake::Snake()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	// Associate a texture with the sprite
	m_Texture.loadFromFile("graphics/snake_head.png");
	m_Sprite.setTexture(m_Texture);

	// Set the origin of the sprite
	m_Sprite.setOrigin(100, 100);
}

void Snake::spawn(IntRect arena, Vector2f resolution)//, int tileSize)
{
	// Place the snake grass center
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height / 2;

	// Copy the details of the arena to the player's m_Arena
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;

	// Strore the resolution for future use
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;

}

Time Snake::getLastHitTime()
{
	return m_LastHit;
}

bool Snake::hit(Time timeHit)
{
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 500)// half a second
	{
		m_LastHit = timeHit;
		m_Health -= 10;
		return true;
	}
	else
	{
		return false;
	}

}

FloatRect Snake::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Vector2f Snake::getCenter()
{
	return m_Position;
}

float Snake::getRotation()
{
	return m_Sprite.getRotation();
}

Sprite Snake::getSprite()
{
	return m_Sprite;
}

int Snake::getHealth()
{
	return m_Health;
}

void Snake::moveLeft()
{
	m_LeftPressed = true;
	m_RightPressed = false;
	m_UpPressed = false;
	m_DownPressed = false;

}

void Snake::moveRight()
{
	m_RightPressed = true;
	m_LeftPressed = false;
	m_UpPressed = false;
	m_DownPressed = false;

}

void Snake::moveUp()
{
	m_UpPressed = true;
	m_DownPressed = false;
	m_RightPressed = false;
	m_LeftPressed = false;

}

void Snake::moveDown()
{
	m_DownPressed = true;
	m_UpPressed = false;
	m_RightPressed = false;
	m_LeftPressed = false;

}

void Snake::update(float elapsedTime)//, Vector2i mousePosition)
{
	float angle = 0;

	if (m_UpPressed)
	{
		m_Position.y -= m_Speed * elapsedTime;
		angle = 0;
	}

	if (m_DownPressed)
	{
		m_Position.y += m_Speed * elapsedTime;
		angle = 180;
	}

	if (m_RightPressed)
	{
		m_Position.x += m_Speed * elapsedTime;
		angle = 90;
	}

	if (m_LeftPressed)
	{
		m_Position.x -= m_Speed * elapsedTime;
		angle = 270;
	}

	m_Sprite.setPosition(m_Position);

	m_Sprite.setRotation(angle);

	// Keep the player in the arena
	if (m_Position.x > m_Arena.width)// - m_TileSize)
	{
		m_Position.x = m_Arena.width;// - m_TileSize;
		//SnakeHealth--;
	}

	if (m_Position.x < m_Arena.left)// + m_TileSize)
	{
		m_Position.x = m_Arena.left;// + m_TileSize;
	}

	if (m_Position.y > m_Arena.height)// - m_TileSize)
	{
		m_Position.y = m_Arena.height;// -m_TileSize;
	}

	if (m_Position.y < m_Arena.top)// + m_TileSize)
	{
		m_Position.y = m_Arena.top;// +m_TileSize;
	}

}

void Snake::upgradeSpeed()
{//20% speed upgrade 
	m_Speed += (START_SPEED * .2);
}

void Snake::upgradeHealth()
{
	// 20% max health upgrade
	m_MaxHealth += (START_HEALTH * .2);

}

void Snake::increaseHealthLevel(int amount)
{
	m_Health += amount;

	// But not beyond the maximum
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}




