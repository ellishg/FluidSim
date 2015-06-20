//
//  Fluid.cpp
//  Fluid
//
//  Created by Ellis Sparky Hoag on 7/12/14.
//  Copyright (c) 2014 Ellis Sparky Hoag. All rights reserved.
//

#include "Fluid.h"

Fluid::Fluid()  {
    
    N = 100;
    
    /*
     *  If you want a screen other than a boring square you
     *  need to remember that I can only draw square points.
     *  So the dimensions need to be rational or something...
     */
    
    gridWidth = N;
    gridHeight = N;
    
    size = (gridWidth + 2) * (gridHeight + 2);
    
    u = new float[size];
    v = new float[size];
    
    u_prev = new float[size];
    v_prev = new float[size];
    
    dens_r = new float[size];
    dens_g = new float[size];
    dens_b = new float[size];
    dens_prev_r = new float[size];
    dens_prev_g = new float[size];
    dens_prev_b = new float[size];
    
    colors = new Color[gridWidth * gridHeight];

    
    visc = 0.0001;  //viscosity
    
    diff = 0.001;   //degree of diffusion of density over time
    
    dt = 0.1;   //rate of change
    
}

Fluid::~Fluid() {
    
    /*delete[] u;
    delete[] v;
    delete[] u_prev;
    delete[] v_prev;
    delete[] dens;
    delete[] dens_prev;*/
}

void Fluid::OnLoop()    {
    
    /*
     *  dens_prev should have been modified by OnEvent and represents
     *  the source densities. Same for u_prev and v_prev
     */
    
    vel_step(u, v, u_prev, v_prev, visc, dt);
    dens_step(dens_r, dens_prev_r, u, v, diff, dt);
    dens_step(dens_g, dens_prev_g, u, v, diff, dt);
    dens_step(dens_b, dens_prev_b, u, v, diff, dt);

    
    std::fill_n(dens_prev_r, size, 0);
    std::fill_n(dens_prev_g, size, 0);
    std::fill_n(dens_prev_b, size, 0);
    std::fill_n(u_prev, size, 0);
    std::fill_n(v_prev, size, 0);
}

