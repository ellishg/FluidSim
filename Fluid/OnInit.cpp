//
//  OnInit.cpp
//  
//
//  Created by Ellis Sparky Hoag on 6/15/14.
//
//

#include "World.h"

bool World::OnInit()    {
    
    char answer;
    
    char fileName[20];
    
    std::cout << "Would you like to save this sim,\nreplay another sim,\nor excecute a sim in realtime? (s/r/e): ";
    
    std::cin >> answer;
    
    if (answer == 's' || answer == 'S') {
        
        simMode = WRITE_TO_FILE;
        
        std::cout << "\nWhat would you like this file to be called? ";
        
        std::cin >> fileName;
        
        myDataFile = fopen(fileName, "wbx");
        
        if (myDataFile == NULL) {
            
            perror("Error opening file");
            
            running = false;
            
            return false;
        }
        
        uint32_t tmpHeaderData [2] = {0, 0};    //{N, frames}//
        
        fwrite(tmpHeaderData, sizeof(uint32_t), 2, myDataFile);
        
    }
    else if (answer == 'r' || answer == 'R') {
        
        simMode = READ_FILE;
        
        std::cout << "What is the name of the file you want to open? ";
        
        std::cin >> fileName;
        
        static int tries = 3;
        
        while (!(myDataFile = fopen(fileName, "rb")) && tries-- > 1) {
            
            std::cout << "\nThat file does not exist, please enter another file name. (You get " << tries << " more tries)";
            
            std::cin >> fileName;
        }
        
        if  (tries <= 0)    {
            
            std::cout << "Please learn to type! Goodbye!\n";
            
            myDataFile = NULL;
            
            running = false;
            
            return false;
        }
        
        uint32_t headerData [2];
        
        if (fread(headerData, sizeof(uint32_t), 2, myDataFile) != 2) {
            perror("Error reading file");
        }
        
        cLength = headerData[0];
        frames = headerData[1];
        
        colors = new Color[cLength * cLength];
        
        //std::cout << headerData[0] << ", " << headerData[1];
    
    }
    else if (answer == 'e' || answer == 'E') {
        
        simMode = REALTIME_SIM;
    }
    else {
    
        std::cout << "That was not an acceptable answer. Goodbye!\n";
        
        myDataFile = NULL;
        
        running = false;
        
        return false;
    }
    
    int N = getN();
    
    vertices = new GLdouble[N * N * 2];
    
    vertColors = new GLdouble[N * N * 3];
    
    //indices = new GLuint[4 * (N - 1) * (N - 1)];
    
    initDrawArray();

    
#ifdef USING_SDL
    
    if (!SDLInit()) {
        std::cout << "Error in SDLInit()\n";
        
        running =  false;
        
        return false;
    }
    
    if (!SDLGLInit()) {
        std::cout << "Error in SDLGLInit()\n";
        
        running = false;
        
        return false;
    }
#endif
    
    
    return true;
}

#ifdef USING_SDL

bool World::SDLInit()   {
    
    if(SDL_Init(SDL_INIT_VIDEO))   {
        printf("Error in SDL_Init(SDL_INIT_VIDEO)!\n");
        
        running = false;
        
        return false;
    }
    
    if((SDLWindow = SDL_CreateWindow("Fluid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)) == NULL)   {
        printf("Error in SDL_CreateWindow()!\n");
        
        running = false;
        
        return false;
    }
    
    return true;
}

bool World::SDLGLInit()    {
    
    if ((GLContext = SDL_GL_CreateContext(SDLWindow)) == NULL) {
        printf("Error in SDL_GL_CreateContext()!\n");
        
        running = false;
        
        return false;
    }
    
    glMatrixMode(GL_PROJECTION);
    
    glLoadIdentity();
    
    glOrtho(0, WORLD_SIZE, 0, WORLD_SIZE, 0, 1);
    
    glMatrixMode(GL_MODELVIEW);
    
    glClearColor(0, 0, 0, 1);
    
    //glEnable(GL_CULL_FACE);
    
    glDisable(GL_DEPTH_TEST);
    
    glPointSize(SCREEN_SIZE / getN());
    
    return true;
}
#endif