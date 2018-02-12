#ifndef LIGHT_H
#define LIGHT_H

#include "globalincludes.h"
#include "scene.h"

class Scene;
class Intersection;

class Light
{
public:
    Color3f color;
    Light();
    Light(Color3f col);
    virtual float isVisible(Point3f p, Intersection *it, Ray *r, Scene &scene) = 0;
};

#endif // LIGHT_H
