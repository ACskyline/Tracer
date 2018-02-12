#include "specularreflectionmaterial.h"
#include <qdebug.h>

SpecularReflectionMaterial::SpecularReflectionMaterial()
{

}


SpecularReflectionMaterial::SpecularReflectionMaterial(QString na, Color3f col, float spec, float refl) : Material(na, col), specularity(spec), reflection(refl)
{

}

Color3f SpecularReflectionMaterial::getScatteredColor(const Intersection &it, const Ray &in, const Ray &out)
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

Color3f SpecularReflectionMaterial::getScatteredColorEX(const Intersection &it, const Ray &in, Scene &s, int bounce)
{
    return getScatteredColorAO(it, in, s) * getScatteredColorLight(it, in, s, bounce);
}

Color3f SpecularReflectionMaterial::getScatteredColorLight(const Intersection &it, const Ray &in, Scene &s, int bounce)
{
    qDebug() << bounce << it.objectHit->name;
    Color3f c(0,0,0);

    if(bounce<0)
        return c;

    glm::vec3 vin = glm::normalize(-in.direction);
    glm::vec3 n = glm::normalize(it.normal);
    glm::vec3 vout = glm::normalize(2 * glm::dot(vin, n) * n - vin);

    Ray ref(it.point + float(BIAS) * vout, vout);
    glm::vec3 cRef(1,1,1);
    Intersection itRef;
    if(s.getIntersection(ref, &itRef))
    {
        cRef = itRef.objectHit->mat->getScatteredColorEX(itRef, ref, s, --bounce);
    }

    //c = cRef;

    float count = 0;
    for(int k = 0;k<s.lightList.size();++k)
    {
        glm::vec3 cLight = s.lightList[k]->color;
        glm::vec3 cShade(0,0,0);
        //Ray out(it.point + float(BIAS) * glm::normalize(s.lightList[k]->position - it.point), glm::normalize(s.lightList[k]->position - it.point));

        Intersection itOut;
        Ray out;
        float visibility = s.lightList[k]->isVisible(it.point, &itOut, &out, s);
        if(visibility > 0)//no blocker
        {
            cShade = visibility * it.objectHit->mat->getScatteredColor(it, in, out);
            count++;
        }
        else//blocker
        {
            cShade = visibility * glm::vec3(0.0f,0.0f,0.0f);
        }

        c = c + cLight * cShade;
    }

    if(count!=0) c/=count;

    c = c * (1-reflection) + reflection * cRef;

    return c;
}

//float SpecularReflectionMaterial::getScatteredColorAO(const Intersection &it, const Ray &in, Scene &s)
//{
//    float result;
//    return result;
//}
