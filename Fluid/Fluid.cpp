//
//  Fluid.cpp
//  Fluid
//
//  Created by Ellis Sparky Hoag on 7/12/14.
//  Copyright (c) 2014 Ellis Sparky Hoag. All rights reserved.
//

#include "Fluid.h"

Fluid::Fluid()  {
    
    N = 150;
    
    size = (N+2) * (N+2);
    
    u = new float[size];
    v = new float[size];
    
    u_prev = new float[size];
    v_prev = new float[size];
    
    dens = new float[size];
    dens_prev = new float[size];
    
    colors = new Color[N * N];

    
    visc = 0.0001;  //viscosity
    
    diff = 0.001;   //degree of diffusion of density over time
    
    dt = 0.3;   //rate of change
    
}

Fluid::~Fluid() {
    
    delete[] u;
    delete[] v;
    delete[] u_prev;
    delete[] v_prev;
    delete[] dens;
    delete[] dens_prev;
}

void Fluid::OnLoop()    {
    
    vel_step(u, v, u_prev, v_prev, visc, dt);
    dens_step(dens, dens_prev, u, v, diff, dt);
    
    std::fill_n(dens_prev, size, 0);
}

void Fluid::prepareRender() {
    
//    const int numDataPoints = 10000;
//    
//    static float data[numDataPoints];
//    
//    static int x = 0;
    
    static float max = 0;
        
    int i, j;
    
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            
            float d = dens[IX(i+1, j+1)];
            
            if (d > max) {
                max = d;
            }
            
            float intensity = d * 255 / max;
            
            colors[EX(i, j)] = Color(intensity, intensity, intensity);
            
            //std::cout << d << ", ";
            
//            int r = d * 255;
//            int g = d / 2 * 255;
//            int b = d / 4 * 255;
            
            //colors[EX(i, j)] = Color(r, g, b);
            //colors[EX(i, j)] = Color(d*1000);
            //colors[EX(i, j)] = Color(i, j, 0);
            //colors[EX(i, j)].print();
            
            
//            if  (x < numDataPoints) {
//                data[x++] = d;
//            }
//            else    {
//                
//                float sum = 0;
//                //max = 0;
//                float min = 100000;
//                
//                for (int k = 0; k < numDataPoints; k++) {
//                    sum += data[k];
//                    
//                    if (data[k] > max) {
//                        max = data[k];
//                    }
//                    if (data[k] < min) {
//                        min = data[k];
//                    }
//                }
//                
//                float mean = sum / numDataPoints;
//                
//                float standardDeviation = 0;
//                
//                for (int k = 0; k < numDataPoints; k++) {
//                    standardDeviation += (data[k] - mean) * (data[k] - mean);
//                }
//                
//                standardDeviation /= numDataPoints - 1;
//                
//                std::cout << "\n\n\nMin: " << min << "\nMax: " << max << "\nMean: " << mean << "\nStDev: " << standardDeviation;
//                
//                x = 0;
//            }
        }
    }
}

void Fluid::add_source(float * x, float * s, float dt)  {
    
    for (int i = 0; i < size; i++) {
        x[i] += dt * s[i];
    }
}

void Fluid::diffuse(int b, float * x, float * x0, float diff, float dt) {
    
    float a = dt * diff * N * N;
    
    int i, j;
    
    for (int k = 0; k < 20; k++) {
        for (i = 1; i <= N; i++) {
            for (j = 1; j <= N; j++) {
                x[IX(i, j)] = (x0[IX(i, j)] + a * (x0[IX(i-1, j)] + x0[IX(i+1, j)] + x0[IX(i, j-1)] + x0[IX(i, j+1)])) / (1 + 4 * a);
            }
        }
    }
    
    set_bnd(b, x);
}


void Fluid::advect(int b, float * d, float * d0, float * u, float * v, float dt)    {
    
    int i0, j0, i1, j1, i, j;
    
    float x, y, s0, s1, t0, t1;
    
    float dt0 = N * dt;
    
    for (i = 1; i <= N; i++) {
        for (j = 1; j <= N; j++) {
            
            x = i - dt0 * u[IX(i, j)];
            
            y = j - dt0 * v[IX(i, j)];
            
            if (x < 0.5) {
                x = 0.5;
            }
            if (x > N + 0.5)    {
                x = N + 0.5;
            }
            
            i0 = (int)x;
            
            i1 = i0 + 1;
            
            if (y < 0.5) {
                y = 0.5;
            }
            if (y > N + 0.5)    {
                y = N + 0.5;
            }
            
            j0 = (int)y;
            
            j1 = j0 + 1;
            
            s1 = x - i0;
            
            s0 = 1 - s1;
            
            t1 = y - j0;
            
            t0 = 1 - t1;
            
            d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) + s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
            
        }
    }
    
    set_bnd(b, d);
}


