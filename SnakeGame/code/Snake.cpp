#include "Snake.h"
#include "TextureHolder.h"

Snake::Snake()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	// Associate a texture with the sprite
	// !!Watch this space!!
	m_Texture.loadFromFile("graphics/snake_head.png");
	m_Sprite.setTexture(m_Texture);
	//m_Sprite = Sprite(TextureHolder::GetTexture("graphics/snake_head2.png"));

	// Set the origin of the sprite to the centre, 
	// for smooth rotation
	m_Sprite.setOrigin(100, 100);
}

void Snake::spawn(IntRect arena, Vector2f resolution, int tileSize)
{
	// Place the player in the middle of the arena
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height / 2;

	// Copy the details of the arena to the player's m_Arena
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;

	// Remember how big the tiles are in this arena
	//m_TileSize = tileSize;

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
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200)// 2 tenths of second
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
}

void Snake::moveRight()
{
	m_RightPressed = true;
}

void Snake::moveUp()
{
	m_UpPressed = true;
}

void Snake::moveDown()
{
	m_DownPressed = true;
}

void Snake::stopLeft()
{
	m_LeftPressed = false;
}

void Snake::stopRight()
{
	m_RightPressed = false;
}

void Snake::stopUp()
{
	m_UpPressed = false;
}

void Snake::stopDown()
{
	m_DownPressed = false;
}

void Snake::update(float elapsedTime, Vector2i mousePosition)
{

	if (m_UpPressed)
	{
		m_Position.y -= m_Speed * elapsedTime;
	}

	if (m_DownPressed)
	{
		m_Position.y += m_Speed * elapsedTime;
	}

	if (m_RightPressed)
	{
		m_Position.x += m_Speed * elapsedTime;
	}

	if (m_LeftPressed)
	{
		m_Position.x -= m_Speed * elapsedTime;
	}

	m_Sprite.setPosition(m_Position);



	// Keep the player in the arena
	if (m_Position.x > m_Arena.width - m_TileSize)
	{
		m_Position.x = m_Arena.width - m_TileSize;
	}

	if (m_Position.x < m_Arena.left + m_TileSize)
	{
		m_Position.x = m_Arena.left + m_TileSize;
	}

	if (m_Position.y > m_Arena.height - m_TileSize)
	{
		m_Position.y = m_Arena.height - m_TileSize;
	}

	if (m_Position.y < m_Arena.top + m_TileSize)
	{
		m_Position.y = m_Arena.top + m_TileSize;
	}

	// Calculate the angle the player is facing
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2,
		mousePosition.x - m_Resolution.x / 2)
		* 180) / 3.141;

	m_Sprite.setRotation(angle);
}

void Snake::upgradeSpeed()
{//20% speed upgrade 
	m_Speed += (START_SPEED * .2);}

void Snake::upgradeHealth()
{
	// 20% max health upgrade
	m_MaxHealth += (START_HEALTH * .2);

}

void Snake::increaseHealthLevel(int amount)
{m_Health += amount;

	// But not beyond the maximum
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}

/*Snake::Snake(float startX, float startY)
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
*/