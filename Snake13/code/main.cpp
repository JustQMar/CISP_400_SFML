#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Snake.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
using namespace sf;

int main()
{
    // Here is the instance of TextureHolder
    TextureHolder holder;

    // The game will always be in one of four states
    enum class State {
        PAUSED, LEVELING_UP,
        GAME_OVER, PLAYING
    };

    // Start with the GAME_OVER state
    State state = State::GAME_OVER;
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
    // Where is the mouse in 
    // relation to world coordinates
    
    
    //Vector2f mouseWorldPosition;
    
    
    // Where is the mouse in 
    // relation to screen coordinates
    Vector2i mouseScreenPosition;
    // Create an instance of the Player class
    Snake player;
    // The boundaries of the arena
    IntRect arena;

    // Create the background
    // VertexArray background;
    // Load the texture for our background vertex array
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
    Bullet bullets[100];
    int currentBullet = 0;
    int bulletsSpare = 24;
    int bulletsInClip = 6;
    int clipSize = 6;
    float fireRate = 1;
    // When was the fire button last pressed?
    Time lastPressed;

    // Create a couple of pickups
    Pickup coinPickup1(1);
    Pickup bombPickup(2);

    // About the game
    int score = 0;
    //int hiScore = 0;

    // For the home/game over screen
    Sprite spriteStartGame;
    Texture textureStartGame = TextureHolder::GetTexture("graphics/background.jpg");
    spriteStartGame.setTexture(textureStartGame);
    spriteStartGame.setPosition(0, 0);
    // Create a view for the HUD
    View hudView(sf::FloatRect(0, 0, 1920, 1080));
    // Create a sprite for the ammo icon
    //Sprite spriteAmmoIcon;
    //Texture textureAmmoIcon = TextureHolder::GetTexture(
    //    "graphics/ammo_icon.png");
    //spriteAmmoIcon.setTexture(textureAmmoIcon);
    //spriteAmmoIcon.setPosition(20, 980);
    // Load the font
    Font font;
    font.loadFromFile("fonts/zombiecontrol.ttf");
    // Paused
    Text pausedText;
    pausedText.setFont(font);
    pausedText.setCharacterSize(155);
    pausedText.setFillColor(Color::White);
    pausedText.setPosition(400, 400);
    pausedText.setString("Paused");
    // Game Over
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(80);
    gameOverText.setFillColor(Color::White);
    gameOverText.setPosition(resolution.x / 16 , resolution.y / 2);
    gameOverText.setString("GAME INSTRUCTION : \n1.Collect coins to increase your size. \n2.Avoid bombs and survive until time runs out. \nClick ENTER to start.");
    
    int SnakeHealth = 3;

    Text ammoText;
    ammoText.setFont(font);
    ammoText.setCharacterSize(55);
    ammoText.setFillColor(Color::White);
    ammoText.setPosition(200, 980);
   

    // Score
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(55);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(20, 0);

    // Load the high score from a text file
    std::ifstream inputFile("gamedata/scores.txt");
    if (inputFile.is_open())
    {
        // >> Reads the data
        //inputFile >> hiScore;
        //inputFile.close();
    }

    // Hi Score
    Text hiScoreText;

    // Wave number
    int wave = 0;
    Text waveNumberText;
    waveNumberText.setFont(font);
    waveNumberText.setCharacterSize(55);
    waveNumberText.setFillColor(Color::White);
    waveNumberText.setPosition(1250, 980);
    waveNumberText.setString("Wave: 0");
    // Health bar
    RectangleShape healthBar;
    healthBar.setFillColor(Color::Red);
    healthBar.setPosition(450, 980);

    // When did we last update the HUD?
    //int framesSinceLastHUDUpdate = 0;
    // How often (in frames) should we update the HUD
    //int fpsMeasurementFrameInterval = 1000;

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
                // Start a new game while in GAME_OVER state
                else if (event.key.code == Keyboard::Return &&
                    state == State::GAME_OVER)
                {
                    wave = 0;
                    score = 0;
                    // Prepare the gun and ammo for next game
                    currentBullet = 0;
                    bulletsSpare = 24;
                    bulletsInClip = 6;
                    clipSize = 6;
                    fireRate = 1;
                    // Reset the player's stats
                    //player.resetPlayerStats();
                    state = State::LEVELING_UP;
                }
                if (state == State::PLAYING)
                {
                    // Reloading
                    if (event.key.code == Keyboard::R)
                    {
                        if (bulletsSpare >= clipSize)
                        {
                            // Plenty of bullets. Reload.
                            bulletsInClip = clipSize;
                            bulletsSpare -= clipSize;
                            //reload.play();
                        }
                        else if (bulletsSpare > 0)
                        {
                            // Only few bullets left
                            bulletsInClip = bulletsSpare;
                            bulletsSpare = 0;
                            //reload.play();
                        }
                        else
                        {
                            // More here soon?!
                            //reloadFailed.play();
                        }
                    }

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

            /*
            
            // Fire a bullet
            if (Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
                {
                    // Pass the centre of the player
                    // and the centre of the cross-hair
                    // to the shoot function
                    bullets[currentBullet].shoot(
                        player.getCenter().x, player.getCenter().y,
                        mouseWorldPosition.x, mouseWorldPosition.y);
                    currentBullet++;
                    if (currentBullet > 99)
                    {
                        currentBullet = 0;
                    }
                    lastPressed = gameTimeTotal;
                    shoot.play();
                    bulletsInClip--;
                }
            }// End fire a bullet
            
            
            */

            

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
                wave++;

                // Prepare the level
                // We will modify the next two lines later
                arena.width = resolution.x - 50;
                arena.height = resolution.y - 50;
                arena.left = 50;
                arena.top = 50;

                // Pass the vertex array by reference 
                // to the createBackground function
                //int tileSize = createBackground(background, arena);

                // Spawn the player in the middle of the arena
                player.spawn(arena, resolution);//, tileSize);

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
            

            // Update the pickups
            //coinPickup1.spawn();
            //bombPickup.spawn();

            coinPickup1.update(dtAsSeconds);
            bombPickup.update(dtAsSeconds);

            // Collision detection
            // Have any zombies been shot?
            for (int i = 0; i < 100; i++)
            {
                for (int j = 0; j < numZombies; j++)
                {
                    if (bullets[i].isInFlight() && zombies[j].isAlive())
                    {
                        if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
                        {
                            // Stop the bullet
                            bullets[i].stop();
                            // Register the hit and see if it was a kill
                            if (zombies[j].hit())
                            {
                                // Not just a hit but a kill too
                                score += 10;
                                //if (score >= hiScore)
                                //{
                                //    hiScore = score;
                                //}
                                //numZombiesAlive--;
                                // When all the zombies are dead (again)
                                //if (numZombiesAlive == 0) {
                                //    state = State::LEVELING_UP;
                                //}
                            }

                            // Make a splat sound
                            //splat.play();

                        }
                    }
                }// End zombie being shot

                // Have any zombies touched the player            
                for (int i = 0; i < numZombies; i++)
                {
                    if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive())
                    {
                        if (player.hit(gameTimeTotal))
                        {
                            // More here later
                            SnakeHealth -= 1;
                            bomb.play();
                        }
                        if (player.getHealth() <= 0)
                        {
                            //std::ofstream outputFile("gamedata/scores.txt");
                            // << writes the data
                            //outputFile << hiScore;
                            //outputFile.close();
                            
                            state = State::GAME_OVER;
                        }
                    }
                }// End player touched

                // Has the player touched a coin
                if (player.getPosition().intersects(coinPickup1.getPosition()) && coinPickup1.isSpawned())
                {
                    player.increaseHealthLevel(coinPickup1.gotIt());
                    // Play a sound
                    pickup.play();

                }

                // Has the player touched a bomb
                if (player.getPosition().intersects(bombPickup.getPosition()) && bombPickup.isSpawned())
                {
                    bulletsSpare += bombPickup.gotIt();
                    // Play a sound
                    pickup.play();

                }

                msSinceLastHUDUpdate += dt.asMilliseconds();

                // size up the health bar
                //healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
                // Increment the number of frames since the previous update
                //framesSinceLastHUDUpdate++;
                // re-calculate every fpsMeasurementFrameInterval frames
                //if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
                if (msSinceLastHUDUpdate > msHUDFrameInterval)
                {
                    // Update game HUD text
                    std::stringstream ssAmmo;
                    std::stringstream ssScore;
                    std::stringstream ssHiScore;
                    std::stringstream ssWave;
                    std::stringstream ssZombiesAlive;
                    // Update the ammo text
                    ssAmmo << SnakeHealth; //<< "/" << bulletsSpare;
                    ammoText.setString(ssAmmo.str());
                    // Update the score text
                    ssScore << "Score:" << score;
                    scoreText.setString(ssScore.str());
                    // Update the high score text
                    //ssHiScore << "Hi Score:" << hiScore;
                    hiScoreText.setString(ssHiScore.str());
                    // Update the wave
                    ssWave << "Wave:" << wave;
                    waveNumberText.setString(ssWave.str());
                    // Update the high score text
                    //ssZombiesAlive << "Zombies:" << numZombiesAlive;
                    //zombiesRemainingText.setString(ssZombiesAlive.str());
                    //framesSinceLastHUDUpdate = 0;
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
            // set the mainView to be displayed in the window
            // And draw everything related to it
            window.setView(mainView);
            // Draw the background
            //window.draw(background, &textureBackground);
            window.draw(spriteBackground);

            // Draw the zombies
            for (int i = 0; i < numZombies; i++)
            {
                window.draw(zombies[i].getSprite());
            }
            for (int i = 0; i < 100; i++)
            {
                if (bullets[i].isInFlight())
                {
                    window.draw(bullets[i].getShape());
                }
            }

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

            //Draw the crosshair
            //window.draw(spriteCrosshair);

            // Switch to the HUD view
            window.setView(hudView);

            // Draw all the HUD elements
            //window.draw(spriteAmmoIcon);
            window.draw(ammoText);
            window.draw(scoreText);
            window.draw(hiScoreText);
            window.draw(healthBar);
            //window.draw(waveNumberText);
            //window.draw(zombiesRemainingText);

        }
        if (state == State::LEVELING_UP)
        {
            window.setView(hudView);
            window.draw(spriteStartGame);
            //window.draw(levelUpText);
        }
        if (state == State::PAUSED)
        {
            window.setView(hudView);
            window.draw(pausedText);
        }
        if (state == State::GAME_OVER)
        {
            window.setView(hudView);
            window.draw(spriteStartGame);
            window.draw(gameOverText);
            window.draw(scoreText);
            window.draw(hiScoreText);
        }
        window.display();

    }

    return 0;
}