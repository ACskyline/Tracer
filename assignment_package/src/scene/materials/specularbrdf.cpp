#include "specularbrdf.h"

Color3f SpecularBRDF::f(const Vector3f &wo, const Vector3f &wi) const
{
    return Color3f(0.f);
}


float SpecularBRDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    return 0.f;
}

Color3f SpecularBRDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &sample, Float *pdf, BxDFType *sampledType) const
{
    //TODO!
    *wi = Vector3f(-wo.x, -wo.y, wo.z);

    if(sampledType!=nullptr)
        *sampledType = type;

    if(fresnel==nullptr)
        return Color3f(1.f);//totally conduct the other color

    float cosI = wo.z;
    Color3f Fr = fresnel->Evaluate(cosI);


    *pdf = 1;
    return R*Fr/glm::abs(wi->z);
}
