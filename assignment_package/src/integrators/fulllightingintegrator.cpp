#include "fulllightingintegrator.h"

#define MINDEPTH 3

#ifdef MY_GLOBAL_DEBUG
#define MY_DEBUG
#endif

Color3f FullLightingIntegrator::Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
{
    if(depth<0)
        return Color3f(0);

    Color3f finalColor(0.f);
    Color3f throughput(1.f);

    Ray rayTemp = ray;

    int depthMax = depth;
    bool specularBounce = false;
    BxDFType flags = BxDFType(BxDFType::BSDF_ALL & ~BxDFType::BSDF_SPECULAR);

    while(depth>0)
    {
        Intersection it;

        if(scene.Intersect(rayTemp, &it))
        {
#ifdef MY_DEBUG
            if(it.objectHit)
                qDebug() << depth << ":" << it.objectHit->name;
#endif

#ifdef MY_DEBUG
                        qDebug() << "start throughput:" << throughput.x << "," << throughput.y << "," << throughput.z;
#endif
            Vector3f wo = -rayTemp.direction;

            if(depth==depthMax||specularBounce)
                finalColor += it.Le(wo) * throughput;

            if(it.objectHit->ProduceBSDF(&it))
            {
                int lightCount = scene.lights.size();
                int lightRand = sampler->Get1D() * lightCount;

                Vector3f wi;
                float pdfLight;
                Color3f colorLight = scene.lights[lightRand]->Sample_Li(it, sampler->Get2D(), &wi, &pdfLight);
                pdfLight /= lightCount;

                Color3f Ld(0.f);//Direct light MIS color
                //1.Direct light MIS
                {
                    //sample light source with multiple importance sampling
                    if(!IsBlack(colorLight) && pdfLight >= 0)//pdfLight==0 is delta light or pdf is infinity(manually set to 0) because AbsDot is 0
                    {
                        //BSDF
                        float pdf = it.bsdf->Pdf(wo, wi, flags);
                        Color3f color = it.bsdf->f(wo, wi, flags) * AbsDot(wi, it.bsdf->normal);
#ifdef MY_DEBUG
                        qDebug() << "sample light: bsdf->f " << color.x << "," << color.y << "," << color.z;
#endif

                        if(!IsBlack(color) && pdf > 0.f)
                        {
                            //visibility
                            Ray r = it.SpawnRay(wi);
                            Intersection shadow;
                            if(scene.Intersect(r, &shadow))
                            {
                                if(shadow.objectHit->GetAreaLight() != scene.lights[lightRand].get())
                                {
                                    colorLight = Color3f(0.f);
                                }
                            }
                            else
                            {
                                //if this is arealight hitting nothing is error
                                //if this is not arealight, hitting nothing means no shadow
                            }

                            //addition
                            if(!IsBlack(colorLight))
                            {
                                if(pdfLight==0)//delta light
                                {
                                    Ld += color * colorLight;
                                }
                                else
                                {
                                    float weight = PowerHeuristic(1, pdfLight, 1, pdf);
                                    Ld += color * colorLight * weight / pdfLight;
                                }
                            }
                        }
                    }
#ifdef MY_DEBUG
                        qDebug() << "Ld after sample light" << Ld.x << "," << Ld.y << "," << Ld.z;
#endif

                    //Sample BSDF with multiple importance sampling
                    if(pdfLight!=0)//pdfLight==0 is delta light or pdf is infinity(manually set to 0) because AbsDot is 0
                    {
                        Vector3f wi_temp;
                        float pdf;
                        BxDFType type;
                        Color3f color = it.bsdf->Sample_f(wo, &wi_temp, sampler->Get2D(), &pdf, flags, &type);
                        color *= AbsDot(wi_temp, it.bsdf->normal);//has to be a separate sentance otherwise wi_temp will not be assigned

                        bool sampledSpecular = type & BSDF_SPECULAR;

                        if(!IsBlack(color) && pdf > 0)//pdf == 0 is total internal reflection
                        {
                            //visibility
                            Intersection shadow;
                            Ray r = it.SpawnRay(wi_temp);
                            Color3f colorLight2(0.f);
                            if(scene.Intersect(r, &shadow))
                            {
                                if(shadow.objectHit->GetAreaLight() == scene.lights[lightRand].get())
                                {
                                    colorLight2 = shadow.Le(-wi_temp);
                                }
                            }
                            else
                            {
                                //global light here
                                colorLight2 = scene.lights[lightRand]->Le(r);
                            }

                            //addition
                            if(!IsBlack(colorLight2))
                            {
                                if(!sampledSpecular)
                                {
                                    float pdfLight2 = scene.lights[lightRand]->Pdf_Li(it, wi_temp) / (float)lightCount;

                                    if(pdfLight2 == 0)
                                    {
                                        //if directional light, no chance to randomly hit it, do nothing
                                        //if delta light, indirect light sampling will never hit it, do nothing
                                        //if didn't hit light, do nothing
                                    }
                                    else
                                    {
                                        float weight = PowerHeuristic(1, pdf, 1, pdfLight2);
                                        Ld += color * colorLight2 * weight / pdf;
                                    }
                                }
                            }
                        }

                    }

#ifdef MY_DEBUG
                        qDebug() << "Ld after sample BSDF" << Ld.x << "," << Ld.y << "," << Ld.z;
#endif

                }


                //2.final color
                finalColor += Ld * throughput;
#ifdef MY_DEBUG
                        qDebug() << "after MIS finalColor:" << finalColor.x << "," << finalColor.y << "," << finalColor.z;
#endif

                Vector3f wi_global;
                //3.Global illumination
                {
                    float pdf;
                    BxDFType type;
                    Color3f color = it.bsdf->Sample_f(wo, &wi_global, sampler->Get2D(), &pdf, BSDF_ALL, &type);

                    if(pdf==0.f || IsBlack(color))//this is when internal reflection happens
                    {
                        //do something
#ifdef MY_DEBUG
                        qDebug() << "total internal reflection";
#endif
                        break;
                    }

                    specularBounce = type & BxDFType::BSDF_SPECULAR;

                    color *= AbsDot(wi_global, it.bsdf->normal) / pdf;//has to be a separate sentance otherwise wi_global will not be assigned

                    throughput *= color;
                }


                //4.Ray
                rayTemp = it.SpawnRay(wi_global);
            }
            else
            {
#ifdef MY_DEBUG
                        qDebug() << "no bsdf";
#endif
                break;
            }
        }
        else
        {
#ifdef MY_DEBUG
                        qDebug() << "no hit";
#endif
            break;
        }

#ifdef MY_DEBUG
                        qDebug() << "start throughput:" << throughput.x << "," << throughput.y << "," << throughput.z;
#endif

        if(depthMax - depth >= MINDEPTH )
        {
            float russian = sampler->Get1D();
            float maxThroughput = glm::max(glm::max(throughput.x, throughput.y),throughput.z);

#ifdef MY_DEBUG
                        qDebug() << "russian:" << russian;
#endif
            if(maxThroughput < russian)
                break;
            throughput/=maxThroughput;
        }

        depth--;
    }

#ifdef MY_DEBUG
    qDebug() << finalColor.x << "," << finalColor.y << "," << finalColor.z;
#endif

    return finalColor;
}
