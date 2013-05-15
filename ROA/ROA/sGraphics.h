#ifndef GRAPHICS_THUCCY_INCLUDED
#define GRAPHICS_THUCCY_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_TTF.h>

class SDL_Graphics {
    private:
        Uint32 bgColor;
        SDL_Color textColor;
        SDL_Surface *textSurface;
        TTF_Font *font;

    public:
        SDL_Surface *screen;

        SDL_Graphics(int w, int h, int bpp = 32, Uint32 flags = SDL_HWSURFACE |SDL_DOUBLEBUF);     //INITIALISE SCREEN constructor

        SDL_Graphics();     //constructor overloaded
        ~SDL_Graphics();    //destructor

        void ShowText(const char*, int x, int y);
        void SetTextColor(int, int, int);
        void SetBgColor(int, int, int);    //specify the background

        SDL_Surface* LoadImage(const char *filename);     //load image and optimise
        SDL_Surface* LoadImageTrans(const char *filename, int transR, int transG, int transB); //for colorkeying
        SDL_Surface* LoadImageAlpha(const char *filename); //for already containing alpha values

        void PutImage(SDL_Surface *src, SDL_Surface *dst,SDL_Rect *clip, int x, int y); //blit image on source
        void PutImage(SDL_Surface *src, int x, int y);

        inline void FreeImage(SDL_Surface* image) {
            SDL_FreeSurface(image);
        }

        inline void Wait(Uint32 ms) {
                SDL_Delay(ms);
            }
        void ClearScreen();
        void ClearScreen(int x, int y, int w, int h);
        void FlipDrawing();

};

 #endif
