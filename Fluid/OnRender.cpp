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
        uint32_t buffer[gridWidth * gridHeight];
        
        if  (fread(buffer, sizeof(uint32_t), gridWidth * gridHeight, myDataFile) != gridWidth * gridHeight)   {
            perror("Error reading file");
        }
        
        for (int i = 0; i < gridWidth * gridHeight; i++) {
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
    
    //glLoadIdentity();
    
    //glScalef(WORLD_SIZE, WORLD_SIZE, 0);
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    
    glVertexPointer(2, GL_DOUBLE, 0, vertices);
    glColorPointer(3, GL_DOUBLE, 0, vertColors);
    
    glDrawArrays(GL_POINTS, 0, gridWidth * gridHeight);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    
    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(3);
    
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5, 0);
    
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 5, (void*)(4 * 2));
        
    //glDrawElements(GL_QUADS, getN() * getN() * 4, GL_UNSIGNED_INT, indices);
    
    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(3);
    
    
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
    
    //int N = getN();
    
    for (int i = 0; i < gridWidth * gridHeight; i++) {
        
        uint8_t r, g, b;
        
        colors[i].getSpectrum(&r, &g, &b);
        
        vertColors[3 * i] = r / 255.f;
        vertColors[3 * i + 1] = g / 255.f;
        vertColors[3 * i + 2] = b / 255.f;
        
        //vertDataArray[5 * i + 2] = r / 255.f;
        //vertDataArray[5 * i + 3] = g / 255.f;
        //vertDataArray[5 * i + 4] = b / 255.f;
        
    }
    
}