void Fluid::prepareRender() {
    
//    const int numDataPoints = 10000;
//    
//    static float data[numDataPoints];
//    
//    static int x = 0;
//    
//    static float max = 0;
    
    int i, j;
    
    for (i = 0; i < gridWidth; i++) {
        for (j = 0; j < gridHeight; j++) {
            
            //float d = dens[IX(i+1, j+1)];
            
            //if (d > max) {
            //    max = d;
            //}
            
            //float intensity = d * 255 / MAX_DENSITY;
            
            float r = dens_r[IX(i+1, j+1)] * 255 / MAX_DENSITY;
            float g = dens_g[IX(i+1, j+1)] * 255 / MAX_DENSITY;
            float b = dens_b[IX(i+1, j+1)] * 255 / MAX_DENSITY;

            
            colors[EX(i, j)] = Color(r, g, b);
            
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
//                max = 0;
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

void Fluid::add_source(float * x, float * s, float dt, bool IsDensity)  {
    
    for (int i = 0; i < size; i++) {
        x[i] += dt * s[i];
        
        if (IsDensity && x[i] > MAX_DENSITY) {
            x[i] = MAX_DENSITY;
        }
    }
}

void Fluid::diffuse(int b, float * x, float * x0, float diff, float dt) {
    
    /*
     * Diffuse fluid in and out of a given cell
     * Diffusion goes to and from each of its four neighbors
     * Using Guass-Seidel relaxation solver?? Cool
     */
    
    float a = dt * diff * gridWidth * gridHeight;
    
    int i, j;
    
    int mid, left, right, top, bottom;
    
    for (int k = 0; k < 20; k++) {
        for (i = 1; i <= gridWidth; i++) {
            for (j = 1; j <= gridHeight; j++) {
                
                mid = IX(i, j);
                left = mid - 1; //IX(i - 1, j)
                right = mid + 1; //IX(i + 1, j)
                top = mid + gridWidth + 2; //IX(i, j + 1)
                bottom = mid - gridWidth - 2; //IX(i, j - 1)
                
                x[mid] = (x0[mid] + a * (x0[left] + x0[right] + x0[bottom] + x0[top])) / (1 + 4 * a);
            }
        }
    }
    
    set_bnd(b, x);
}


void Fluid::advect(int b, float * d, float * d0, float * u, float * v, float dt)    {
    
    /*
     *  Calculate movement of fluid through vector field.
     *  Solves backwards by figuring out where fluid would have come from for each cell
     *  Liner backtrace?? Also cool
     */
    
    int i0, j0, i1, j1, i, j;
    
    float x, y, s0, s1, t0, t1;
    
    float dt0Width = gridWidth * dt;
    float dt0Height = gridHeight * dt;
    
    for (i = 1; i <= gridWidth; i++) {
        for (j = 1; j <= gridHeight; j++) {
            
            x = i - dt0Width * u[IX(i, j)];
            
            y = j - dt0Height * v[IX(i, j)];
            
            if (x < 0.5) {
                x = 0.5;
            }
            if (x > gridWidth + 0.5)    {
                x = gridWidth + 0.5;
            }
            
            i0 = (int)x;
            
            i1 = i0 + 1;
            
            if (y < 0.5) {
                y = 0.5;
            }
            if (y > gridHeight + 0.5)    {
                y = gridHeight + 0.5;
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
    
    /*
     *  We assume that all source densities are initially contained in x0
     */
    
    add_source(x, x0, dt, true);
    
    //SWAP(x0, x);
    
    diffuse(0, x0, x, diff, dt);
    
    //SWAP(x0, x);
    
    advect(0, x, x0, u, v, dt);
}


void Fluid::vel_step(float * u, float * v, float * u0, float * v0, float visc, float dt)    {
    
    /*
     *  We assume that all source forces are initially contained in v0 and u0
     */
    
    add_source(u, u0, dt, false);
    
    add_source(v, v0, dt, false);
    
    //SWAP(u0, u);
    
    diffuse(1, u0, u, visc, dt);
    
    //SWAP(v0, v);
    
    diffuse(2, v0, v, visc, dt);
    
    project(u0, v0, u, v);
    
    //SWAP(u0, u);
    //SWAP(v0, v);
        
    advect(1, u, u0, u0, v0, dt);
    advect(2, v, v0, u0, v0, dt);
    
    project(u, v, u0, v0);
}


void Fluid::project(float * u, float * v, float * p, float * div)   {
    
    /*
     *  The purpose of this is to make the vector field mass conserving
     *  It makes the field nice and swirly
     *  Poisson Equation?? Very cool
     */
    
    float width = 1.f / gridWidth;
    float height = 1.f / gridHeight;
    
    int i, j;
    
    int mid, left, right, top, bottom;
    
    for (i = 1; i <= gridWidth; i++) {
        for (j = 1; j <= gridHeight; j++) {
            
            mid = IX(i, j);
            left = mid - 1; //IX(i - 1, j)
            right = mid + 1; //IX(i + 1, j)
            top = mid + gridWidth + 2; //IX(i, j + 1)
            bottom = mid - gridWidth - 2; //IX(i, j - 1)
            
            div[mid] = -0.5 * (width * (u[right] - u[left]) + height * (v[top] - v[bottom]));
            
            p[mid] = 0;
        }
    }
    
    set_bnd(0, div);
    set_bnd(0, p);
    
    for (int k = 0; k < 20; k++) {
        for (i = 1; i <= gridWidth; i++) {
            for (j = 1; j <= gridHeight; j++) {
                
                mid = IX(i, j);
                left = mid - 1; //IX(i - 1, j)
                right = mid + 1; //IX(i + 1, j)
                top = mid + gridWidth + 2; //IX(i, j + 1)
                bottom = mid - gridWidth - 2; //IX(i, j - 1)
                
                p[mid] = (div[mid] + p[left] + p[right] + p[bottom] + p[top]) / 4.f;
            }
        }
        set_bnd(0, p);
    }
    
    for (i = 1; i <= gridWidth; i++) {
        for (j = 1; j <= gridHeight; j++) {
            
            mid = IX(i, j);
            left = mid - 1; //IX(i - 1, j)
            right = mid + 1; //IX(i + 1, j)
            top = mid + gridWidth + 2; //IX(i, j + 1)
            bottom = mid - gridWidth - 2; //IX(i, j - 1)

            
            u[mid] -= 0.5f * (p[right] - p[left]) / width;
            v[mid] -= 0.5f * (p[top] - p[bottom]) / height;
        }
    }
    
    set_bnd(1, u);
    set_bnd(2, v);
}

void Fluid::set_bnd(int b, float * x)   {
    
    for (int i = 1; i <= gridHeight; i++) {
        x[IX(0, i)] = (b == 1) ? -x[IX(1, i)] : x[IX(1, i)];
        x[IX(gridWidth+1, i)] = (b == 1) ? -x[IX(gridWidth, i)] : x[IX(gridWidth, i)];
    }
    for (int i = 1; i <= gridWidth; i++) {
        x[IX(i, 0)] = (b == 2) ? -x[IX(i, 1)] : x[IX(i, 1)];
        x[IX(i, gridHeight+1)] = (b == 2) ? -x[IX(i, gridHeight)] : x[IX(i, gridHeight)];
    }
    
    x[IX(0, 0)] = 0.5f * (x[IX(1, 0)]+ x[IX(0, 1)]);
    x[IX(0, gridHeight+1)] = 0.5f * (x[IX(1, gridHeight+1)]+ x[IX(0, gridHeight)]);
    x[IX(gridWidth+1, 0)] = 0.5f * (x[IX(gridWidth, 0)]+ x[IX(gridWidth+1, 1)]);
    x[IX(gridWidth+1, gridHeight+1)] = 0.5f * (x[IX(gridWidth, gridHeight+1)]+ x[IX(gridWidth+1, gridHeight)]);
    
}

void Fluid::setForceArea(float * x, int a, int b, float s, float r) {
    
    //assert(a < N && b < N);
    
    int i, j, dx, dy;
    
    float f;
    
    int minX = (a - ceilf(r) > 0) ? a - ceilf(r) : 0;
    int maxX = (a + ceilf(r) < gridWidth) ? a + ceilf(r) : gridWidth;
    int minY = (b - ceilf(r) > 0) ? b - ceilf(r) : 0;
    int maxY = (b + ceilf(r) < gridHeight) ? b + ceilf(r) : gridHeight;
    
    for (i = minX; i <= maxX; i++) {
        dx = a - i;
        
        for (j = minY; j <= maxY; j++) {
            dy = b - j;
            
            f = 1 - sqrtf(dx*dx + dy*dy) / r;
            
            if (f < 0) {
                f = 0;
            }
            
            //assert(f <= 1);
            //else if(f > 1)  {
            //    f = 1;
            //}
            
            x[IX(i, j)] += f * s;
        }
    }
}

void Fluid::addFluid(int x, int y, float strength, float radius, Color color)   {
    
    uint8_t red, green, blue;
    
    color.getSpectrum(&red, &green, &blue);
    
    setForceArea(dens_prev_r, x, y, strength * red / 255.f, radius);
    setForceArea(dens_prev_g, x, y, strength * green / 255.f, radius);
    setForceArea(dens_prev_b, x, y, strength * blue / 255.f, radius);
}

void Fluid::addVelocity(int x, int y, Vector dir, float r)   {
    
    //assert(x < N && y < N);
    
    setForceArea(u_prev, x, y, dir.x, r);
    
    setForceArea(v_prev, x, y, dir.y, r);
}

