#include "squareplane.h"
#include "warpfunctions.h"

float SquarePlane::Area() const
{
    Vector3f scale = transform.getScale();
    return glm::abs(scale.x*scale.y);
}

bool SquarePlane::Intersect(const Ray &ray, Intersection *isect)
{
    //Transform the ray
    Ray r_loc = ray.GetTransformedCopy(transform.invT());

    //Ray-plane intersection
    float t = glm::dot(glm::vec3(0,0,1), (glm::vec3(0.5f, 0.5f, 0) - r_loc.origin)) / glm::dot(glm::vec3(0,0,1), r_loc.direction);
    Point3f P = Point3f(t * r_loc.direction + r_loc.origin);
    //Check that P is within the bounds of the square
    if(t > 0 && P.x >= -0.5f && P.x <= 0.5f && P.y >= -0.5f && P.y <= 0.5f)
    {
        InitializeIntersection(isect, t, P);
        return true;
    }
    return false;
}

void SquarePlane::ComputeTBN(const Point3f &P, Normal3f *nor, Vector3f *tan, Vector3f *bit) const
{
    *nor = glm::normalize(transform.invTransT() * Normal3f(0,0,1));
    //TODO: Compute tangent and bitangent
    *tan = glm::normalize(glm::vec3(transform.T() * glm::vec4(1, 0, 0, 0.f)));
    *bit = glm::normalize(glm::vec3(transform.T() * glm::vec4(0, 1, 0, 0.f)));
}


Point2f SquarePlane::GetUVCoordinates(const Point3f &point) const
{
    return Point2f(point.x + 0.5f, point.y + 0.5f);
}

Intersection SquarePlane::Sample(const Point2f &xi, Float *pdf) const
{
    glm::vec4 plocal((xi.x-0.5), (xi.y-0.5), 0, 1);
    Normal3f nlocal(0, 0, 1);

    Intersection it;
    it.point = Point3f(transform.T() * plocal);
    it.normalGeometric = glm::normalize(transform.invTransT() * nlocal);

    *pdf = 1.0/Area();

    return it;
}
