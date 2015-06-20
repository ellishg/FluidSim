//
//  OnInit.cpp
//  
//
//  Created by Ellis Sparky Hoag on 6/15/14.
//
//

#include "World.h"

bool World::OnCleanup() {
    
    if (simMode == WRITE_TO_FILE && myDataFile != NULL) {
        uint32_t headerBuffer[3] = {gridWidth, gridHeight, frames};
        
        rewind(myDataFile);
        
        fwrite(headerBuffer, sizeof(uint32_t), 3, myDataFile);
    }
    
    if (simMode == WRITE_TO_FILE || simMode == READ_FILE) {
        fclose(myDataFile);
    }
    
    delete [] colors;
    
    delete [] vertColors;
    
    delete [] vertices;
    
    //delete [] vertDataArray;
    
    //delete [] indices;
    
    //glDeleteBuffers(1, &vertexBuffer);
    
    return true;
}