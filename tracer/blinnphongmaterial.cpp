#include "blinnphongmaterial.h"

BlinnPhongMaterial::BlinnPhongMaterial()
{

}

BlinnPhongMaterial::BlinnPhongMaterial(QString na, Color3f col, float spec) : Material(na, col), specularity(spec)
{

}

Color3f BlinnPhongMaterial::getScatteredColor(const Intersection &it,
                                              const Ray &in,
                                              const Ray &out)
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

Color3f BlinnPhongMaterial::getScatteredColorEX(const Intersection &it, const Ray &in, Scene &s, int bounce)
{
    return getScatteredColorAO(it, in, s) * getScatteredColorLight(it, in, s, bounce);
}

Color3f BlinnPhongMaterial::getScatteredColorLight(const Intersection &it, const Ray &in, Scene &s, int bounce)
{
    Color3f c(0,0,0);
    float count = 0;
    for(int k = 0;k<s.lightList.size();++k)
    {
        glm::vec3 cLight = s.lightList[k]->color;
        glm::vec3 cShade(0,0,0);
        //Ray out(it.point + float(BIAS) * glm::normalize(s.lightList[k]->position - it.point), glm::normalize(s.lightList[k]->position - it.point));
        Ray out;
        Intersection itOut;
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
    return c;
}

//float BlinnPhongMaterial::getScatteredColorAO(const Intersection &it, const Ray &in, Scene &s)
//{
//    float result;
//    return result;
//}
