#include "specularbtdf.h"

Color3f SpecularBTDF::f(const Vector3f &wo, const Vector3f &wi) const
{
    return Color3f(0.f);
}


float SpecularBTDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    return 0.f;
}

Color3f SpecularBTDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &sample, Float *pdf, BxDFType *sampledType) const
{
    //TODO!
    if(sampledType!=nullptr)
        *sampledType = type;

    if(fresnel==nullptr)
        return T;

    float eta = wo.z > 0 ? etaA/etaB : etaB/etaA;

    float cosI = wo.z;

    Color3f Fr = fresnel->Evaluate(cosI);

    if(Fr==glm::vec3(1,1,1))
    {
        *pdf = 0.f;
        return Color3f(0.f);
    }
    else
    {
        float sinI = glm::sqrt(wo.x*wo.x + wo.y*wo.y);
        float sinT = sinI*eta;
        float cosT = glm::sqrt(1-sinT*sinT);

        glm::vec3 woProj = glm::normalize(wo - glm::vec3(0,0,wo.z));
        glm::vec3 woN = glm::normalize(glm::vec3(0,0,wo.z));

        *wi = -woProj * sinT - woN * cosT;//glm::refract(-wo, glm::vec3(0,0,1), eta);
        *pdf = 1.f;

        return T * (glm::vec3(1,1,1)-Fr) / glm::abs(wi->z);
    }
}
