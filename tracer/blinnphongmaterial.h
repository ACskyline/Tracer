#ifndef BLINNPHONGMATERIAL_H
#define BLINNPHONGMATERIAL_H

#include "material.h"

class BlinnPhongMaterial : public Material
{
public:
    float specularity;
    BlinnPhongMaterial();
    BlinnPhongMaterial(QString na, Color3f col, float spec);
    Color3f getScatteredColor(const Intersection &it, const Ray &in, const Ray &out);
    //bounce not applicable
    Color3f getScatteredColorEX(const Intersection &it, const Ray &in, Scene &s, int bounce);
    Color3f getScatteredColorLight(const Intersection &it, const Ray &in, Scene &s, int bounce);
    //float getScatteredColorAO(const Intersection &it, const Ray &in, Scene &s);
};

#endif // BLINNPHONGMATERIAL_H
