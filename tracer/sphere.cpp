#include "sphere.h"

Sphere::Sphere()
{

}

Sphere::Sphere(float _radius, Transform _transform) : Shape(_transform), radius(_radius)
{

}

bool Sphere::getIntersection(Ray r, Intersection *i)
{
    Ray rLocal = r.transform(transform.worldSRTInv);
    glm::vec3 dir = glm::normalize(rLocal.direction);
    glm::vec3 origin = rLocal.origin;
    glm::vec3 target(0,0,0);
    float A = dir.x*dir.x + dir.y*dir.y + dir.z*dir.z;
    float B = 2*(glm::dot(dir,origin - target));
    float C = (origin.x - target.x)*(origin.x - target.x) +
            (origin.y - target.y)*(origin.y - target.y) +
            (origin.z - target.z)*(origin.z - target.z) -
            radius*radius;
    float d = B*B - 4*A*C;

    if(d < 0)
    {
        return false;
    }
    else
    {
        float t1 = (-B - sqrt(d))/(2*A);
        float t2 = (-B + sqrt(d))/(2*A);
        float t = 0;
        //float t = glm::min(t1,t2) < 0.f ? glm::max(t1,t2) : glm::min(t1,t2);
        if(t1<=0&&t2<=0)
            return false;
        else
            t = glm::min(t1,t2) <= 0.f ? glm::max(t1,t2) : glm::min(t1,t2);
        Point3f pLocal = origin + dir * t;
        Normal3f nLocal = glm::normalize(pLocal);
        glm::vec4 pWorld = transform.worldSRT * glm::vec4(pLocal.x, pLocal.y, pLocal.z, 1.f);
        glm::vec4 nWorld = transform.worldSRTInvTrans * glm::vec4(nLocal.x, nLocal.y, nLocal.z, 0.f);
        Point3f p(pWorld.x, pWorld.y, pWorld.z);
        Normal3f n(nWorld.x, nWorld.y, nWorld.z);
        i->point = p;
        i->normal = glm::normalize(n);
        //i->t = t; //wrong! t is not the same t since i normalized the direction of the ray after the matrix transformation
        i->t = t < 0 ? -1 * glm::length(p - r.origin)/glm::length(r.direction) : glm::length(p - r.origin)/glm::length(r.direction);//should not use vec3::length() not sure why
        return true;
    }
}
