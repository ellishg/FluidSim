//
//  OnInit.cpp
//  
//
//  Created by Ellis Sparky Hoag on 6/15/14.
//
//

#include "World.h"


void World::OnRender()  {
    
#ifdef USING_SDL
    
    OnSDLRender();
#endif
    
}

void World::renderData()    {
    
    if (frames-- > 0) {
        uint32_t buffer[cLength * cLength];
        
        if  (fread(buffer, sizeof(uint32_t), cLength * cLength, myDataFile) != cLength * cLength)   {
            perror("Error reading file");
        }
        
        for (int i = 0; i < cLength * cLength; i++) {
            colors[i].setBits(buffer[i]);
        }
        
        OnSDLRender();
    }
    else    {
        running = false;
    }
}



#ifdef USING_SDL

void World::OnSDLRender()   {
    
    generateColorArrays();
    
    glLoadIdentity();
    
    glScalef(WORLD_SIZE, WORLD_SIZE, 0);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    
    glVertexPointer(2, GL_DOUBLE, 0, vertices);
    glColorPointer(3, GL_DOUBLE, 0, vertColors);
    
    //glDrawElements(GL_POINTS, 1 * (getN() ) * (getN() ), GL_UNSIGNED_INT, indices);
    glDrawArrays(GL_POINTS, 0, getN() * getN());
    
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
   // glClear(GL_COLOR_BUFFER_BIT);
    
//    int i, j;
//    
//    uint8_t r0, r1, r2, r3;
//    uint8_t g0, g1, g2, g3;
//    uint8_t b0, b1, b2, b3;
//    
//    for (i = 0; i < cLength-1; i++) {
//        for (j = 0; j < cLength-1; j++) {
//            
//            colors[i + j * cLength].getSpectrum(&r0, &g0, &b0);
//            colors[1 + i + j * cLength].getSpectrum(&r1, &g1, &b1);
//            colors[1 + i + (j + 1) * cLength].getSpectrum(&r2, &g2, &b2);
//            colors[i + (j + 1) * cLength].getSpectrum(&r3, &g3, &b3);
//            
//            glLoadIdentity();
//            
//            glTranslatef((float)i / cLength * WORLD_SIZE, (float)j / cLength * WORLD_SIZE, 0);
//            
//            glScalef(WORLD_SIZE / (cLength / 2.f), WORLD_SIZE / (cLength / 2.f), 0);
//            
//            glBegin(GL_QUADS);
//                glColor3ub(r0, g0, b0);
//                glVertex2f(0, 0);
//                glColor3ub(r1, g1, b1);
//                glVertex2f(1, 0);
//                glColor3ub(r2, g2, g2);
//                glVertex2f(1, 1);
//                glColor3ub(r3, g3, b3);
//                glVertex2f(0, 1);
//            glEnd();
//        }
//    }
    SDL_GL_SwapWindow(SDLWindow);
}
#endif

void World::generateColorArrays()    {
    
    int N = getN();
    
    for (int i = 0; i < 3 * N * N; i += 3) {
        
        uint8_t r, g, b;
        
        colors[i / 3].getSpectrum(&r, &g, &b);
        
        vertColors[i] = r / 255.f;
        vertColors[i + 1] = g / 255.f;
        vertColors[i + 2] = b / 255.f;
    }
    
}

void World::initDrawArray() {
    
    int N = getN();
    
    for (int i = 0; i < N * N; i++) {
        vertices[2 * i] = ((i % N) + 0.5) / N;
        vertices[2 * i + 1] = ((i / N) + 0.5) / N;
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