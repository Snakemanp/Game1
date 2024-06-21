#pragma once
#include <SFML/Audio.hpp>

class Sound {
public:
    static sf::SoundBuffer killbuffer;
    static sf::SoundBuffer diebuffer;
    static sf::SoundBuffer pupbuffer;
    static sf::SoundBuffer shootbuffer;

    static bool iskill, isdie, ispup, isshoot;

    static sf::Sound kill;
    static sf::Sound die;
    static sf::Sound pup;
    static sf::Sound shoot;

    static void set();

    static void play_kill();
    static void play_die();
    static void play_pup();
    static void play_shoot();
};
