#ifndef CAMERA_H
#define CAMERA_H

#include "globalincludes.h"
#include "ray.h"

class Camera
{
public:
    Camera();
    Camera(Point3f _eye, Point3f _ref, Vector3f _worldUp,
           float _fovY, float _near, float  _far, int _width, int _height);
    Point3f eye;
    Point3f ref;
    Vector3f worldUp;
    float fovY;
    float near;
    float far;
    int width;
    int height;
    glm::mat4 viewProj;
    glm::mat4 viewProjInv;
    Ray rayCast(Point2f p);
};

#endif // CAMERA_H
