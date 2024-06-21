#include "Sound.h"

// Define static members
sf::SoundBuffer Sound::killbuffer;
sf::SoundBuffer Sound::diebuffer;
sf::SoundBuffer Sound::pupbuffer;
sf::SoundBuffer Sound::shootbuffer;

bool Sound::iskill = false;
bool Sound::isdie = false;
bool Sound::ispup = false;
bool Sound::isshoot = false;

sf::Sound Sound::kill;
sf::Sound Sound::die;
sf::Sound Sound::pup;
sf::Sound Sound::shoot;

void Sound::set() {
    if (!killbuffer.loadFromFile("hurt.wav")) {
        iskill = false;
    }
    else {
        iskill = true;
        kill.setBuffer(killbuffer);
        kill.setVolume(100.0f);
    }

    if (!diebuffer.loadFromFile("explosion.wav")) {
        isdie = false;
    }
    else {
        isdie = true;
        die.setBuffer(diebuffer);
        die.setVolume(100.0f);
    }

    if (!pupbuffer.loadFromFile("power_up.wav")) {
        ispup = false;
    }
    else {
        ispup = true;
        pup.setBuffer(pupbuffer);
        pup.setVolume(100.0f);
    }

    if (!shootbuffer.loadFromFile("tap.wav")) {
        isshoot = false;
    }
    else {
        isshoot = true;
        shoot.setBuffer(shootbuffer);
        shoot.setVolume(100.0f);
    }
}

void Sound::play_kill() {
    if (iskill) kill.play();
}

void Sound::play_die() {
    if (isdie) die.play();
}

void Sound::play_pup() {
    if (ispup) pup.play();
}

void Sound::play_shoot() {
    if (isshoot) shoot.play();
}
