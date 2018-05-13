#include "naiveintegrator.h"


//DOES NOT SUPPORT DIRECTIONAL LIGHT
//BECAUSE THERE IS NO SHAPE ON DIRECTIONAL LIGHT
//SO IT WILL NEVER HIT
//ALSO RAYS COMING FROM DIRECTIONAL LIGHT ARE IN ONLY ONE DIRECTION
//MEANING THEY ARE IMPOSSIBLE TO RANDOMLY HIT BY OUR SAMPLED DIRECTION

Color3f NaiveIntegrator::Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
{
    //TODO
    if(depth<0)
        return Color3f(0);

    Intersection it;

    if(scene.Intersect(ray, &it))
    {
        Vector3f wo = -ray.direction;

        if(it.objectHit->ProduceBSDF(&it))
        {
            //qDebug() << it.objectHit->name << "->";
            Vector3f wi;
            float pdf;
            Color3f color = it.bsdf->Sample_f(wo, &wi, sampler->Get2D(), &pdf);
//#define DD
#ifdef DD
            qDebug() << "4.objectColor: " << color.x << "," << color.y << "," << color.z;
#endif
            Ray r = it.SpawnRay(wi);
            if(pdf == 0.f)//internal reflection
                return it.Le(wo);
            else
                return it.Le(wo) + color * Li(r, scene, sampler, --depth) / pdf * glm::abs(glm::dot(wi, it.bsdf->normal));
        }
        else
        {
            return it.Le(wo);
        }
    }

    return Color3f(0);
}
