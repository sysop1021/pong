#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string> // need to manipulate int scores into strings for display
#include <time.h> // needed to seed PRNG

#define MAX_SCORE 11

int main(int argc, char* argv[])
{
    // INITIALIZATION GARBAGE
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Pong");

    bool gameWon = false;

    sf::Texture bgTex, p1Tex, p2Tex, ballTex;

    bgTex.loadFromFile("resources/bg.png");
    p1Tex.loadFromFile("resources/paddle1.png");
    p2Tex.loadFromFile("resources/paddle2.png");
    ballTex.loadFromFile("resources/ball.png");

    sf::SoundBuffer paddleBuffer;
    paddleBuffer.loadFromFile("resources/hit.wav");

    sf::SoundBuffer wallBuffer;
    wallBuffer.loadFromFile("resources/hit2.wav");

    sf::SoundBuffer scoreBuffer;
    scoreBuffer.loadFromFile("resources/score.wav");

    sf::Sound paddleSound;
    paddleSound.setBuffer(paddleBuffer);

    sf::Sound wallSound;
    wallSound.setBuffer(wallBuffer);

    sf::Sound scoreSound;
    scoreSound.setBuffer(scoreBuffer);

    sf::Font font;
    font.loadFromFile("resources/Lato-Regular.ttf");

    sf::Text score("0 - 0", font);

    sf::Sprite bg(bgTex), p1(p1Tex), p2(p2Tex), ball(ballTex);

    bg.setOrigin(0, 0);
    bg.setPosition(0, 0);

    ball.setOrigin(ballTex.getSize().x / 2, ballTex.getSize().y / 2);
    ball.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    p1.setOrigin(p1Tex.getSize().x / 2, p1Tex.getSize().y / 2);
    p1.setPosition(window.getSize().x * 0.1f, window.getSize().y / 2);

    p2.setOrigin(p2Tex.getSize().x / 2, p2Tex.getSize().y / 2);
    p2.setPosition(window.getSize().x * 0.9f, window.getSize().y / 2);

    int player_yPos = 0;

    // initial ball speed
    sf::Vector2f ballSpeed((rand() % 5 + 1), (rand() % 5 + 1));

    if (rand() % 2 == 0)
    {
        ballSpeed.x *= -1;
    }

    if (rand() % 2 == 0)
    {
        ballSpeed.y *= -1;
    }

    float p1Speed = 3.f;

    // sometimes too slow, and sometimes ball movement is too vertical

    int p1Score = 0, p2Score = 0;
    std::string p1ScoreStr, p2ScoreStr, scoreStr;

    // GAME LOOP
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // handle player paddle movement
            if (event.type == sf::Event::MouseMoved)
            {
                // Kinda works, but not quite. it's too sensitive to the mouse cursor leaving the bounds of the window
                if (sf::Mouse::getPosition(window).y >= (p1Tex.getSize().y / 2) && sf::Mouse::getPosition(window).y + (p1Tex.getSize().y / 2) <= window.getSize().y)
                {
                    player_yPos = sf::Mouse::getPosition(window).y;
                }
            }
        }
        window.clear();

        window.draw(bg);
        window.draw(ball);
        window.draw(p1);
        window.draw(p2);

        window.draw(score);

        window.display();

        // handle paddle movement
        if(!gameWon)
        {
            p2.setPosition(window.getSize().x * 0.9f, player_yPos);
        }

        // AI Paddle movement
        if (ball.getPosition().y < p1.getPosition().y - p1Tex.getSize().y)
        {
            p1.move(0.f, -p1Speed);
        }

        else if (ball.getPosition().y > p1.getPosition().y + p1Tex.getSize().y)
        {
            p1.move(0.f, p1Speed);
        }

        else
        {

        }

        // collision detection with end walls
        if (ball.getPosition().x + ballTex.getSize().x / 2 > window.getSize().x || ball.getPosition().x - ballTex.getSize().x / 2 < 0)
        {
	    scoreSound.play();

            if (ballSpeed.x > 0) // hackish?
            {
                p1Score++;
            }

            else
            {
                p2Score++;
            }

            // update scoreboard
            if (!gameWon)
            {
                p1ScoreStr = std::to_string(p1Score);
                p2ScoreStr = std::to_string(p2Score);
                scoreStr = p1ScoreStr + " - " + p2ScoreStr;

                score.setString(scoreStr);
            }

            //GAME WON CONDITION CHECK
            if (p1Score == MAX_SCORE)
            {
                score.setString("GAME OVER");
                gameWon = true;
            }

            if (p2Score == MAX_SCORE)
            {
                score.setString("GAME WON");
                gameWon = true;
            }

            // reset ball position and speed after score
            ball.setPosition(window.getSize().x / 2, window.getSize().y / 2);
            ballSpeed.x = (rand() % 5 + 1);
            ballSpeed.y = (rand() % 5 + 1);
            if (rand() % 2 == 0)
            {
                ballSpeed.x *= -1;
            }

            if (rand() % 2 == 0)
            {
                ballSpeed.y *= -1;
            }

        }

        // collision detection with floor/ceiling
        if (ball.getPosition().y + ballTex.getSize().y / 2 > window.getSize().y || ball.getPosition().y - ballTex.getSize().y / 2 < 0)
        {
            ballSpeed.y *= -1;
            wallSound.play();
        }

        // collision detection and bounce off player paddles with "spear prevention" based on if current ballspeed.x is pos or neg
        if ((ball.getGlobalBounds().intersects(p1.getGlobalBounds()) && ballSpeed.x < 0) || (ball.getGlobalBounds().intersects(p2.getGlobalBounds()) && ballSpeed.x > 0))
        {
            ballSpeed.x *= -1;
            paddleSound.play();
        }

        if (!gameWon)
        {
            ball.move(ballSpeed);
        }
    }

    return 0;
}
