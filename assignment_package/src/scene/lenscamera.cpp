#include "lenscamera.h"
#include "camera.h"
#include "warpfunctions.h"


LensCamera::LensCamera()
{

}

LensCamera::LensCamera(float lR, float fD) : lensRadius(lR), focalDistance(fD)
{

}

LensCamera::LensCamera(float lR, float fD, unsigned int w, unsigned int h) : lensRadius(lR), focalDistance(fD), Camera(w, h)
{

}


LensCamera::LensCamera(float lR, float fD, unsigned int w, unsigned int h, const Vector3f &e, const Vector3f &r, const Vector3f &worldUp)
 : lensRadius(lR), focalDistance(fD), Camera(w, h, e, r, worldUp)
{

}

Ray LensCamera::Raycast(const Point2f &pt) const
{
    float ndc_x = (2.f*pt.x/width - 1);
    float ndc_y = (1 - 2.f*pt.y/height);
    Point3f P = ref + ndc_x*H + ndc_y*V;
    Point3f pFocus = eye + (P-eye) * focalDistance / glm::dot(P - eye, look);

    Point2f pLens = Point2f(lensRadius * WarpFunctions::squareToDiskConcentric(sampler->Get2D()));

    Point3f origin = eye + pLens.x * right + pLens.y * up;
    Ray r(origin, glm::normalize(pFocus - origin));

    return r;
}
