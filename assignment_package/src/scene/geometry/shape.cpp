#include "shape.h"
#include <QDateTime>

pcg32 Shape::colorRNG = pcg32(QDateTime::currentMSecsSinceEpoch());


void Shape::InitializeIntersection(Intersection *isect, float t, Point3f pLocal) const
{
    isect->point = Point3f(transform.T() * glm::vec4(pLocal, 1));
    ComputeTBN(pLocal, &(isect->normalGeometric), &(isect->tangent), &(isect->bitangent));
    isect->uv = GetUVCoordinates(pLocal);
    isect->t = t;
}

Intersection Shape::Sample(const Intersection &ref, const Point2f &xi, float *pdf) const
{
    //TODO
    Intersection it = Sample(xi, pdf);
    Vector3f direction = glm::normalize(ref.point - it.point);
    float length = glm::length(it.point - ref.point);

    float absDot = AbsDot(it.normalGeometric, direction);

    if(absDot != 0)
        *pdf = length * length * *pdf / absDot;
    else
        *pdf = 0;

    return it;
}
