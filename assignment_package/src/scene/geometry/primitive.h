#pragma once
#include "shape.h"
#include <scene/lights/light.h>

class AreaLight;
class Shape;

// A class that holds all the information for a given object in a scene,
// such as its Shape, its Material, and (if applicable) ita AreaLight
// Parallels the GeometricPrimitive class from PBRT
class Primitive
{
public:
    Primitive() :
        name("Some Primitive"), shape(nullptr), material(nullptr), areaLight(nullptr)
    {}
    Primitive(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material = nullptr, std::shared_ptr<AreaLight> areaLight = nullptr)
        : shape(shape), material(material), areaLight(areaLight)
    {}
    virtual ~Primitive()
    {} //me virtual
    // Returns whether or not the given Ray intersects this Primitive.
    // Passes additional intersection data through the Intersection pointer
    virtual bool Intersect(const Ray& r, Intersection* isect) const;//me virtual

    const AreaLight* GetAreaLight() const;
    const Material* GetMaterial() const;
    // Ask our _material_ to generate a BSDF containing
    // BxDFs and store it in isect.
    bool ProduceBSDF(Intersection *isect) const;


    QString name;//Mainly used for debugging purposes
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    std::shared_ptr<AreaLight> areaLight;
};
