#include "bsdf.h"
#include <warpfunctions.h>
#include <pcg32.h>

#include "specularbtdf.h"
#include "specularbrdf.h"

#ifdef MY_GLOBAL_DEBUG
#define MY_DEBUG
#endif

BSDF::BSDF(const Intersection& isect, float eta /*= 1*/)
//TODO: Properly set worldToTangent and tangentToWorld
    : worldToTangent(glm::inverse(glm::mat3(isect.tangent, isect.bitangent, isect.normalGeometric))),
      tangentToWorld(glm::mat3(isect.tangent, isect.bitangent, isect.normalGeometric)),
      normal(isect.normalGeometric),
      eta(eta),
      numBxDFs(0),
      bxdfs{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
{}


void BSDF::UpdateTangentSpaceMatrices(const Normal3f& n, const Vector3f& t, const Vector3f b)
{
    //TODO: Update worldToTangent and tangentToWorld based on the normal, tangent, and bitangent
    tangentToWorld = glm::mat3(t, b, n);//glm::mat3(glm::normalize(t), glm::normalize(b), glm::normalize(n));
    //can't use tranpose
    //because for a mesh, n, t and bit are not necessarily orthonormal!!
    //since you are calculating t, bit by uv and normal by interpolation for triangle
    worldToTangent = glm::inverse(tangentToWorld);
}


//
Color3f BSDF::f(const Vector3f &woW, const Vector3f &wiW, BxDFType flags /*= BSDF_ALL*/) const
{
    //TODO
    Color3f total(0.f);
    Vector3f woL = glm::normalize(worldToTangent * woW);
    Vector3f wiL = glm::normalize(worldToTangent * wiW);
    int count = 0;
    bool reflect = SameHemisphere(woL, wiL);//glm::dot(wiW, normal) * glm::dot(woW, normal) > 0;//somehow wrong


#ifdef MY_DEBUG
    qDebug() << "------woL " << woL.x << ","  << woL.y << ","  << woL.z;
    qDebug() << "------wiL " << wiL.x << ","  << wiL.y << ","  << wiL.z;
    qDebug() << "------reflect " << reflect;
#endif

    for(int i = 0;i<MaxBxDFs;i++)
    {
        if(bxdfs[i]!=nullptr&&bxdfs[i]->MatchesFlags(flags)&&
                ((reflect && (bxdfs[i]->type & BxDFType::BSDF_REFLECTION)) ||
                 (!reflect && (bxdfs[i]->type & BxDFType::BSDF_TRANSMISSION))))
        {
            total += bxdfs[i]->f(woL, wiL);
            count++;
        }
    }

    return total;
}

// Use the input random number _xi_ to select
// one of our BxDFs that matches the _type_ flags.

// After selecting our random BxDF, rewrite the first uniform
// random number contained within _xi_ to another number within
// [0, 1) so that we don't bias the _wi_ sample generated from
// BxDF::Sample_f.

// Convert woW and wiW into tangent space and pass them to
// the chosen BxDF's Sample_f (along with pdf).
// Store the color returned by BxDF::Sample_f and convert
// the _wi_ obtained from this function back into world space.

// Iterate over all BxDFs that we DID NOT select above (so, all
// but the one sampled BxDF) and add their PDFs to the PDF we obtained
// from BxDF::Sample_f, then average them all together.

// Finally, iterate over all BxDFs and sum together the results of their
// f() for the chosen wo and wi, then return that sum.

Color3f BSDF::Sample_f(const Vector3f &woW, Vector3f *wiW, const Point2f &xi,
                       float *pdf, BxDFType type, BxDFType *sampledType) const
{
    //TODO

    Vector3f woL = glm::normalize(worldToTangent * woW);
    Vector3f wiL;
    pcg32 temp(glm::abs(1.f/xi.x*1.f/xi.y));
    float random = temp.nextFloat();
    float chosenPdf = 0;
    Color3f chosenColor;

    int total = BxDFsMatchingFlags(type);
    if(total==0)
    {
        *pdf = 0;
        return Color3f(0);
    }
    int randomBxDFcount = (int)(random * total);
    //qDebug() << BxDFsMatchingFlags(type);
    int count = 0;
    BxDF* randomBxDF;

    for(int i = 0;i<MaxBxDFs;i++)
    {
        if(bxdfs[i]!=nullptr&&bxdfs[i]->MatchesFlags(type))//test whether type is a super set of bxdfs[i]->type
        {
            if(count == randomBxDFcount)
            {
                randomBxDF = bxdfs[i];
                break;
            }
            count++;
        }
    }

#ifdef MY_DEBUG
    if(dynamic_cast<SpecularBTDF*>(randomBxDF))
        qDebug() << "trans";
    if(dynamic_cast<SpecularBRDF*>(randomBxDF))
        qDebug() << "ref";
#endif

    if(randomBxDF->type & BSDF_SPECULAR)
    {
        //qDebug() << randomBxDFcount;
        chosenColor = randomBxDF->Sample_f(woL, &wiL, xi, &chosenPdf, sampledType);
        *wiW = glm::normalize(tangentToWorld * wiL);//
        *pdf = chosenPdf / total;
        return chosenColor;
    }

    chosenColor = randomBxDF->Sample_f(woL, &wiL, xi, &chosenPdf, sampledType);
    *wiW = glm::normalize(tangentToWorld * wiL);//
    *pdf = Pdf(woW, *wiW, type);
    return f(woW, *wiW, type);

}


float BSDF::Pdf(const Vector3f &woW, const Vector3f &wiW, BxDFType flags) const
{
    //TODO
    Vector3f woL = glm::normalize(worldToTangent * woW);
    Vector3f wiL = glm::normalize(worldToTangent * wiW);
#ifdef MY_DEBUG
    qDebug() << "woL " << woL.x << ","  << woL.y << ","  << woL.z;
    qDebug() << "wiL " << wiL.x << ","  << wiL.y << ","  << wiL.z;
#endif

    float result = 0;
    int count = 0;
    for(int i = 0;i<MaxBxDFs;i++)
    {
        if(bxdfs[i]!=nullptr&&bxdfs[i]->MatchesFlags(flags))
        {
            //result += bxdfs[i]->Pdf(woL,wiL);
            float dot = glm::dot(wiW, normal);
            result += dot / glm::pi<float>();
            //qDebug() << "wi:" << wiW.x << ","  << wiW.y << ","  << wiW.z;
            //qDebug() << "normal:" << normal.x << ","  << normal.y << ","  << normal.z;
            //qDebug() << "dot:" << dot;
            //qDebug() << "result:"  << result;
            count++;
        }
    }

#ifdef MY_DEBUG
    qDebug() << "result " << result;
    qDebug() << "count " << count;
#endif
    if(count == 0) return 0;
    else return result/count;
}

Color3f BxDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &xi,
                       Float *pdf, BxDFType *sampledType) const
{
    *wi = WarpFunctions::squareToHemisphereUniform(xi);
    *pdf = Pdf(wo, *wi);
    *sampledType = type;
    //TODO
    return f(wo, *wi);
}

// The PDF for uniform hemisphere sampling
float BxDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    return SameHemisphere(wo, wi) ? Inv2Pi : 0;
}

BSDF::~BSDF()
{
    for(int i = 0; i < numBxDFs; i++)
    {
        delete bxdfs[i];
    }
}
