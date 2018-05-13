#include "transmissivematerial.h"
#include "specularbtdf.h"

void TransmissiveMaterial::ProduceBSDF(Intersection *isect) const
{
    // Important! Must initialize the intersection's BSDF!
    isect->bsdf = std::make_shared<BSDF>(*isect, indexOfRefraction);

    Color3f color = Kt;
    if(this->textureMap)
    {
        color *= Material::GetImageColor(isect->uv, this->textureMap.get());
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

    isect->bsdf->Add(new SpecularBTDF(color, 1.f, indexOfRefraction, new FresnelNoReflect()));
}
