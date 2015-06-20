//
//  OnEvent.cpp
//  
//
//  Created by Ellis Sparky Hoag on 6/15/14.
//
//

#include "World.h"

void World::OnEvent()   {
    
    
#ifdef USING_SDL
    
    SDLEvent();
    
#endif
    
}


#ifdef USING_SDL

void World::SDLEvent()   {
    while (SDL_PollEvent(&Event)) {
        switch (Event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                SDLKeyDown(Event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                SDLKeyUp(Event.key.keysym.sym);
                break;
            case SDL_MOUSEMOTION:
                int x, y;
                SDL_GetMouseState(&x, &y);
                SDLMouseEvent(Event.button, x, y);
                break;
            default:
                break;
        }
    }
}

void World::SDLKeyDown(SDL_Keycode sym)   {
    
    switch (sym) {
        case SDLK_ESCAPE:
            running = false;
            break;
        case SDLK_SPACE:
            //fluid.addFluid(20, 20, 10, 10);
            break;
        case SDLK_LSHIFT:
            shiftKey = true;
            break;
        case SDLK_1:
            oneKey = true;
            break;
        case SDLK_2:
            twoKey = true;
            break;
        case SDLK_3:
            threeKey = true;
            break;
        default:
            break;
    }
}

void World::SDLKeyUp(SDL_Keycode sym) {
    
    switch (sym) {
        case SDLK_LSHIFT:
            shiftKey = false;
            break;
        case SDLK_1:
            oneKey = false;
            break;
        case SDLK_2:
            twoKey = false;
            break;
        case SDLK_3:
            threeKey = false;
            break;
        default:
            break;
    }
}

void World::SDLMouseEvent(SDL_MouseButtonEvent mouseButton, int x, int y) {
    
    x *= (float)gridWidth / SCREEN_SIZE_LENGTH;
    y *= (float)gridHeight / SCREEN_SIZE_HEIGHT;
    
    y = gridHeight - y - 1;
    
    //std::cout << (int)(mouseButton.button & SDL_BUTTON_LEFT) << ": " << x << ", " << y << "\n" ;
    
    if (mouseButton.button & SDL_BUTTON_LEFT) {
        if (oneKey) {
            fluid.addFluid(x, y, 5, 5, Color(255, 0, 0));
        }
        else if (twoKey) {
            fluid.addFluid(x, y, 5, 5, Color(0, 255, 0));
        }
        else if (threeKey) {
            fluid.addFluid(x, y, 5, 5, Color(0, 0, 255));
        }
        else if (shiftKey)  {
            fluid.addFluid(x, y, 5, 5, Color(128, 128, 128));
        }
        else {
            static int lastX = x;
            static int lastY = y;
            
            Vector r = Vector(x, y) - Vector(lastX, lastY);
            
            fluid.addVelocity(x, y, r, 5);
            
            lastX = x;
            lastY = y;
        }
    }
}
#endif