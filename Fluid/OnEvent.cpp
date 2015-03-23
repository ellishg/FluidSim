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
        default:
            break;
    }
}

void World::SDLKeyUp(SDL_Keycode sym) {
    
    switch (sym) {
        case SDLK_LSHIFT:
            shiftKey = false;
            break;
        default:
            break;
    }
}

void World::SDLMouseEvent(SDL_MouseButtonEvent mouseButton, int x, int y) {
    
    x *= (float)getN() / SCREEN_SIZE;
    y *= (float)getN() / SCREEN_SIZE;
    
    y = getN() - y;
    
    //std::cout << (int)(mouseButton.button & SDL_BUTTON_LEFT) << ": " << x << ", " << y << "\n" ;
    if (shiftKey && (mouseButton.button & SDL_BUTTON_LEFT)) {
        fluid.addFluid(x, y, 10, 10);
    }
    else if (mouseButton.button & SDL_BUTTON_LEFT) {
        static int lastX = x;
        static int lastY = y;
        
        Vector r = Vector(x, y) - Vector(lastX, lastY);
        
        fluid.addVelocity(x, y, r, 1);
        
        lastX = x;
        lastY = y;
    }
}
#endif