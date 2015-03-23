//
//  Color.cpp
//  Fluid
//
//  Created by Ellis Sparky Hoag on 7/12/14.
//  Copyright (c) 2014 Ellis Sparky Hoag. All rights reserved.
//

#include "Color.h"

Color::Color()  {
    bits = 0;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b)   {
    setColor(r, g, b);
}

void Color::operator=(Color x)  {
    
    bits = x.bits;
}

void Color::operator+=(Color x)   {
    uint8_t R, r, G, g, B, b;
    
    getSpectrum(&R, &G, &B);
    x.getSpectrum(&r, &g, &b);
    
    setColor(R+r / 2, G+g / 2, B+b / 2);
}

Color Color::operator+(Color x) {
    uint8_t R, r, G, g, B, b;
    
    x.getSpectrum(&R, &G, &B);
    getSpectrum(&r, &g, &b);
    
    return  Color(R+r / 2, G+g / 2, B+b / 2);
}

void Color::operator*=(float x)    {
    
    if (x > 1) {
        x = 1;
    }
    if (x < 0) {
        x = 0;
    }
    
    uint8_t r, g, b;
    
    getSpectrum(&r, &g, &b);
    
    setColor(r*x, g*x, b*x);
}

Color Color::operator*(float x) {
    if (x > 1) {
        x = 1;
    }
    
    if (x < 0) {
        x = 0;
    }
    
    uint8_t r, g, b;
    
    getSpectrum(&r, &g, &b);
    
    return Color(r*x, g*x, b*x);
}

void Color::getSpectrum(uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = (bits >> 16) & 0x000000FF;
    *g = (bits >> 8) & 0x000000FF;
    *b = bits & 0x000000FF;
}

void Color::setColor(uint8_t r, uint8_t g, uint8_t b)   {
    
    bits = (r << 16) | (g << 8) | b;
}

void Color::print() {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    getSpectrum(&r, &g, &b);
    
    printf("{ %i, %i, %i }\n", r, g, b);
}