void Fluid::dens_step(float * x, float * x0, float * u, float * v, float diff, float dt)    {
    
    add_source(x, x0, dt);
    
    SWAP(x0, x);
    
    diffuse(0, x, x0, diff, dt);
    
    SWAP(x0, x);
    
    advect(0, x, x0, u, v, dt);
}


void Fluid::vel_step(float * u, float * v, float * u0, float * v0, float visc, float dt)    {
    
    add_source(u, u0, dt);
    
    add_source(v, v0, dt);
    
    SWAP(u0, u);
    
    diffuse(1, u, u0, visc, dt);
    
    SWAP(v0, v);
    
    diffuse(2, v, v0, visc, dt);
    
    project(u, v, u0, v0);
    
    SWAP(u0, u);
    SWAP(v0, v);
        
    advect(1, u, u0, u0, v0, dt);
    advect(2, v, v0, u0, v0, dt);
    
    project(u, v, u0, v0);
}


void Fluid::project(float * u, float * v, float * p, float * div)   {
    
    float h = 1.f / N;
    
    int i, j;
    
    for (i = 1; i <= N; i++) {
        for (j = 1; j <= N; j++) {
            div[IX(i, j)] = -0.5 * h * (u[IX(i+1, j)] - u[IX(i-1, j)] + v[IX(i, j+1)] - v[IX(i, j-1)]);
            
            p[IX(i, j)] = 0;
        }
    }
    
    set_bnd(0, div);
    set_bnd(0, p);
    
    for (int k = 0; k < 20; k++) {
        for (i = 1; i <= N; i++) {
            for (j = 1; j <= N; j++) {
                p[IX(i, j)] = (div[IX(i, j)] + p[IX(i-1, j)] + p[IX(i+1, j)] + p[IX(i, j-1)] + p[IX(i, j+1)]) / 4.f;
            }
        }
        set_bnd(0, p);
    }
    
    for (i = 1; i <= N; i++) {
        for (j = 1; j <= N; j++) {
            u[IX(i, j)] -= 0.5f * (p[IX(i+1, j)] - p[IX(i-1, j)]) / h;
            v[IX(i, j)] -= 0.5f * (p[IX(i, j+1)] - p[IX(i, j-1)]) / h;
        }
    }
    
    set_bnd(1, u);
    set_bnd(2, v);
}

void Fluid::set_bnd(int b, float * x)   {
    
    for (int i = 1; i <= N; i++) {
        x[IX(0, i)] = (b == 1) ? -x[IX(1, i)] : x[IX(1, i)];
        x[IX(N+1, i)] = (b == 1) ? -x[IX(N, i)] : x[IX(N, i)];
        x[IX(i, 0)] = (b == 2) ? -x[IX(i, 1)] : x[IX(i, 1)];
        x[IX(i, N+1)] = (b == 2) ? -x[IX(i, N)] : x[IX(i, N)];
    }
    
    x[IX(0, 0)] = 0.5f * (x[IX(1, 0)]+ x[IX(0, 1)]);
    x[IX(0, N+1)] = 0.5f * (x[IX(1, N+1)]+ x[IX(0, N)]);
    x[IX(N+1, 0)] = 0.5f * (x[IX(N, 0)]+ x[IX(N+1, 1)]);
    x[IX(N+1, N+1)] = 0.5f * (x[IX(N, N+1)]+ x[IX(N+1, N)]);
    
}

void Fluid::setForceArea(float * x, int a, int b, float s, float r) {
    
    int i, j, dx, dy;
    
    float f;
    
    for (i = 1; i <= N; i++) {
        dx = a - i;
        
        for (j = 1; j <= N; j++) {
            dy = b - j;
            
            f = 1 - sqrtf(dx*dx + dy*dy) / r;
            
            if (f < 0) {
                f = 0;
            }
            else if(f > 1)  {
                f = 1;
            }
            
            x[IX(i, j)] += f * s;
        }
    }
}


void Fluid::addFluid(int x, int y, float s, float r)   {
    
    assert(x < N && y < N);
    
    setForceArea(dens_prev, x, y, s, r);
}

void Fluid::addVelocity(int x, int y, Vector dir, float r)   {
    
    assert(x < N && y < N);
    
    setForceArea(u_prev, x, y, dir.x, r);
    
    setForceArea(v_prev, x, y, dir.y, r);
}

