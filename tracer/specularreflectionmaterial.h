#ifndef SPECULARREFLECTIONMATERIAL_H
#define SPECULARREFLECTIONMATERIAL_H

#include "material.h"

class SpecularReflectionMaterial : public Material
{
public:
    float specularity;
    float reflection;
    SpecularReflectionMaterial();
    SpecularReflectionMaterial(QString na, Color3f col, float spec, float refl);
    Color3f getScatteredColor(const Intersection &it, const Ray &in, const Ray &out);
    Color3f getScatteredColorEX(const Intersection &it, const Ray &in, Scene &s, int bounce);
    Color3f getScatteredColorLight(const Intersection &it, const Ray &in, Scene &s, int bounce);
    //float getScatteredColorAO(const Intersection &it, const Ray &in, Scene &s);
};

#endif // SPECULARREFLECTIONMATERIAL_H
