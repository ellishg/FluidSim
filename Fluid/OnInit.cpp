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
    
    std::cout << "Would you like to:\nsave this sim (s)\nreplay another sim (r)\nor excecute a sim in realtime (e) ";
    
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
        
        uint32_t tmpHeaderData [3] = {gridWidth, gridHeight, 0};    //{Nx, Ny, frames}//
        
        fwrite(tmpHeaderData, sizeof(uint32_t), 3, myDataFile);
        
    }
    else if (answer == 'r' || answer == 'R') {
        
        simMode = READ_FILE;
        
        std::cout << "What is the name of the file you want to open? ";
        
        std::cin >> fileName;
        
        static int tries = 3;
        
        while (!(myDataFile = fopen(fileName, "rb")) && tries-- >= 1) {
            
            std::cout << "\nThat file does not exist, please enter another file name. (You get " << tries << " more tries) ";
            
            std::cin >> fileName;
        }
        
        if  (tries <= 0)    {
            
            std::cout << "Please learn to type! Goodbye!\n";
            
            myDataFile = NULL;
            
            running = false;
            
            return false;
        }
        
        uint32_t headerData [3];
        
        if (fread(headerData, sizeof(uint32_t), 3, myDataFile) != 3) {
            perror("Error reading file");
        }
        
        gridWidth = headerData[0];
        gridHeight = headerData[1];
        frames = headerData[2];
        
        colors = new Color[gridWidth * gridHeight];
        
        //std::cout << headerData[0] << ", " << headerData[1] << ", " << headerData[2];
    
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
    
    //int N = getN();
    
    vertices = new GLdouble[gridWidth * gridHeight * 2];
    
    vertColors = new GLdouble[gridWidth * gridHeight * 3];
    
    //vertDataArray = new GLfloat[N * N * 5]; //X, Y, R, G, B
    
    //indices = new GLuint[(N - 1) * (N - 1) * 4];   //4 = 4 vertices per square
        
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
    
    
    //glGenBuffers(1, &vertexBuffer);
    //glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * N * N * 5, vertDataArray, GL_STATIC_DRAW);
    
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5, 0);
    //glEnableVertexAttribArray(0);
    
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5, (void*)(4 * 2));
    //glEnableVertexAttribArray(1);
    
    
    return true;
}

#ifdef USING_SDL

