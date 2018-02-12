#include "disk.h"

Disk::Disk()
{

}

Disk::Disk(float _radius, Transform _transform) : Shape(_transform), radius(_radius)
{

}

bool Disk::getIntersection(Ray r, Intersection *i)
{
    //assuming plane is in x-y plane and facing z
    Ray rLocal = r.transform(transform.worldSRTInv);
    glm::vec3 dir = glm::normalize(rLocal.direction);
    glm::vec3 origin = rLocal.origin;
    glm::vec3 target(0,0,0);//center point
    glm::vec3 ABC(0,0,1);//not necessarily normailized
    dir -= target;
    origin -= target;
    float D = -glm::dot(ABC, target);//can be deducted using target and ABC
    if(glm::dot(dir,ABC) == 0)
    {
        return false;
    }
    else
    {
        float t = (-D-glm::dot(origin, ABC)) / glm::dot(dir,ABC);
        Point3f pLocal = origin + dir * t;

        if(pLocal.x*pLocal.x + pLocal.y*pLocal.y > radius*radius)//assuming disk in x-y plane and axis aligned
        {
            return false;
        }
        else
        {
            Normal3f nLocal = glm::normalize(ABC);//share only one normal
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
}
