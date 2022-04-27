//
// Created by dawid on 23.03.2021.
//

#include "Audio.h"


Audio::Audio() {
    SDL_Init(SDL_INIT_AUDIO);

    auto flags =
            MIX_INIT_MP3 |
            MIX_INIT_FLAC |
            MIX_INIT_MID |
            MIX_INIT_MOD |
            MIX_INIT_OGG |
            MIX_INIT_OPUS;

    Mix_Init(flags);

//      alternative 22050Hz for better performance
    if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, 2048) == -1)
        std::cerr << "Audio::init -> failed to initialize mixer api.\n";
}

Audio::~Audio() {
    Mix_Quit();
    for (int i = 0; i < Mix_QuerySpec(&frequency, &format, &channels); ++i)
        Mix_CloseAudio();
    SDL_Quit();
}

[[maybe_unused]] void Audio::playMusic(Mix_Music *music) {
    if (!Mix_PlayingMusic()) {
        Mix_PlayMusic(music, -1);   //  -1 -> infinite loop;
    } else if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    } else {
        Mix_PauseMusic();
    }
}

[[maybe_unused]] void Audio::playSound(Mix_Chunk *sound) {
    Mix_PlayChannel(-1, sound, 0);
}

Mix_Music *Audio::loadMusic(const std::string &file_path) {
    return Mix_LoadMUS(file_path.c_str());
}

Mix_Chunk *Audio::loadSound(const std::string &file_path) {
    return Mix_LoadWAV(file_path.c_str());
}


[[maybe_unused]] void Audio::musicVolume(uint8_t volume) {
    Mix_VolumeMusic(volume);
}

[[maybe_unused]] void Audio::soundVolume(Mix_Chunk *sound, uint8_t volume) {
    Mix_VolumeChunk(sound, volume);
}
