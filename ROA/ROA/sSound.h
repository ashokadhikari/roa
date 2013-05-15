#ifndef SSOUND_H_INCLUDED
#define SSOUND_H_INCLUDED

#include <SDL_mixer.h>
#include <SDL.h>

class SDL_Sound {
    private:
    Mix_Chunk* sample;
    Mix_Music* track;


    public:
    SDL_Sound(int frequency, Uint16 format = MIX_DEFAULT_FORMAT, int channels = 2, int chunksize = 1024);
    SDL_Sound();

    ~SDL_Sound();

    Mix_Chunk* LoadSample(const char *filename);
    void AdjustVolume(Mix_Chunk *sample, int volume);

    inline void FreeSample(Mix_Chunk *chunk) {
        Mix_FreeChunk(chunk);
    }

    void PlaySample(int channel, Mix_Chunk *chunk, int loops);

    void PauseSample(int );
    void ResumeSample(int );
    void StopChannel(int );

    Mix_Music* LoadMusic(const char *filename);
    void PlayMusic(Mix_Music* music, int loops);

    inline void FreeMusic(Mix_Music* music) {
        Mix_FreeMusic(music);
    }

    void AdjustVolume(int volume);

    inline void PauseMusic() {
        Mix_PauseMusic();
    }
    inline void ResumeMusic() {
        Mix_ResumeMusic();
    }
    inline void RewindMusic() {
        Mix_RewindMusic();
    }

    inline void StopMusic() {
        Mix_HaltMusic();
    }
};


#endif // SSOUND_H_INCLUDED
