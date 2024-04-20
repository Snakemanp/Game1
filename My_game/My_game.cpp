// My_game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<SFML/Graphics.hpp>
#include"game.cpp"
int main()
{
    int frames = 0;
    int lastenemy = -90;
    int speed = 30;
    sf::RenderWindow window(sf::VideoMode(1200, 800), "My window");
    window.setFramerateLimit(30);
    game game1(window);
    game1.spawn_player();
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        window.clear(sf::Color::Black);
        game1.input();
        if(!game1.pause&&!game1.end) {
            game1.playermovement();
            if ((frames - lastenemy) > speed) {
                game1.spawn_enemy();
                lastenemy = frames;
            }
            game1.update();
        }
        game1.display();
        window.display();
        frames++;
    }
    std::cout << "Player Score: " << ' ' << game1.get_player_score()<<'\n';
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
