#ifndef SQUAREPLANE_H
#define SQUAREPLANE_H

#include "shape.h"

class SquarePlane : public Shape
{
public:
    float side;
    SquarePlane();
    SquarePlane(float _side, Transform _transform);
    bool getIntersection(Ray r, Intersection *i);
};

#endif // SQUAREPLANE_H
