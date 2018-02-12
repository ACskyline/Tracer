#ifndef DISK_H
#define DISK_H

#include "shape.h"

class Disk : public Shape
{
public:
    float radius;
    Disk();
    Disk(float _radius, Transform _transform);
    bool getIntersection(Ray r, Intersection *i);
};

#endif // DISK_H
