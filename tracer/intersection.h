#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "globalincludes.h"
#include "primitive.h"

class Primitive;

class Intersection
{
public:
    Intersection();
    Point3f point;
    Normal3f normal;
    float t;
    std::shared_ptr<Primitive> objectHit;
};

#endif // INTERSECTION_H
