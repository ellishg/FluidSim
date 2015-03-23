//
//  Color.h
//  Fluid
//
//  Created by Ellis Sparky Hoag on 7/12/14.
//  Copyright (c) 2014 Ellis Sparky Hoag. All rights reserved.
//

#ifndef __Fluid__Color__
#define __Fluid__Color__

#include <iostream>

class Color {
    
private:
    
    uint32_t bits;

public:
    
    Color();
    
    Color(uint8_t r, uint8_t g, uint8_t b);
    
    Color(uint32_t _b) {bits = _b;}
    
    uint32_t getBits()  {return bits;}
    
    void setBits(uint32_t _b) {bits = _b;}
    
    void operator=(Color x);
    
    void operator+=(Color x);
    
    Color operator+(Color x);
    
    void operator*=(float x);
    
    Color operator*(float x);
    
    void getSpectrum(uint8_t *r, uint8_t *g, uint8_t *b);
    
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    
    void print();
    
};


#endif /* defined(__Fluid__Color__) */
