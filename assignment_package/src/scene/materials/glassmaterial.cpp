#include "glassmaterial.h"
#include "specularbrdf.h"
#include "specularbtdf.h"

void GlassMaterial::ProduceBSDF(Intersection *isect) const
{
    // Important! Must initialize the intersection's BSDF!
    isect->bsdf = std::make_shared<BSDF>(*isect, indexOfRefraction);

    Color3f reflectColor = Kr;
    if(this->textureMapRefl)
    {
        reflectColor *= Material::GetImageColor(isect->uv, this->textureMapRefl.get());
    }
    Color3f transmitColor = Kt;
    if(this->textureMapTransmit)
    {
        transmitColor *= Material::GetImageColor(isect->uv, this->textureMapTransmit.get());
    }
    if(this->normalMap)
    {
        //who ever wrote this base code deserve to die!
        //isect->bsdf->normal = isect->bsdf->tangentToWorld *  Material::GetImageColor(isect->uv, this->normalMap.get());
        //Update bsdf's TBN matrices to support the new normal
        //seriously, what?!
        //Vector3f tangent, bitangent;
        //CoordinateSystem(isect->bsdf->normal, &tangent, &bitangent);
        //isect->bsdf->UpdateTangentSpaceMatrices(isect->bsdf->normal, tangent, bitangent);

        Normal3f n = Material::GetImageColor(isect->uv, this->normalMap.get());
        n = n*2.f-Normal3f(1,1,1);
        isect->bsdf->normal = isect->bsdf->tangentToWorld * n;
        isect->bsdf->UpdateTangentSpaceMatrices(isect->bsdf->normal, isect->tangent, isect->bitangent);
    }

    // Add SpecularBTDF and SpecularBRDF each with FresnelDielectric
    isect->bsdf->Add(new SpecularBRDF(reflectColor, new FresnelDielectric(1.f, indexOfRefraction)));
    isect->bsdf->Add(new SpecularBTDF(transmitColor, 1.f, indexOfRefraction, new FresnelDielectric(1.f, indexOfRefraction)));
    
}
