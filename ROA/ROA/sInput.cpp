#include "sInput.h"
#include <SDL.h>

SDL_Input::SDL_Input() {
    for(int i = 0; i < 323; i++) {
            keystate[i] = false;
    }
    quit = false;
}

void SDL_Input::ProcessEvents(void) {

    static SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYDOWN:
                keystate[event.key.keysym.sym] = true;
                break;

            case SDL_KEYUP:
                keystate[event.key.keysym.sym] = false;
                break;

            case SDL_MOUSEMOTION:
                mouse.x = event.motion.x;
                mouse.y = event.motion.y;
				mouse.xrel = event.motion.xrel;
				mouse.yrel = event.motion.yrel;
                break;

            case SDL_MOUSEBUTTONDOWN:
               mouse.button[event.button.button - 1] = true;
               mouse.x = event.button.x;
               mouse.y = event.button.y;
                break;

            case SDL_MOUSEBUTTONUP:
                mouse.button[event.button.button - 1] = false;
                mouse.x = event.button.x;
                mouse.y = event.button.y;
                break;

            case SDL_QUIT:
                quit = true;
                break;

            default: break;
                }

            }


    }


bool* SDL_Input::GetKeyState(void) {
    return keystate;
}

MouseInfo* SDL_Input::GetMouseState(void) {
    return &mouse;
}
