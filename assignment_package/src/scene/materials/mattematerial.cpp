#include "mattematerial.h"
#include "lambertbrdf.h"

void MatteMaterial::ProduceBSDF(Intersection *isect) const
{
    // Important! Must initialize the intersection's BSDF!
    isect->bsdf = std::make_shared<BSDF>(*isect);

    Color3f color = Kd;
    if(this->textureMap)
    {
        glm::vec3 temp = Material::GetImageColor(isect->uv, this->textureMap.get());
        //qDebug() << isect->uv.x << "," << isect->uv.y << ":" << temp.x << "," << temp.y << "," << temp.z;
        color *= temp;//Material::GetImageColor(isect->uv, this->textureMap.get());
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

    if(sigma == 0.f)
    {
        isect->bsdf->Add(new LambertBRDF(color));
    }
    //Else do Oren-Nayar (not required implementation)
}
