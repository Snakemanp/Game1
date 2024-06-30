// My_game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "game.cpp"
#include <iostream>
#include <Windows.h>
#include "Home.cpp"
#include "Resources.h"

int main()
{
    long int frames = 0;
    long int lastenemy = 60;
    long int speed = 30;

    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);

    int width = workArea.right - workArea.left;
    int height = workArea.bottom - workArea.top;

    sf::RenderWindow window(sf::VideoMode(width, height), "Geo-Wars", sf::Style::Default);
    HWND hwnd = window.getSystemHandle();

    HICON hIcon = static_cast<HICON>(LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED));
    if (hIcon) {
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    }

    window.setPosition(sf::Vector2i(0,0));
    window.setFramerateLimit(30);


    Home home(window);

    sf::Clock deltaClock;

    while (window.isOpen()) {
        //home.processEvents();

        ImGui::SFML::Update(window, deltaClock.restart());

        home.update();

        window.clear();
        home.render();
        window.display();
    }
    home.~Home();
    if (hIcon) {
        DestroyIcon(hIcon);
    }
    return 0;

}
