#ifndef RAY_H
#define RAY_H

#include "globalincludes.h"

class Ray
{
public:
    Ray();
    Ray(Point3f ori, Vector3f dir);
    Point3f origin;
    Vector3f direction;
    Ray transform(glm::mat4 m);
};

#endif // RAY_H
