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
        uint32_t headerBuffer[2] = {cLength, frames};
        
        rewind(myDataFile);
        
        fwrite(headerBuffer, sizeof(uint32_t), 2, myDataFile);
    }
    
    if (simMode == WRITE_TO_FILE || simMode == READ_FILE) {
        fclose(myDataFile);
    }
    
    delete [] colors;
    
    delete [] vertColors;
    
    delete [] vertices;
    
    //delete [] indices;
    
    return true;
}