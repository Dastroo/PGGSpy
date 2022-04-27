//
// Created by dawid on 23.03.2021.
//

#pragma once

#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

class Audio {
    int frequency = 44100;
    int channels = 2;
    uint16_t format = MIX_DEFAULT_FORMAT;

public:
    Audio();

    ~Audio();

    ///  10s < music ; longer than 10 seconds
    [[maybe_unused]] void playMusic(Mix_Music *music);

    ///  10s > chunk ; shorter than 10 seconds
    [[maybe_unused]] void playSound(Mix_Chunk *sound);

    Mix_Music *loadMusic(std::string const &file_path);

    Mix_Chunk *loadSound(std::string const &file_path);

    /// @volume between 0-128
    [[maybe_unused]] void musicVolume(uint8_t volume);

    /// @volume between 0-128
    [[maybe_unused]] void soundVolume(Mix_Chunk *sound, uint8_t volume);
};