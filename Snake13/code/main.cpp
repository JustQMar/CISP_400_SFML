#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Snake.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
//#include "Bullet.h"
#include "Pickup.h"
using namespace sf;

int main()
{
    // Here is the instance of TextureHolder
    TextureHolder holder;

    // The game will always be in one of four states
    enum class State {
        PAUSED, LEVELING_UP,
        GAME_OVER, 
        PLAYING, STARTING
    };

    // Start with the GAME_OVER state
    State state = State::STARTING;
    // Get the screen resolution and 
    // create an SFML window
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

    // Where is the mouse in relation to screen coordinates
    Vector2i mouseScreenPosition;
    // Create an instance of the Player class
    Snake player;
    // The boundaries of the arena
    IntRect arena;

    // Create the background
    Texture textureBackground;
    textureBackground.loadFromFile("graphics/grass.png");
    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    // Prepare for a horde of zombies
    int numZombies;
    int numZombiesAlive;
    Zombie* zombies = nullptr;

    // 100 bullets should do
    //Bullet bullets[100];
    //int currentBullet = 0;
    //int bulletsSpare = 24;
    // When was the fire button last pressed?
    Time lastPressed;

    // Create a couple of pickups
    Pickup coinPickup1(1);
    Pickup bombPickup(2);

    // About the game
    int Score = 0;

    // For the home or game over screen
    Sprite spriteStartGame;
    Texture textureStartGame = TextureHolder::GetTexture("graphics/background.jpg");
    spriteStartGame.setTexture(textureStartGame);
    spriteStartGame.setPosition(0, 0);

    // Create a view for the HUD
    View hudView(sf::FloatRect(0, 0, 1920, 1080));

    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    // Paused
    Text pausedText;
    pausedText.setFont(font);
    pausedText.setCharacterSize(155);
    pausedText.setFillColor(Color::White);
    pausedText.setPosition(400, 400);
    pausedText.setString("Paused");
    // Start
    Text gameStartText;
    gameStartText.setFont(font);
    gameStartText.setCharacterSize(70);
    gameStartText.setFillColor(Color::White);
    gameStartText.setPosition(resolution.x / 16 , resolution.y / 2);
    gameStartText.setString("GAME INSTRUCTION : \n1. Collect coins to increase your score. \n2. Avoid bombs and the edge of the screen. \nClick ENTER to start.");
    // Game Over
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(80);
    gameOverText.setFillColor(Color::White);
    gameOverText.setPosition(resolution.x / 4, resolution.y / 2);
    gameOverText.setString("GAME OVER: \nPress Enter to Restart");

    //Heath
    int SnakeHealth = 3;
    Text healthText;
    healthText.setFont(font);
    healthText.setCharacterSize(55);
    healthText.setFillColor(Color::White);
    healthText.setPosition(20, 980);

    // Score
    int score = 0;
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(55);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(20, 0);


    // Hi Score
    Text hiScoreText;


    // Health bar
    RectangleShape healthBar;
    healthBar.setFillColor(Color::Red);
    healthBar.setPosition(450, 980);

    float msSinceLastHUDUpdate = 0;
    float msHUDFrameInterval = 1000;

    // Prepare the powerup sound
    SoundBuffer powerupBuffer;
    powerupBuffer.loadFromFile("sound/powerup.wav");
    Sound powerup;
    powerup.setBuffer(powerupBuffer);
    // Prepare the pickup sound
    SoundBuffer pickupBuffer;
    pickupBuffer.loadFromFile("sound/coin.wav");
    Sound pickup;
    pickup.setBuffer(pickupBuffer);
    // Prepare the pickup sound
    SoundBuffer bombBuffer;
    bombBuffer.loadFromFile("sound/bomb.wav");
    Sound bomb;
    bomb.setBuffer(bombBuffer);
    // Prepare the pickup sound
    SoundBuffer countBuffer;
   countBuffer.loadFromFile("sound/countdown.wav");
    Sound count;
    count.setBuffer(countBuffer);
    // Prepare the hit sound
    SoundBuffer hitBuffer;
    hitBuffer.loadFromFile("sound/hit.wav");
    Sound hit;
    hit.setBuffer(hitBuffer);

    
    Texture textureBoom;
    textureBoom.loadFromFile("graphics/tree.png");
    Sprite spriteBoom;
    spriteBoom.setTexture(textureBoom);

    // The main game loop
    while (window.isOpen())
    {
        /*
        ************
        Handle input
        ************
        */
        // Handle events by polling
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
                    // clock reset
                    clock.restart();
                }

                // Start a new game while in Start state
                else if (event.key.code == Keyboard::Return &&
                    state == State::STARTING)
                {

                    // Prepare health
                    Score = 0;
                    SnakeHealth = 3;
                    // Reset the player's stats
                    //player.resetPlayerStats();
                    state = State::LEVELING_UP;
                }

                // Start a new game while in Start state
                else if (event.key.code == Keyboard::Return &&
                    state == State::GAME_OVER)
                {
                    // Prepare health
                    Score = 0;
                    SnakeHealth = 3;
                    state = State::LEVELING_UP;
                }

            }
        }// End event polling
        // Handle the player quitting
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
        // Handle WASD while playing
        if (state == State::PLAYING)
        {

            // Handle the pressing and releasing of the WASD keys
            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                player.moveUp();
            }

            if (Keyboard::isKeyPressed(Keyboard::S))
            {
                player.moveDown();
            }

            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                player.moveLeft();
            }

            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                player.moveRight();
            }

        }// End WASD while playing
        // Handle the LEVELING up state
        if (state == State::LEVELING_UP)
        {
            // Handle the player levelling up
            if (event.key.code == Keyboard::Enter)
            {
                state = State::PLAYING;
            }

            if (state == State::PLAYING)
            {
                // Increase the wave number
                //wave++;

                // Prepare the level
                // We will modify the next two lines later
                arena.width = resolution.x - 50;
                arena.height = resolution.y - 50;
                arena.left = 50;
                arena.top = 50;

                // Spawn the player in the middle of the arena
                player.spawn(arena, resolution);

                // Create a horde of zombies
                numZombies = 1;

                // Delete the previously allocated memory (if it exists)
                delete[] zombies;
                zombies = createHorde(numZombies, arena);
                numZombiesAlive = numZombies;

                // Play the powerup sound
                powerup.play();

                // Configure the pick-ups
                
                coinPickup1.setArena(arena);
                bombPickup.setArena(arena);


                // Reset the clock so there isn't a frame jump
                clock.restart();
            }
        }// End LEVELING up

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
            player.update(dtAsSeconds);// Mouse::getPosition());
            // Make a note of the players new position
            Vector2f playerPosition(player.getCenter());
            
            coinPickup1.update(dtAsSeconds);
            bombPickup.update(dtAsSeconds);

            // Collision detection
            for (int i = 0; i < 100; i++)
            {

                // Has the player touched a coin
                if (player.getPosition().intersects(coinPickup1.getPosition()) && coinPickup1.isSpawned())
                {
                    player.increaseHealthLevel(coinPickup1.gotIt());
                    Score += 1;
                    pickup.play();

                }

                // Has the player touched a bomb
                if (player.getPosition().intersects(bombPickup.getPosition()) && bombPickup.isSpawned())
                {
                    //bulletsSpare += bombPickup.gotIt();
                    SnakeHealth -= 1;

                    bomb.play();

                    bombPickup.getPosition();
                    spriteBoom.setPosition(810, 0);


                    if (SnakeHealth <= 0)
                    {
                        state = State::GAME_OVER;
                    }


                }

                msSinceLastHUDUpdate += dt.asMilliseconds();

                if (msSinceLastHUDUpdate > msHUDFrameInterval)
                {
                    // Update game HUD text
                    std::stringstream ssLife;
                    std::stringstream ssScore;

                    // Update the life text
                    ssLife << "Life:" << SnakeHealth; 
                    healthText.setString(ssLife.str());

                    // Update the score text
                    ssScore << "Score:" << Score;
                    scoreText.setString(ssScore.str());


                    msSinceLastHUDUpdate = 0;
                }// End HUD update


            }//End Collision Detection

        }// End updating the scene

        /*
        **************
        Draw the scene
        **************
        */

        window.clear();

        if (state == State::PLAYING)
        {
            // set mainView in the window
            window.setView(mainView);
            // Draw background
            window.draw(spriteBackground);


            // Draw the pick-ups, if currently spawned
            if (bombPickup.isSpawned())
            {
                window.draw(bombPickup.getSprite());
            }


            if (coinPickup1.isSpawned())
            {
                window.draw(coinPickup1.getSprite());
            }


            // Draw the player
            window.draw(player.getSprite());

            // Switch to the HUD view
            window.setView(hudView);

            // Draw all the HUD elements
            //window.draw(healthText);
            window.draw(scoreText);
            //window.draw(healthBar);

        }
        if (state == State::LEVELING_UP)
        {
            window.setView(hudView);
            window.draw(spriteStartGame);
        }
        if (state == State::PAUSED)
        {
            window.setView(hudView);
            window.draw(pausedText);
        }
        if (state == State::STARTING)
        {
            window.setView(hudView);
            window.draw(spriteStartGame);
            window.draw(gameStartText);
            window.draw(scoreText);
        }
        if (state == State::GAME_OVER)
        {
            window.draw(gameOverText);
            window.draw(scoreText);

        }
        window.display();

    }

    return 0;
}