#ifndef LAMBERTMATERIAL_H
#define LAMBERTMATERIAL_H

#include "material.h"

class LambertMaterial : public Material
{
public:
    LambertMaterial();
    LambertMaterial(QString na, Color3f col);
    Color3f getScatteredColor(const Intersection &it, const Ray &in, const Ray &out);
    //bounce not applicable
    Color3f getScatteredColorEX(const Intersection &it, const Ray &in, Scene &s, int bounce);
    //float getScatteredColorAO(const Intersection &it, const Ray &in, Scene &s);
    Color3f getScatteredColorLight(const Intersection &it, const Ray &in, Scene &s, int bounce);
};

#endif // LAMBERTMATERIAL_H
