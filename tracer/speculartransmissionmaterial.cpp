#include "speculartransmissionmaterial.h"
#include <QDebug>

SpecularTransmissionMaterial::SpecularTransmissionMaterial()
{

}

SpecularTransmissionMaterial::SpecularTransmissionMaterial(
        QString na, Color3f col, float spec, float refr, float snlIn, float snlOut) :
    Material(na, col), specularity(spec), refraction(refr), snellIn(snlIn), snellOut(snlOut)
{

}

Color3f SpecularTransmissionMaterial::getScatteredColor(const Intersection &it, const Ray &in, const Ray &out)
{
    return glm::pow(
                glm::abs(
                    glm::dot(
                        glm::normalize((-glm::normalize(in.direction)
                                        +glm::normalize(out.direction))
                                       /2.f),
                        glm::normalize(it.normal)
                        )
                    ),
                specularity) * color;
}

Color3f SpecularTransmissionMaterial::getScatteredColorEX(const Intersection &it, const Ray &in, Scene &s, int bounce)
{
    return getScatteredColorAO(it, in, s) * getScatteredColorLight(it, in, s, bounce);
}

Color3f SpecularTransmissionMaterial::getScatteredColorLight(const Intersection &it, const Ray &in, Scene &s, int bounce)
{
    qDebug() << bounce << it.objectHit->name;
    Color3f c(0,0,0);

    if(bounce<0)
        return c;

    glm::vec3 vin = glm::normalize(-in.direction);
    glm::vec3 n = glm::normalize(it.normal);
    glm::vec3 vinVerti = glm::dot(vin, n)*n;
    glm::vec3 vinHori = vin - vinVerti;
    vinVerti = glm::normalize(vinVerti);
    vinHori = glm::normalize(vinHori);
    float sinOut = 1;
    if(glm::dot(vin, n) > 0)//enter mat
    {
        sinOut = glm::clamp(glm::sqrt(1-glm::dot(vin,n)*glm::dot(vin,n)) * snellIn, 0.f, 1.0f);
    }
    else//exit mat
    {
        sinOut = glm::clamp(glm::sqrt(1-glm::dot(vin,n)*glm::dot(vin,n)) / snellOut, 0.f, 1.0f);
    }
    float cosOut = glm::clamp(glm::sqrt(1-sinOut*sinOut), 0.f, 1.0f);
    glm::vec3 vout = glm::normalize((-vinVerti) * cosOut + (-vinHori) * sinOut);

    Ray ref(it.point + float(BIAS) * vout, vout);
    glm::vec3 cRef(1,1,1);
    Intersection itRef;
    if(s.getIntersection(ref, &itRef))
    {
        cRef = itRef.objectHit->mat->getScatteredColorEX(itRef, ref, s, --bounce);
    }

    //c = cRef;
    c = color;

    c = c * (1-refraction) + refraction * cRef;

    return c;
}

//float SpecularTransmissionMaterial::getScatteredColorAO(const Intersection &it, const Ray &in, Scene &s)
//{
//    float result;
//    return result;
//}
