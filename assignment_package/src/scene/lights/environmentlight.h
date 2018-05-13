#ifndef ENVIRONMENTLIGHT_H
#define ENVIRONMENTLIGHT_H

#include "scene/lights/light.h"

class EnvironmentLight : public Light
{
public:
    EnvironmentLight(const Transform &t,
                     const std::shared_ptr<QImage> &_textureMap,
                     Color3f _L) :
        Light(t), textureMap(_textureMap), L(_L)
    {}

    std::shared_ptr<QImage> textureMap;
    Color3f L;

    Color3f Sample_Li(const Intersection &ref, const Point2f &xi, Vector3f *wi, Float *pdf) const;
    float Pdf_Li(const Intersection &ref, const Vector3f &wi) const;
    Color3f Le(const Ray &r) const;
};

#endif // ENVIRONMENTLIGHT_H
