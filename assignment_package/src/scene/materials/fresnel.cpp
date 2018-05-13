#include "fresnel.h"

Color3f FresnelDielectric::Evaluate(float cosThetaI) const
{
    //TODO
    float nI, nT;

    if(cosThetaI > 0)//same side
    {
        nI = etaI;
        nT = etaT;
    }
    else
    {
        nI = etaT;
        nT = etaI;
        cosThetaI = -cosThetaI;
    }

    float sinThetaI = glm::sqrt(1 - cosThetaI*cosThetaI);

    float sinThetaT = nI / nT * sinThetaI;

    if(sinThetaT >= 1)
        return Color3f(1.f);

    float cosThetaT = glm::sqrt(1 - sinThetaT*sinThetaT);

    float rParl = ((nT * cosThetaI) - (nI * cosThetaT)) / ((nT * cosThetaI) + (nI * cosThetaT));
    float rPerp = ((nI * cosThetaI) - (nT * cosThetaT)) / ((nI * cosThetaI) + (nT * cosThetaT));
    return Color3f((rParl * rParl + rPerp * rPerp) / 2.f);
}
