#include "pointlight.h"
#include "speculartransmissionmaterial.h"

PointLight::PointLight()
{

}

PointLight::PointLight(Color3f col, Point3f pos) : Light(col), position(pos)
{

}

float PointLight::isVisible(Point3f p, Intersection *it, Ray *out, Scene &scene)
{
    Ray r(p + float(BIAS) * glm::normalize(position - p), glm::normalize(position - p));
    bool hit = scene.getIntersection(r, it);

    if(hit)//hit
    {
        if(glm::length(it->point-r.origin) > glm::length(position-r.origin))//not blocked
        {
            *out = r;
            return 1;
        }
        else//blocked
        {
            if(dynamic_cast<SpecularTransmissionMaterial*>(it->objectHit->mat.get())!=nullptr)//refractive material
            {
                Ray R(it->point + float(BIAS)*r.direction, r.direction);
                Intersection It;
                if(isVisible(it->point, &It, &R, scene))
                {
                    *out = r;
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else//other material
            {
                return 0;
            }
        }
    }
    else//not hit
    {
        *out = r;
        return 1;
    }
}
