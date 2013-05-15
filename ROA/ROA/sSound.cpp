#include "sSound.h"

SDL_Sound::SDL_Sound(int frequency, Uint16 format, int channels, int chunksize) {
    SDL_InitSubSystem(SDL_INIT_AUDIO);

    Mix_OpenAudio(frequency,format,channels,chunksize);
}

SDL_Sound::~SDL_Sound() {
    SDL_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

Mix_Chunk* SDL_Sound::LoadSample(const char *filename) {
    sample = Mix_LoadWAV(filename);
    return sample;
}

void SDL_Sound::AdjustVolume(Mix_Chunk *sample, int volume) {
    Mix_VolumeChunk(sample, volume);
}

void SDL_Sound::PlaySample(int channel, Mix_Chunk *chunk, int loops) {
    Mix_PlayChannel(channel, chunk, loops);
}

void SDL_Sound::PauseSample(int channel) {
    Mix_Pause(channel);
}

void SDL_Sound::ResumeSample(int channel) {
    Mix_Resume(channel);
}

void SDL_Sound::StopChannel(int channel) {
    Mix_HaltChannel(channel);
}

Mix_Music* SDL_Sound::LoadMusic(const char *filename) {
    return Mix_LoadMUS(filename);
}

void SDL_Sound::PlayMusic(Mix_Music* music, int loops) {
    Mix_PlayMusic(music,loops);
}

void SDL_Sound::AdjustVolume(int volume) {
    Mix_VolumeMusic(volume);
}


