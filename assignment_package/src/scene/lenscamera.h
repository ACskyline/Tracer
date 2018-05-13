#ifndef LENCAMERA_H
#define LENCAMERA_H

#include "camera.h"

class LensCamera : public Camera
{
public:
    float lensRadius;
    float focalDistance;

    LensCamera();
    LensCamera(float lR, float fD);
    LensCamera(float lR, float fD, unsigned int w, unsigned int h);
    LensCamera(float lR, float fD, unsigned int w, unsigned int h, const Vector3f &e, const Vector3f &r, const Vector3f &worldUp);

    Ray Raycast(const Point2f &pt) const;
};

#endif // LENCAMERA_H
