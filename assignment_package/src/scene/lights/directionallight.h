#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight(const Transform &t,
                     const Color3f& _L,
                     const Vector3f& _direction) :
        Light(t), L(_L), direction(_direction)
    {}

    Color3f Sample_Li(const Intersection &ref, const Point2f &xi, Vector3f *wi, Float *pdf) const;
    float Pdf_Li(const Intersection &ref, const Vector3f &wi) const;

    Color3f L;
    Vector3f direction;
};

#endif // DIRECTIONALLIGHT_H
