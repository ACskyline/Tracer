#include "diffusearealight.h"

Color3f DiffuseAreaLight::L(const Intersection &isect, const Vector3f &w) const
{
    //TODO
    if(glm::dot(isect.normalGeometric,w)<0 && !twoSided)
        return Color3f(0.f);

    return emittedLight;
}

Color3f DiffuseAreaLight::Sample_Li(const Intersection &ref, const Point2f &xi, Vector3f *wi, Float *pdf) const
{
    Intersection it = shape->Sample(ref, xi, pdf);
    if(it.point == ref.point)
        return Color3f(0.f);

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    //we are converting to solid angle pdf in Shape::Sample, so we don't need to do this here
    //    Vector3f direction = glm::normalize(ref.point - it.point);
    //    float length = glm::length(it.point - ref.point);

    //    float absDot = AbsDot(it.normalGeometric, direction);

    //    if(absDot != 0)
    //        *pdf = length * length * *pdf / ( absDot * shape->Area());
    //    else
    //        *pdf = 0;
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    *wi = glm::normalize(it.point - ref.point);
    return L(it, -*wi);//be careful for this direction
}

float DiffuseAreaLight::Pdf_Li(const Intersection &ref, const Vector3f &wi) const
{
    //TO DO
    Ray r = ref.SpawnRay(wi);
    Intersection it;
    float pdf = 0;
    if(shape->Intersect(r, &it))
    {

        pdf = 1.f / shape->Area();

        Vector3f direction = glm::normalize(ref.point - it.point);
        float length = glm::length(it.point - ref.point);

        float absDot = AbsDot(it.normalGeometric, direction);

        if(absDot != 0)
            pdf = length * length * pdf / absDot;
        else
            pdf = 0;
    }

    return pdf;
}
