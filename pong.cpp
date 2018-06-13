/**
    TODO: fix the ability to "spear" the ball with the paddle and prevent correct bouncing
    TODO: make a paddle AI that is actually beatable, instead of precisely real-time
        following the y-pos of the ball
    TODO: make a game won condition and start a new game
        (classic Pong is first to 11 - https://en.wikipedia.org/wiki/Pong#Gameplay)
    TODO: ball takes off in a random direction when reset
    TODO: ball speeds up with each volley - speed resets after point is scored
    TODO: ball waits for a moment after being reset and before launching (?)
    TODO: vary the reflection angle based on what part of the paddle is hit
    TODO: sound - "boink" noise on bounce
    TODO: GOOD GRAFIKS - https://www.youtube.com/watch?v=Z_o6DxhslAI
    TODO: implement framestep (per Game Programming Patterns - Game Loop chapter)
            http://gameprogrammingpatterns.com/game-loop.html#run,-run-as-fast-as-you-can

    TODO: refactor - OOP nonsense and seperate files

    TODO: port to Android
*/

#include <SFML/Graphics.hpp>
//#include <iostream> // not needed - using temporarily to use cout for testing
#include <string> // need to manipulate int scores into strings for display
#include <time.h> // needed to seed PRNG

int main(int argc, char* argv[])
{
    // INITIALIZATION GARBAGE
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Pong");
    window.setFramerateLimit(60);

    sf::Texture bgTex, p1Tex, p2Tex, ballTex;

    bgTex.loadFromFile("resources/bg.png");
    p1Tex.loadFromFile("resources/paddle1.png");
    p2Tex.loadFromFile("resources/paddle2.png");
    ballTex.loadFromFile("resources/ball.png");

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
    sf::Vector2f ballSpeed(rand() % 4 + 2, rand() % 4 + 2); // TODO: ballSpeed needs to be able to be negative, too
    /**
        Okay, so, what actually needs to be done here is to determine a fixed SPEED and randomly select a DIRECTION.
        If speed is going to be 4px/tick (random eg.), that needs to be Pythagrean'ed to determine the x speed and y speed
        (independent of the speed going up with each volley)
    */

    int p1Score = 0, p2Score = 0;
    std:: string p1ScoreStr, p2ScoreStr, scoreStr;

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
                // if (sf::Mouse::getPosition(window).y >= (p1Tex.getSize().y / 2) && sf::Mouse::getPosition(window).y + (p1Tex.getSize().y / 2) <= window.getSize().y)
                // Kinda works, but not quite
                //{
                    player_yPos = sf::Mouse::getPosition(window).y;
                //}
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
        p2.setPosition(window.getSize().x * 0.9f, player_yPos);

        // TODO: AI PADDLE MOVEMENT -  TEMPORARILY COMMENTED OUT TO GET SCORING ON BOTH SIDES
        //p1.setPosition(window.getSize().x * 0.1f, ball.getPosition().y);

        // collision detection with end walls
        if (ball.getPosition().x + ballTex.getSize().x / 2 > window.getSize().x || ball.getPosition().x - ballTex.getSize().x / 2 < 0)
        {
            // reset ball position after score
            ball.setPosition(window.getSize().x / 2, window.getSize().y / 2);

            if (ballSpeed.x > 0) // hackish?
            {
                p1Score++;
            }

            else
            {
                p2Score++;
            }

            //std::cout << p1Score << " - " << p2Score << std::endl;

            //score.setString(std::to_string(p1Score) + " - " + std::to_string(p2Score) + std::endl);
            // not sure why the above line don't work, but the junk below does... ???

            /**
            BUG: std::to_string does not work with MinGW compiler - F:\Dev\pong\pong.cpp|107|error: 'to_string' is not a member of 'std'
            compiles and runs with gcc on linux.

            The fix per http://tehsausage.com/mingw-to-string
            ...does not work - F:\Dev\pong\pong.cpp|112|error: no match for 'operator<<' (operand types are 'std::string {aka std::basic_string<char>}' and 'const char [4]')|

            FIXED with the following workaround:
            */
            p1ScoreStr = std::to_string(p1Score);
            p2ScoreStr = std::to_string(p2Score);
            scoreStr = p1ScoreStr + " - " + p2ScoreStr;
            /*************/

            score.setString(scoreStr);
        }

        // collision detection with floor/ceiling
        if (ball.getPosition().y + ballTex.getSize().y / 2 > window.getSize().y || ball.getPosition().y - ballTex.getSize().y / 2 < 0)
        {
            ballSpeed.y *= -1;
        }

        // collision detection and bounce off player paddles
        if (ball.getGlobalBounds().intersects(p1.getGlobalBounds()) || ball.getGlobalBounds().intersects(p2.getGlobalBounds()))
        {
            // except for collision with the top of the paddle.
            ballSpeed.x *= -1;;
        }

        ball.move(ballSpeed);

    }
    return 0;
}
