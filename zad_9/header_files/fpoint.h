#pragma once
#include "Graph.h"
#include <iostream>

class FPoint
{
public:
    float x,y;
    FPoint(float cx=0.0f, float cy=0.0f) : x(cx), y(cy) {}
    friend std::ostream& operator <<(std::ostream& os, const FPoint& p);
    friend std::istream& operator >>(std::istream& is, FPoint& p);
    operator Graph_lib::Point() const;
};

FPoint min(const FPoint& lf, const FPoint& rt);
FPoint max(const FPoint& lf, const FPoint& rt);
FPoint operator+(const FPoint& lpoint, float val);
FPoint operator-(const FPoint& lpoint, float val);
FPoint operator+(const FPoint& lpoint, const FPoint& rpoint);
FPoint operator*(const FPoint& lpoint, const FPoint& rpoint);
float distance(const FPoint& p1, const FPoint& p2);
