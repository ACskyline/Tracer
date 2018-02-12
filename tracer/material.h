#ifndef MATERIAL_H
#define MATERIAL_H

#include "globalincludes.h"
#include "Scene.h"

class Scene;
class Intersection;

class Material
{
public:
    Color3f color;
    QString name;
    Material();
    Material(QString na, Color3f col);
    virtual ~Material();
    virtual Color3f getScatteredColor(const Intersection &it, const Ray &in, const Ray &out) = 0;
    virtual Color3f getScatteredColorEX(const Intersection &it, const Ray &in, Scene &s, int bounce = BOUNCEMAX) = 0;
    float getScatteredColorAO(const Intersection &it, const Ray &in, Scene &s);
    virtual Color3f getScatteredColorLight(const Intersection &it, const Ray &in, Scene &s, int bounce = BOUNCEMAX) = 0;
};

#endif // MATERIAL_H
