#include "directionallight.h"

Color3f DirectionalLight::Sample_Li(const Intersection &ref, const Point2f &xi, Vector3f *wi, Float *pdf) const
{
    *wi = -glm::normalize(transform.T3()*direction);
    *pdf = 1;

    return L;
}

float DirectionalLight::Pdf_Li(const Intersection &ref, const Vector3f &wi) const
{
    return 0;//directional light does not participate in sampling bsdf case
}
