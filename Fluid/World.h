//
//  World.h
//  
//
//  Created by Ellis Sparky Hoag on 6/15/14.
//
//

#ifndef ____World__
#define ____World__

#define USING_SDL

#define FRAMES_PER_SECOND 30

#define SCREEN_SIZE 600
#define SCREEN_SIZE_LENGTH SCREEN_SIZE
#define SCREEN_SIZE_HEIGHT (SCREEN_SIZE * (float)gridHeight / gridWidth)

#define WORLD_SIZE_LENGTH 1
#define WORLD_SIZE_HEIGHT ((float)gridHeight / gridWidth)

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "Vector.h"
#include "Color.h"
#include "Fluid.h"


#ifdef USING_SDL

    #include "SDL2/SDL.h"
    #include "SDL2/SDL_opengl.h"

    #include "GLUT/glut.h"

    #define GET_TICKS SDL_GetTicks()
#endif

enum SIM_MODE{
    WRITE_TO_FILE,
    READ_FILE,
    REALTIME_SIM
};

class World {
    
private:
    
    bool running;
    
    bool shiftKey;
    
    bool oneKey, twoKey, threeKey;
    
    Fluid fluid;
    
    FILE * myDataFile;
    
    uint32_t frames;
    
    Color * colors;
        
    GLdouble * vertices;
    
    GLdouble * vertColors;
    
    //GLfloat * vertDataArray;
    
    //GLuint * indices;
    
    //GLuint vertexBuffer;
    
    //int getN()  {return (simMode == READ_FILE) ? cLength : fluid.getN();}
    
    uint32_t gridWidth, gridHeight, N;
    
    
#ifdef USING_SDL
    
    SDL_Window * SDLWindow;
    
    SDL_GLContext GLContext;
    
    SDL_Event Event;
#endif
    
public:
    
    World();
        
    bool OnInit();
    
    void initDrawArray();
    
#ifdef USING_SDL
    bool SDLInit();
    
    bool SDLGLInit();
#endif
    
    void OnLoop();
    
    void OnRender();
    
    void writeToFile();
    
    void renderData();
    
    void generateColorArrays();
    
    
#ifdef USING_SDL
    
    void OnSDLRender();
#endif
    
    void OnEvent();
    
    
#ifdef USING_SDL
    
    void SDLEvent();
    
    void SDLKeyDown(SDL_Keycode sym);
    
    void SDLKeyUp(SDL_Keycode sym);
    
    void SDLMouseEvent(SDL_MouseButtonEvent mouseButton, int x, int y);
    
    void setWindowTitle(const char * title) {SDL_SetWindowTitle(SDLWindow, title);}
#endif
    
    bool OnCleanup();
    
    bool isRunning()    {return running;}
    
    SIM_MODE simMode;
};

#endif /* defined(____World__) */
