#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "light.h"

class PointLight : public Light
{
public:
    Point3f position;
    PointLight();
    PointLight(Color3f col, Point3f pos);
    float isVisible(Point3f p, Intersection *it, Ray *out, Scene &scene);
};

#endif // POINTLIGHT_H
