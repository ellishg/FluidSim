//
//  OnInit.cpp
//  
//
//  Created by Ellis Sparky Hoag on 6/15/14.
//
//

#include "World.h"

void World::OnLoop()    {
    
    fluid.addFluid(30, 30, 10, 20);
    
    fluid.addVelocity(10, 10, Vector(rand() % 10 / 10.f, 1), 3);
 
    fluid.OnLoop();
    
    fluid.prepareRender();
    
    colors = fluid.getColors(&cLength);
    
    if (simMode == WRITE_TO_FILE) {
        writeToFile();
    }
    
    frames++;
}

void World::writeToFile()   {
    
    uint32_t buffer[cLength * cLength];
    
    for (int i = 0; i < cLength * cLength; i++) {
        buffer[i] = colors[i].getBits();
    }
    
    fwrite(buffer, sizeof(uint32_t), cLength * cLength, myDataFile);
    
}