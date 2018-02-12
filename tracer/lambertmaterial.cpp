#include "lambertmaterial.h"
#include "sampler.h"
#include <QDebug>

LambertMaterial::LambertMaterial()
{

}

LambertMaterial::LambertMaterial(QString na, Color3f col) : Material(na, col)
{

}


Color3f LambertMaterial::getScatteredColor(const Intersection &it, const Ray &in, const Ray &out)
{
    return glm::clamp(glm::dot(glm::normalize(it.normal), glm::normalize(out.direction)), 0.f, 1.0f) * color;
}

Color3f LambertMaterial::getScatteredColorEX(const Intersection &it, const Ray &in, Scene &s, int bounce)
{
    //return getScatteredColorLight(it, in, s, bounce);
    //return color * getScatteredColorAO(it, in, s);
    return getScatteredColorAO(it, in, s) * getScatteredColorLight(it, in, s, bounce);
}

Color3f LambertMaterial::getScatteredColorLight(const Intersection &it, const Ray &in, Scene &s, int bounce)
{
    glm::vec3 c(0,0,0);
    float count = 0;
    for(int k = 0;k<s.lightList.size();++k)
    {
        glm::vec3 cLight = s.lightList[k]->color;
        glm::vec3 cShade(0,0,0);
        //Ray out(it.point + float(BIAS) * glm::normalize(s.lightList[k]->position - it.point), glm::normalize(s.lightList[k]->position - it.point));
        Ray out;
        Intersection itOut;
        float visibility = s.lightList[k]->isVisible(it.point, &itOut, &out, s);
        //if(it.objectHit->name=="ceiling") qDebug() << visibility;
        if(visibility>0)//no blocker
        {
            //if(it.objectHit->name=="ceiling") qDebug() << "normal:" << it.normal.x << "," << it.normal.y << "," << it.normal.z << " >> out:" << out.direction.x << "," << out.direction.y << "," << out.direction.z << "===" << glm::dot(it.normal, out.direction);
            cShade = visibility * it.objectHit->mat->getScatteredColor(it, in, out);
            count++;
        }
        else//blocker
        {
            cShade = visibility * glm::vec3(0.0f,0.0f,0.0f);
        }

        c = c + cLight * cShade;
    }

    if(count!=0) c /= count;
    return c;
}

//float LambertMaterial::getScatteredColorAO(const Intersection &it, const Ray &in, Scene &s)
//{
//    //when translating points on the hemisphere, we should translate
//    //the micro coordinate system to world and make sure it is still
//    //orthonormal(by using different matrix to transform normal and
//    //tangent/bitangent), and then map the hemisphere point to world
//    //coordinate, instead of finding the point on local coordinate and
//    //then map to world coordinate, because it means we are scaling
//    //the micro coordinate system(the hemisphere)
//    float result = 0;

//    Sampler sam;
//    int numSamples = SAMPLE;
//    WarpMethod wp = HEMISPHERE_COSINE;

//    sam.generateSamples(numSamples, GRID, wp);
//    const std::vector<float>& samples = sam.peekSamples();

//    glm::vec4 normalMicroInLocal = glm::vec4(glm::normalize(glm::vec3(glm::transpose(it.objectHit->shape->transform.worldSRT) * glm::vec4(it.normal, 0))), 0.f);//local normal is micro normal
//    glm::vec4 tangentMicroInLocal = glm::vec4(glm::normalize(glm::cross(glm::vec3(normalMicroInLocal), glm::vec3(1,0,0))), 0);
//    glm::vec4 bitangentMicroInLocal = glm::vec4(glm::normalize(glm::cross(glm::vec3(normalMicroInLocal), glm::vec3(tangentMicroInLocal))), 0);

//    //qDebug() << normalMicroInLocal.x << "," << normalMicroInLocal.y << "," << normalMicroInLocal.z;
//    //qDebug() << tangentMicroInLocal.x << "," << tangentMicroInLocal.y << "," << tangentMicroInLocal.z;
//    //qDebug() << bitangentMicroInLocal.x << "," << bitangentMicroInLocal.y << "," << bitangentMicroInLocal.z;

//    //glm::mat4 micro2local(tangentMicroInLocal, bitangentMicroInLocal, normalMicroInLocal, it.objectHit->shape->transform.worldSRTInv*glm::vec4(it.point,1));

//    glm::mat4 local2world = it.objectHit->shape->transform.worldSRT;
//    glm::mat4 local2worldNormal = it.objectHit->shape->transform.worldSRTInvTrans;

//    glm::vec4 normalMicroInWorld = glm::vec4(glm::normalize(glm::vec3(local2worldNormal * normalMicroInLocal)), 0.f);
//    glm::vec4 tangentMicroInWorld = glm::vec4(glm::normalize(glm::vec3(local2world * tangentMicroInLocal)), 0.f);
//    glm::vec4 bitangentMicroInWorld = glm::vec4(glm::normalize(glm::vec3(local2world * bitangentMicroInLocal)), 0.f);

//    glm::mat4 micro2World(tangentMicroInWorld, bitangentMicroInWorld, normalMicroInWorld, glm::vec4(it.point, 1.0));

//    for(int i = 0;i<samples.size();i+=3)
//    {
//        glm::vec3 positionMicro(samples[i],samples[i+1],samples[i+2]);

//        //glm::vec4 positionLocal = micro2local * glm::vec4(positionMicro, 1.f);

//        glm::vec4 positionWorld = micro2World * glm::vec4(positionMicro, 1.f);//local2world * positionLocal;

//        Ray R(it.point + float(BIAS) * glm::normalize(glm::vec3(positionWorld) - it.point), glm::normalize(glm::vec3(positionWorld) - it.point));
//        Intersection I;
//        bool hit = s.getIntersection(R, &I);
//        if(!hit || (hit&&glm::length(I.point-it.point) > AOMAXLENGTH) )
//        {
//            result++;
//        }
//        else//hit
//        {
//            //if(it.objectHit->name == "sphere1"&&I.objectHit->name == "right")
//                //qDebug() << it.objectHit->name << ":" << it.point.x << "," << it.point.y << "," << it.point.z << ">>" << I.objectHit->name << ":" << I.point.x << "," << I.point.y << "," << I.point.z << "=" << I.point.x - it.point.x << "," << I.point.y - it.point.y << "," << I.point.z - it.point.z << " >> t = " << I.t;
//        }
//    }
//    result /= numSamples;
//    return result;
//}
