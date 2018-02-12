#include "primitive.h"

Primitive::Primitive()
{

}

Primitive::Primitive(QString _name)
{
    name = _name;
}


bool Primitive::getIntersection(Ray r, Intersection *i)
{
    if(shape->getIntersection(r, i))
    {
        //this is wrong
        //i->objectHit = std::make_shared<Primitive>(*this);
        return true;
    }
    else
    {
        return false;
    }
}
