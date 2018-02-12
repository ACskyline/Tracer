#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "globalincludes.h"
#include "shape.h"
#include "ray.h"
#include "material.h"
#include <QString>

class Intersection;
class Shape;
class Material;

class Primitive
{
public:
    Primitive();
    Primitive(QString _name);
    QString name;
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> mat;
    bool getIntersection(Ray r, Intersection* i);
};

#endif // PRIMITIVE_H
