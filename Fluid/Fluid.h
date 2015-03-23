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

#define IX(i, j) ((i) + (N+2) * (j))
#define EX(i, j) ((i) + (N) * (j))
#define SWAP(x0, x) {float * tmp = x0; x0 = x; x = tmp;}

class Fluid {
    
private:
    
    int N;
    
    int size;
    
    float *u, *v, *u_prev, *v_prev;
    
    float *dens, *dens_prev;
        
    float visc, diff, dt;
    
    Color * colors;
    
    
    void add_source(float * x, float * s, float dt);
    
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
    
    Color * getColors(uint32_t * _length) {*_length = N; return colors;}
    
    void addFluid(int x, int y, float s, float r);
    
    void addVelocity(int x, int y, Vector dir, float r);
    
    int getN()  {return N;}

};

#endif /* defined(__Fluid__Fluid__) */
