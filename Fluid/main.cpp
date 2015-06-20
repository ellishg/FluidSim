#include "World.h"

World::World()  {
    
    running = true;
    
    frames = 0;
    
    gridWidth = fluid.getGridWidth();
    gridHeight = fluid.getGridHeight();
    N = fluid.getN();
}

int main()  {
        
    World world;
    
    srand((unsigned int)time(NULL));
    
    
    if (!world.OnInit()) {
        std::cout << "Error OnInit!\n";
        return 0;
    }
    
    while (world.isRunning()) {
        
        unsigned long prevTime = GET_TICKS;
        
        if (world.simMode != READ_FILE) {
            
            world.OnLoop();
        
            world.OnRender();
        }
        else    {
            world.renderData();
        }
        
        do {
            world.OnEvent();
            
        } while (GET_TICKS - prevTime < 1000 / FRAMES_PER_SECOND);
        
        char titleStr[32];  //this should be long enough for my title
        
        sprintf(titleStr, "Fluid FPS: %d", (int)(1000.f / (GET_TICKS - prevTime)));
        
        world.setWindowTitle(titleStr);
        
        //std::cout << 1000 / (GET_TICKS - prevTime) << ", ";
    }
    
    if (!world.OnCleanup()) {
        std::cout << "Error OnCleanup!\n";
        return 0;
    }
    
    return 0;
}