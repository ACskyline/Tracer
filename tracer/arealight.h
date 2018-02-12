#ifndef AREALIGHT_H
#define AREALIGHT_H

#include "shape.h"

class AreaLight : public Light
{
public:
    std::shared_ptr<Shape> shape;
    AreaLight();
    AreaLight(Color3f col, std::shared_ptr<Shape> sha);
    float isVisible(Point3f p, Intersection *it, Ray *out, Scene &scene);
};

#endif // AREALIGHT_H
