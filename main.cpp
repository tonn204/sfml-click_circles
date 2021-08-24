#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

sf::CircleShape createNewCircle(void);
bool roundToMicroseconds(int timeInMicroseconds, int upBorder, int downBroder);
bool roundToMiliseconds(int timeInMiliseconds, int upBorder, int downBroder);
int changeFreqDownValue = 65;
int miliSecUpValue = 630, miliSecDownValue = 570;

template <typename T>
void remove_at(std::vector<T>& v, typename std::vector<T>::size_type n)
{
    std::swap(v[n], v.back());
    v.pop_back();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Hit Circle");
    window.setVerticalSyncEnabled(true);

    sf::Clock spawn_clock, pulse_clock;

    std::vector<sf::CircleShape> circles;
    std::vector<float> circle_radius;

    sf::SoundBuffer noiseBuffer;
    if (!noiseBuffer.loadFromFile("noise1.wav"));

    sf::Sound noiseSound;
    noiseSound.setBuffer(noiseBuffer);

    sf::Font myFont;
    if (!myFont.loadFromFile("font.ttf"));

    sf::Text scoreText;
    scoreText.setFont(myFont);
    scoreText.setCharacterSize(15);

    sf::Text defeatText;
    defeatText.setFont(myFont);
    defeatText.setCharacterSize(20);

    int score = 0, lostCircles = 0;
    bool gameResume = true;

    srand(time(NULL));

    while (window.isOpen())
    {
        int spawnTime = spawn_clock.getElapsedTime().asMilliseconds();
        int pulseTime = pulse_clock.getElapsedTime().asMicroseconds();

        if (roundToMiliseconds(spawnTime, miliSecUpValue, miliSecDownValue) && gameResume)
        {    
            spawn_clock.restart();
            circles.push_back(createNewCircle());
        }


        if (roundToMicroseconds(pulseTime / 1000, 1000, changeFreqDownValue) && gameResume)
        {
            pulse_clock.restart();

            float newRadius = 0.0f;

            for (int i = 0; i < circles.size(); i++)
            {
                newRadius = circles[i].getRadius() + 0.2;
                circles[i].setRadius(newRadius);              
            }
        }

        for (int i = 0; i < circles.size(); i++)
        {
            if (circles[i].getRadius() >= 80.0f && gameResume)
            {
                lostCircles++;
                remove_at(circles, i);               
                break;
            }
        }

        if (lostCircles >= 3)
        {
            gameResume = false;
        }
        else
        {
            gameResume = true;
        }

        scoreText.setString("Your score is: " + std::to_string(score));

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                else if (event.key.code == sf::Keyboard::Space && gameResume == false)
                {
                    gameResume = true;
                    score = 0;
                    lostCircles = 0;
                    changeFreqDownValue = 65;
                    miliSecUpValue = 630;
                    miliSecDownValue = 570;
                    spawn_clock.restart();
                    pulse_clock.restart();
                    circles.clear();
                }
            case sf::Event::MouseButtonPressed:
                if (event.key.code == sf::Mouse::Left)
                {
                    for (int i = 0; i < circles.size(); i++)
                    {
                        if (circles[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) && gameResume)
                        {
                            noiseSound.play();
                            score++;
                            if(changeFreqDownValue > 1)
                                changeFreqDownValue--;
                            std::cout << "changeFreqDownValue: " << changeFreqDownValue << std::endl;
                            if (miliSecUpValue > 330 && miliSecDownValue > 250)
                            {
                                miliSecUpValue -= 5;
                                miliSecDownValue -= 5;
                            }
                            remove_at(circles, i);
                            break;
                        }
                    }

                }
            }
        }

        window.clear();
        for (int i = 0; i < circles.size(); i++)
            window.draw(circles[i]);
        if(gameResume)
            window.draw(scoreText);
        else
        {
            defeatText.setString("You lost. Your Score was: " + std::to_string(score) + "\nPress space to play again.");
            defeatText.setPosition(200.0f, 300.0f);
            window.draw(defeatText);
        }
        window.display();
    }

    return 0;
}

sf::CircleShape createNewCircle(void)
{
    sf::CircleShape circle(10.0f);
    circle.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
    circle.setPosition(rand() % 641 + 80,rand() % 441 + 80);  
    circle.move(-circle.getRadius(), 0.0f);

    return circle;
}

bool roundToMicroseconds(int timeInMicroseconds, int upBorder, int downBroder)
{
    if (timeInMicroseconds >= downBroder && timeInMicroseconds <= upBorder)
        return true;
    else
        return false;
}

bool roundToMiliseconds(int timeInMiliseconds, int upBorder, int downBroder)
{
    if (timeInMiliseconds >= downBroder && timeInMiliseconds <= upBorder)
        return true;
    else
        return false;
}
