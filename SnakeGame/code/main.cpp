#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Pickup.h"
#include "Coin.h"
#include "Bomb.h"
#include <SFML/Audio.hpp>

using namespace sf;

int main()
{
	// The game will always be in one of four states
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	// Start with the GAME_OVER state
	State state = State::GAME_OVER;

	// Get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Snake Game", Style::Fullscreen);

	// Create a an SFML View for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Here is our clock for timing everything
	Clock clock;

	// How long has the PLAYING state been active
	Time gameTimeTotal;

	// Create an instance of the Player class
	Snake player;

	// The boundaries of the arena
	IntRect arena;

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/grass.png");
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	// Create a couple of pickups
	//Pickup healthPickup(1);
	//Pickup bombPickup(2);

	Bomb hello;

	// Prepare the coin
	Texture textureCoin;
	textureCoin.loadFromFile("graphics/coin.png");
	Sprite spriteCoin;
	spriteCoin.setTexture(textureCoin);
	spriteCoin.setPosition(800, 200);

	// Prepare the bomb
	Texture textureBomb;
	textureBomb.loadFromFile("graphics/bomb.png");
	Sprite spriteBomb;
	spriteBomb.setTexture(textureBomb);
	spriteBomb.setPosition(200, 300);

	// Prepare the hit sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/bomb.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	// Prepare the splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/coin.wav");
	Sound splat;
	splat.setBuffer(splatBuffer);

	// We will add a ball in the next chapter
	/*
	Coin ball(100, 100);
	bool m_Spawned = true;
	if (m_Spawned)
	{
		Coin (0, 0);
	}
	*/

	// The main game loop
	while (window.isOpen())
	{
		/*
		************
		Handle input
		************
		*/

		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				// Pause a game while playing
				if (event.key.code == Keyboard::Return &&
					state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if (event.key.code == Keyboard::Return &&
					state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return &&
					state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}

				if (state == State::PLAYING)
				{
				}

			}
		}// End event polling


		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Handle controls while playing
		if (state == State::PLAYING)
		{
			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
				//hit.play();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
				//hit.play();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
				//hit.play();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
				//hello.spawn(); 
				//hit.play();
			}

		}// End WASD while playing

		// Handle the levelling up state
		if (state == State::LEVELING_UP)
		{
			// Handle the player levelling up
			if (event.key.code == Keyboard::Enter)
			{
				state = State::PLAYING;
			}

			if (state == State::PLAYING)
			{
				// Prepare thelevel
				// We will modify the next two lines later
				arena.width = resolution.x - 50;
				arena.height = resolution.y - 50;
				arena.left = 50;
				arena.top = 50;

				// We will modify this line of code later
				//int tileSize = CreateBackground(background);

				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution);//, tileSize);

				// Configure the pick-ups
				//coinPickup.setArena();
				//coinPickup.spawn();


				// Reset the clock so there isn't a frame jump
				clock.restart();


			}
		}// End levelling up

		/*
		****************
		UPDATE THE FRAME
		****************
		*/
		if (state == State::PLAYING)
		{
			// Update the delta time
			Time dt = clock.restart();
			// Update the total game time
			gameTimeTotal += dt;
			// Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			//ball.update(dt);
			/*
			// Has the ball hit the bat?
			if (spriteCoin.getPosition().intersects(player.getPosition()))
			{
				// Hit detected so reverse the ball and score a point
				ball.reboundBatOrTop();
				splat.play();
				m_Spawned = false;
			}
			*/

		}// End updating the scene

		/*
		**************
		Draw the scene
		**************
		*/

		if (state == State::PLAYING)
		{
			window.clear();

			// set the mainView to be displayed in the window
			// And draw everything related to it
			window.setView(mainView);
			// Draw the background
			window.draw(spriteBackground);
			// Draw the player
			window.draw(player.getSprite());
			window.draw(spriteCoin);
			window.draw(spriteBomb);
			//window.draw(ball.getShape());

		}

		if (state == State::LEVELING_UP)
		{
		}

		if (state == State::PAUSED)
		{
		}

		if (state == State::GAME_OVER)
		{
		}

		window.display();

	}// End game loop

	return 0;
}