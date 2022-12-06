#include "Pickup.h"
#include "TextureHolder.h"

Pickup::Pickup()
{
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/coin.png"));
        m_Sprite.setOrigin(100, 100);
}

void Pickup::setArena(IntRect arena)
{

	// Copy the details of the arena to the pickup's m_Arena
	m_Arena.left = arena.left + 50;
	m_Arena.width = arena.width - 50;
	m_Arena.top = arena.top + 50;
	m_Arena.height = arena.height - 50;

	spawn();
}

void Pickup::spawn()
{
	// Spawn at a random location
	srand((int)time(0) / m_Type);
	int x = (rand() % m_Arena.width);
	srand((int)time(0) * m_Type);
	int y = (rand() % m_Arena.height);

	// Not currently spawned
	//m_Spawned = false;
	m_SecondsSinceSpawn = 0;
	m_Spawned = true;

	m_Sprite.setPosition(x, y);
}
