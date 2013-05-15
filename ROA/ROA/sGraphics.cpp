#include "sGraphics.h"
//initialise graphics
SDL_Graphics::SDL_Graphics(int w, int h, int bpp, Uint32 flags) {
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    TTF_Init();

    font = TTF_OpenFont("fonts/vga869.fon", 16);
    screen = SDL_SetVideoMode(w, h, bpp, flags);
    bgColor = SDL_MapRGB(screen -> format, 0,0,0);
}

SDL_Graphics::~SDL_Graphics() {
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    TTF_Quit();
}

void SDL_Graphics::SetBgColor(int r, int g, int b) {
    SDL_MapRGB(screen -> format, r,g,b);
}

void SDL_Graphics::ShowText(const char* txt, int x, int y) {
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);//|TTF_STYLE_ITALIC);
    textSurface=TTF_RenderText_Solid(font,txt,textColor);
    PutImage(textSurface, x, y);
    SDL_FreeSurface(textSurface);
}

void SDL_Graphics::SetTextColor(int r, int g, int b) {
    textColor.r = r;
    textColor.g = g;
    textColor.b = b;
}

SDL_Surface* SDL_Graphics::LoadImage(const char *filename) {
    SDL_Surface *loadedImage = NULL;
    SDL_Surface *optimised = NULL;

    if((loadedImage = IMG_Load(filename)) == NULL) printf("%d", IMG_GetError());

    if(loadedImage != NULL) {
        optimised = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);

    }
    return optimised;
}

SDL_Surface* SDL_Graphics::LoadImageTrans(const char *filename, int transR, int transG, int transB) {
    SDL_Surface *loadedImage = NULL;
    SDL_Surface *optimised = NULL;

    if((loadedImage = IMG_Load(filename)) == NULL) printf("%d", IMG_GetError());

    if(loadedImage != NULL) {
        Uint32 colorkey;
        colorkey = SDL_MapRGB(screen -> format, transR, transG, transB);
        SDL_SetColorKey(loadedImage, SDL_SRCCOLORKEY, colorkey);
        optimised = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    return optimised;
}

SDL_Surface* SDL_Graphics::LoadImageAlpha(const char *filename) {
    SDL_Surface *loadedImage = NULL;
    SDL_Surface *optimised = NULL;

    if((loadedImage = IMG_Load(filename)) == NULL) printf("%d", IMG_GetError());

    if(loadedImage != NULL) {
        optimised = SDL_DisplayFormatAlpha(loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    return optimised;
}

void SDL_Graphics::PutImage(SDL_Surface *src, SDL_Surface *dst,SDL_Rect *clip, int x, int y) {
    static SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;

    SDL_BlitSurface(src, clip, dst, &dstrect);
}

void SDL_Graphics::PutImage(SDL_Surface *src, int x, int y) {
    static SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;

    SDL_BlitSurface(src, NULL, screen, &dstrect);
}

void SDL_Graphics::ClearScreen() {
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen -> format, 0,0,0));
}

void SDL_Graphics::ClearScreen(int x, int y,int w, int h) {
    SDL_Rect bclip;
    bclip.x = x;
    bclip.y = y;
    bclip.w = w;
    bclip.h = h;
    SDL_FillRect(screen, &bclip, SDL_MapRGB(screen -> format, 0,0,0));
}

void SDL_Graphics::FlipDrawing() {
    SDL_Flip(screen);
}
