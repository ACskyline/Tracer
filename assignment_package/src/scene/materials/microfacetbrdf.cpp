#include "microfacetbrdf.h"

Color3f MicrofacetBRDF::f(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO!
    float cosThetaO = AbsCosTheta(wo), cosThetaI = AbsCosTheta(wi);
    Vector3f wh = wi + wo;

    if(cosThetaI==0 || cosThetaO == 0) return Color3f(0.f);
    if(wh.x == 0&&wh.y==0&&wh.z==0) return Color3f(0.f);

    wh = glm::normalize(wh);

    Color3f F = (fresnel != nullptr) ? fresnel->Evaluate(glm::dot(wi, wh)) : Color3f(1.f);

    return R * distribution->D(wh) * distribution->G(wo,wi)*F/(4*cosThetaI*cosThetaO);
}

Color3f MicrofacetBRDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &xi, Float *pdf, BxDFType *sampledType) const
{
    //TODO
    Vector3f wh = distribution->Sample_wh(wo, xi);
    *wi = 2 * glm::dot(wo,wh)*wh - wo;
    if(!SameHemisphere(wo,*wi)) return Color3f(0.f);

    *pdf = distribution->Pdf(wo,wh) / (4 * glm::dot(wo,wh));//Not Pdf(wo,wi);
    if(sampledType!=nullptr)
        *sampledType = type;

    return f(wo,*wi);
}

float MicrofacetBRDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO
    if(!SameHemisphere(wo,wi))
        return 0.f;
    Vector3f wh = glm::normalize(wo+wi);

    return distribution->Pdf(wo,wh) / (4 * glm::dot(wo,wh));
}
