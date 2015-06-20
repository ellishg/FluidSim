//
//  OnInit.cpp
//  
//
//  Created by Ellis Sparky Hoag on 6/15/14.
//
//

#include "World.h"

void World::OnLoop()    {
    
    fluid.addFluid(20, 20, 4, 8, Color(255, 0, 0));
    fluid.addFluid(gridWidth / 2, gridHeight - 20, 4, 8, Color(0, 255, 0));
    fluid.addFluid(gridWidth - 20, 20, 4, 8, Color(0, 0, 255));
    
    fluid.addVelocity(20, 15, Vector(rand() % 10 / 10.f, 1), 5);
    fluid.addVelocity(gridWidth / 2, gridHeight - 20, Vector((rand() % 10 / 10.f) - 0.5f, -1), 5);
    fluid.addVelocity(gridWidth - 20, 20, Vector(rand() % 10 / -10.f, 1), 5);
 
    fluid.OnLoop();
    
    fluid.prepareRender();
    
    colors = fluid.getColors();
    
    if (simMode == WRITE_TO_FILE) {
        writeToFile();
    }
    
    frames++;
}

void World::writeToFile()   {
    
    uint32_t buffer[gridWidth * gridHeight];
    
    for (int i = 0; i < gridWidth * gridHeight; i++) {
        buffer[i] = colors[i].getBits();
    }
    
    fwrite(buffer, sizeof(uint32_t), gridWidth * gridHeight, myDataFile);
    
}