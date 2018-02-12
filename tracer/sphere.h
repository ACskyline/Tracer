#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"

class Sphere : public Shape
{
public:
    float radius;
    Sphere();
    Sphere(float _radius, Transform _transform);
    bool getIntersection(Ray r, Intersection *i);
};

#endif // SPHERE_H
