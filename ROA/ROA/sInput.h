#ifndef SINPUT_H_INCLUDED
#define SINPUT_H_INCLUDED

#define MU_LEFT 0
#define MU_MIDDLE 1
#define MU_RIGHT 2
#define MU_WHEEL_UP 3
#define MU_WHEEL_DOWN 4

typedef struct MouseInfo {
    int x, y;
    int xrel, yrel;
    bool button[5];

        MouseInfo() 
		{
            for(int i = 0; i < 5; i++) {
                button[i] = false;
            }
        }

}MouseInfo;

class SDL_Input {
    private:
        bool keystate[323];
        bool quit;
        MouseInfo mouse;

    public:
        SDL_Input();

        void ProcessEvents();
        bool* GetKeyState();

        MouseInfo* GetMouseState();

        inline bool QuitRequest() {
            return quit;
        }
};

#endif // SINPUT_H_INCLUDED
