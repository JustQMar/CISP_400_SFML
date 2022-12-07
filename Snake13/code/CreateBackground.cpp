#include <SFML/Graphics.hpp>
#include "Arena.h"

int createBackground(IntRect arena) //VertexArray& rVA,
{
	// Anything we do to rVA we are actually doing to background (in the main function)

	// How big is each tile/texture
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	int worldWidth = arena.width; // TILE_SIZE;
	int worldHeight = arena.height; // TILE_SIZE;

	return TILE_SIZE;
}