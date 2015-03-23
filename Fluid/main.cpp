#include "World.h"

World::World()  {
    
    running = true;
    
    frames = 0;
}

int main()  {
        
    World world;
    
    //srand(time(NULL));
    
    
    if (!world.OnInit()) {
        std::cout << "Error OnInit!\n";
        //return 0;
    }
    
    if (world.simMode != READ_FILE) {
        
        while (world.isRunning()) {
            
            unsigned long prevTime = GET_TICKS;
            
            world.OnLoop();
            
            do {
                world.OnEvent();
                
                world.OnRender();
                
            } while (GET_TICKS - prevTime < 1000 / FRAMES_PER_SECOND);
            
            //std::cout << 1000 / (GET_TICKS - prevTime) << ", ";
        }
    }
    else    {
        while (world.isRunning()) {
            
            unsigned long prevTime = GET_TICKS;
            
            world.renderData();
            
            do {
                world.OnEvent();
            } while (GET_TICKS - prevTime < 1000 / FRAMES_PER_SECOND);
            
            //std::cout << 1000.f / (GET_TICKS - prevTime) << ", ";

        }
    }
    
    if (!world.OnCleanup()) {
        std::cout << "Error OnCleanup!\n";
        //return 0;
    }
    
    return 0;
}