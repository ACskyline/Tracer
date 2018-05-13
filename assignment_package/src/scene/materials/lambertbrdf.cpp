#include "lambertbrdf.h"
#include <warpfunctions.h>

Color3f LambertBRDF::f(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO
    return R * InvPi;
}

Color3f LambertBRDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u,
                              Float *pdf, BxDFType *sampledType) const
{
    //TODO
    *wi = WarpFunctions::squareToHemisphereCosine(u);
    *pdf = Pdf(wo, *wi);
    if(sampledType!=nullptr)
        *sampledType = type;
    return f(wo, *wi);
}

float LambertBRDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO
    if(SameHemisphere(wo, wi))
        return WarpFunctions::squareToHemisphereCosinePDF(wi);
    else
    {
#ifdef MY_GLOBAL_DEBUG
        qDebug() << "------shiit";
#endif
        return 0;
    }
}
