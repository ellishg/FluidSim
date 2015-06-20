//
//  Fluid.h
//  Fluid
//
//  Created by Ellis Sparky Hoag on 7/12/14.
//  Copyright (c) 2014 Ellis Sparky Hoag. All rights reserved.
//

#ifndef __Fluid__Fluid__
#define __Fluid__Fluid__

#include <assert.h>

#include "Vector.h"
#include "Color.h"

#define IX(i, j) ((i) + (gridWidth + 2) * (j))
#define EX(i, j) ((i) + (gridWidth) * (j))
#define SWAP(x0, x) {float * tmp = x0; x0 = x; x = tmp;}

#define MAX_DENSITY 10

class Fluid {
    
private:
    
    int N;
    int gridWidth, gridHeight;
    
    int size;
    
    float *u, *v, *u_prev, *v_prev;
    
    float *dens_r, *dens_g, *dens_b, *dens_prev_r, *dens_prev_g, *dens_prev_b;
        
    float visc, diff, dt;
    
    Color * colors;
    
    
    void add_source(float * x, float * s, float dt, bool IsDensity);
    
    void diffuse(int b, float * x, float * x0, float diff, float dt);
    
    void advect(int b, float * d, float * d0, float * u, float * v, float dt);
    
    void dens_step(float * x, float * x0, float * u, float * v, float diff, float dt);
    
    void vel_step(float * u, float * v, float * u0, float * v0, float visc, float dt);
    
    void project(float * u, float * v, float * p, float * div);
    
    void set_bnd(int b, float * x);
    
    void setForceArea(float * u, int a, int b, float s, float r);

    
public:
    
    Fluid();
    
    ~Fluid();
    
    void OnLoop();
    
    void prepareRender();
    
    Color * getColors() {return colors;}
    
    void addFluid(int x, int y, float strength, float radius, Color color);
        
    void addVelocity(int x, int y, Vector dir, float r);
    
    int getGridWidth()  {return gridWidth;}
    
    int getGridHeight() {return gridHeight;}
    
    int getN()  {return N;}

};

#endif /* defined(__Fluid__Fluid__) */
