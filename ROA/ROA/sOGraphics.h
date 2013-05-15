#ifndef GRAPHICS_THUCCY_INCLUDED
#define GRAPHICS_THUCCY_INCLUDED
#include <windows.h>
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library
#include <gl\glaux.h>							// Header File For The GLaux Library
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_TTF.h>
#include "modelStructure.h"
#include "Vector3D.h"


class SDL_OpenGL_Graphics {
    private:
        Uint32 bgColor;
        SDL_Color textColor;
        SDL_Surface *textSurface;
        TTF_Font *font;
		bool InitGL(void);
		bool resizeWindow(int,int);
		
    public:
        SDL_Surface *screen;
        //SDL_OpenGL_Graphics(int w, int h, int bpp = 32, Uint32 flags = SDL_HWPALETTE|SDL_RESIZABLE|SDL_GL_DOUBLEBUFFER|SDL_OPENGL);     //INITIALISE SCREEN constructor

		SDL_OpenGL_Graphics(){}     //constructor overloaded
        ~SDL_OpenGL_Graphics();    //destructor

		void InitializeVideoMode(int w, int h, int bpp = 32, Uint32 flags = SDL_HWPALETTE|SDL_RESIZABLE|SDL_GL_DOUBLEBUFFER|SDL_OPENGL|SDL_FULLSCREEN);

        void ShowText(const char*, int x, int y);
        void SetTextColor(int, int, int);
        void SetBgColor(int, int, int);    //specify the background

        SDL_Surface* LoadImage(const char *filename);     //load image and optimise
        SDL_Surface* LoadImageTrans(const char *filename, int transR, int transG, int transB); //for colorkeying
        SDL_Surface* LoadImageAlpha(const char *filename); //for already containing alpha values

        void PutImage(SDL_Surface *src, SDL_Surface *dst,SDL_Rect *clip, int x, int y); //blit image on source
        void PutImage(SDL_Surface *src, int x, int y);

		///functions for 3d object manipulation

		ModelData* LoadModel(const char* filename);
	    GLuint* LoadTexture(const char* filename);
		void DrawModel(ModelData *object,GLuint *texture,int,int,GLfloat);
		void Translate_Rotate_and_Scale (Vector3D, double, Vector3D);



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

///Non class fucntions


 #endif
