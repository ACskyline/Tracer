#ifndef SPECULARTRANSMISSIONMATERIAL_H
#define SPECULARTRANSMISSIONMATERIAL_H

#include "material.h"

class SpecularTransmissionMaterial : public Material
{
public:
    float specularity;
    float refraction;
    float snellIn;
    float snellOut;
    SpecularTransmissionMaterial();
    SpecularTransmissionMaterial(QString na, Color3f col, float spec, float refr, float snlIn, float snlOut);
    Color3f getScatteredColor(const Intersection &it, const Ray &in, const Ray &out);
    Color3f getScatteredColorEX(const Intersection &it, const Ray &in, Scene &s, int bounce);
    Color3f getScatteredColorLight(const Intersection &it, const Ray &in, Scene &s, int bounce);
    //float getScatteredColorAO(const Intersection &it, const Ray &in, Scene &s);
};

#endif // SPECULARTRANSMISSIONMATERIAL_H