bool World::SDLInit()   {
    
    if(SDL_Init(SDL_INIT_VIDEO))   {
        printf("Error in SDL_Init(SDL_INIT_VIDEO)!\n");
        
        running = false;
        
        return false;
    }
    
    if((SDLWindow = SDL_CreateWindow("Fluid FPS:", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_SIZE_LENGTH, SCREEN_SIZE_HEIGHT, SDL_WINDOW_OPENGL)) == NULL)   {
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
    
    glOrtho(0, WORLD_SIZE_LENGTH, 0, WORLD_SIZE_HEIGHT, 0, 1);
    
    glMatrixMode(GL_MODELVIEW);
    
    glClearColor(0, 0, 0, 1);
    
    //glEnable(GL_CULL_FACE);
    
    glDisable(GL_DEPTH_TEST);
    
    /*  
     *  This needs to change if I want a window that is not a square
     *  Also, if SCREEN_SIZE / N doesn't compute to an integer or nice
     *  rational number then you will see black lines inbetween 'pixels'
     *  due to rounding errors
     */
    glPointSize((GLdouble)SCREEN_SIZE / N);
        
    glScalef(WORLD_SIZE_LENGTH, WORLD_SIZE_HEIGHT, 0);
    
    return true;
}
#endif

void World::initDrawArray() {
    
    //int N = getN();
    
//    int i, j;
//    
//    int k = 0;
//    int h = 0;
//    
//    for (i = 0; i < N; i++) {
//        for (j = 0; j < N; j++) {
//            
//            /*X-coord*/
//            vertDataArray[5 * k + 0] = i / N;
//            /*Y-coord*/
//            vertDataArray[5 * k + 1] = j / N;
//            
//            /*RBG*/
//            vertDataArray[5 * k + 2] = 0.0;
//            vertDataArray[5 * k + 3] = 0.0;
//            vertDataArray[5 * k + 4] = 0.0;
//            
//            if ((i != N - 1) && (j != N - 1)) {
//                indices[4 * h + 0] = k;
//                indices[4 * h + 1] = k + 1;
//                indices[4 * h + 2] = k + N + 1;
//                indices[4 * h + 3] = k + N;
//                
//                
//                h++;
//            }
//            
//            k++;
//        }
//    }
    
    for (int i = 0; i < gridWidth * gridHeight; i++) {
        vertices[2 * i] = ((i % gridWidth) + 0.5) / gridWidth;
        vertices[2 * i + 1] = ((i / gridWidth) + 0.5) / gridHeight;
    }
    
    //    for (int i = 0; i < N * N; i++) {
    //        indices[i] = i;
    //    }
    //
    //    for (int i = 0; i < N - 1; i++) {
    //        for (int j = 0; j < N - 1; j++) {
    //            indices[4 * (i + j * (N - 1))] = i + j * (N );
    //            indices[4 * (i + j * (N - 1)) + 1] = i + (j + 1) * (N );
    //            indices[4 * (i + j * (N - 1)) + 2] = i + 1 + (j + 1) * (N );
    //            indices[4 * (i + j * (N - 1)) + 3] = i + 1 + j * (N );
    //        }
    //    }
    
    //    for (int i = 0; i < (N - 1) * (N - 1); i++) {
    //        indices[4 * i] = i;
    //        indices[4 * i + 1] = (i + N);
    //        indices[4 * i + 2] = (i + N + 1);
    //        indices[4 * i + 3] = (i + 1);
    //
    //        //std::cout << 4 * i << ": ";
    //
    //        //std::cout << 2 * i << ", " << 2 * (i + N) << ", " << 2 * (i + N + 1) << ", " << 2 * (i + 1) << "\n";
    //    }
    
    //    int x = 0;
    //
    //    for (int i = 0; i < N - 1; i++) {
    //        for (int j = 0; j < N - 1; j++) {
    //            indices[x++] = x;//2 * i + j * (N-1);
    //            indices[x++] = x;//(2 * i + j * (N-1)) + N - 1;
    //            indices[x++] = x;//(2 * i + j * (N-1)) + N;
    //            indices[x++] = x;//(2 * i + j * (N-1)) + 1;
    //        }
    //    }
    
    
    //    int x = 0;
    //
    //    const int numberMovementsInRow = 4 * N - 3;
    //
    //    const int numberRows = N - 1;
    //
    //    int incrementType;
    //
    //    int incrementDirection;
    //
    //    int increment = 0;
    //
    //    for (int i = 0; i < 4 * N * N - 7 * N + 4; i++) {
    //        indices[i] = x;
    //
    //        if (((int)i / numberRows) % 2 == 0) {
    //            //is moving right//
    //            incrementDirection = 1;
    //        }
    //        else    {
    //            //is moving left//
    //            incrementDirection = -1;
    //        }
    //
    //        incrementType = (i % numberMovementsInRow) % 4;
    //
    //        switch (incrementDirection) {
    //            case 1:
    //                switch (incrementType) {
    //                    case 0:
    //                        increment = N;
    //                        break;
    //                    case 1:
    //                        increment = 1;
    //                        break;
    //                    case 2:
    //                        increment = -N - 1;
    //                        break;
    //                    case 3:
    //                        increment = 1;
    //                        break;
    //                    default:
    //                        break;
    //                }
    //                break;
    //            case -1:
    //                switch (incrementType) {
    //                    case 0:
    //                        increment = -N;
    //                        break;
    //                    case 1:
    //                        increment = -1;
    //                        break;
    //                    case 2:
    //                        increment = -N + 1;
    //                        break;
    //                    case 3:
    //                        increment = -1;
    //                        break;
    //                    default:
    //                        break;
    //                }
    //
    //                break;
    //            default:
    //                break;
    //        }
    //        
    //        assert(increment != 0);
    //        
    //        x += increment;
    //    }
}