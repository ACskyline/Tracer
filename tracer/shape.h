#ifndef SHAPE_H
#define SHAPE_H

#include "transform.h"
#include "ray.h"
#include "intersection.h"

class Intersection;

class Shape
{
public:
    Shape();
    Shape(Transform _transform);
    virtual ~Shape();
    Transform transform;
    virtual bool getIntersection(Ray r, Intersection* i) = 0;
};

#endif // SHAPE_H
