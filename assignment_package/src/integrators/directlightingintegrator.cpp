#include "directlightingintegrator.h"

#ifdef MY_GLOBAL_DEBUG
#define DD
#endif

//#define NORMAL_ONLY

//#define MY_DEBUG

Color3f DirectLightingIntegrator::Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
{
    if(depth<0)
        return Color3f(0);

    Color3f finalColor;

    //1.Normal MIS
    Intersection it;

    if(scene.Intersect(ray, &it))
    {
#ifdef MY_DEBUG
        qDebug() << "!!!!!!camera to " << it.objectHit->name << " true";
#endif

#ifdef DD
            qDebug() << ">>>>>>intersect";
#endif
        Vector3f wo = -ray.direction;

#ifdef NORMAL_ONLY
        finalColor += (it.normalGeometric + glm::vec3(1,1,1) )/2.f;
#endif

#ifndef NORMAL_ONLY


#ifdef MY_DEBUG
            qDebug() << "!!!!!!produceBsdf";
#endif
        if(it.objectHit->ProduceBSDF(&it))
        {
#ifdef MY_DEBUG
            qDebug() << "!!!!!!produceBsdf true";
#endif

#ifdef DD
            qDebug() << " produceBsdf";

#endif
            int lightCount = scene.lights.size();
            int lightRand = sampler->Get1D() * lightCount;

#ifdef DD
            qDebug() << " light name " << scene.lights[lightRand]->name;
#endif

#ifdef MY_DEBUG
            qDebug() << "!!!!!!sample light start";
#endif
            Vector3f wi;
            float pdfLight;
            Color3f colorLight = scene.lights[lightRand]->Sample_Li(it, sampler->Get2D(), &wi, &pdfLight);
            //pdfLight /= lightCount;

#ifdef DD
            qDebug() << " colorLight "  << colorLight.x << "," << colorLight.y << "," << colorLight.z;
#endif

            Color3f Ld(0.f);//final color

            //sample light source with multiple importance sampling

#ifdef DD
            qDebug() << " sample light";
#endif


#ifdef MY_DEBUG
            qDebug() << "!!!!!!colorLight not black and pdfLight >= 0 ?";
#endif
            if(!IsBlack(colorLight) && pdfLight >= 0)//pdfLight==0 is delta light or pdf is infinity(manually set to 0) because AbsDot is 0
            {
#ifdef MY_DEBUG
            qDebug() << "!!!!!!colorLight not black and pdfLight >= 0 true";
#endif
                //BSDF
                float pdf = it.bsdf->Pdf(wo, wi);
#ifdef DD
                qDebug() << "  colorLight not black and pdfLight > 0";
                qDebug() << "  w2t row 1" << it.bsdf->worldToTangent[0][0] << "," << it.bsdf->worldToTangent[1][0] << "," << it.bsdf->worldToTangent[2][0];
                qDebug() << "  w2t row 2" << it.bsdf->worldToTangent[0][1] << "," << it.bsdf->worldToTangent[1][1] << "," << it.bsdf->worldToTangent[2][1];
                qDebug() << "  w2t row 3" << it.bsdf->worldToTangent[0][2] << "," << it.bsdf->worldToTangent[1][2] << "," << it.bsdf->worldToTangent[2][2];
                qDebug() << "  wo "  << wo.x << "," << wo.y << "," << wo.z;
                qDebug() << "  wi "  << wi.x << "," << wi.y << "," << wi.z;
                qDebug() << "  it.normalGeometric "  << it.normalGeometric.x << "," << it.normalGeometric.y << "," << it.normalGeometric.z;
                qDebug() << "  it.bsdf->normal " << it.bsdf->normal.x << "," << it.bsdf->normal.y << "," << it.bsdf->normal.z;
                qDebug() << "  pdf" << pdf;
#endif
                Color3f color = it.bsdf->f(wo, wi);
#ifdef DD
                qDebug() << "  f "  << color.x << "," << color.y << "," << color.z;
#endif

                float absdot = AbsDot(wi, it.bsdf->normal);
                color *= absdot;


#ifdef DD
                qDebug() << "  absdot "  << absdot;
#endif


#ifdef DD
                qDebug() << "  color "  << color.x << "," << color.y << "," << color.z;
#endif

#ifdef MY_DEBUG
            qDebug() << "!!!!!!color is not black and pdf > 0 ?";
#endif
                if(!IsBlack(color) && pdf > 0.f)
                {
#ifdef MY_DEBUG
            qDebug() << "!!!!!!color is not black and pdf > 0 true";
#endif
                    //visibility
                    Ray r = it.SpawnRay(wi);
                    Intersection shadow;
#ifdef MY_DEBUG
        qDebug() << "!!!!!!" << it.objectHit->name << " to " << r.direction.x << "," << r.direction.y << "," << r.direction.z;
#endif
                    if(scene.Intersect(r, &shadow))
                    {
#ifdef MY_DEBUG
        qDebug() << "!!!!!!" << it.objectHit->name << " to " << shadow.objectHit->name;
#endif
                        if(shadow.objectHit->GetAreaLight() != scene.lights[lightRand].get())
                        {
                            colorLight = Color3f(0.f);
                        }
                    }
                    else
                    {

#ifdef MY_DEBUG
        qDebug() << "!!!!!!" << it.objectHit->name << " to false";
#endif
                        //if this is arealight hitting nothing is error
                        //if this is not arealight, hitting nothing means no shadow
                    }
#ifdef DD
                    qDebug() << "   color not black";
                    qDebug() << "   colorLight"  << colorLight.x << "," << colorLight.y << "," << colorLight.z;
#endif
                    //addition
#ifdef MY_DEBUG
            qDebug() << "!!!!!!colorLight is not black ?";
#endif
                    if(!IsBlack(colorLight))
                    {
#ifdef MY_DEBUG
            qDebug() << "!!!!!!colorLight is not black true";
#endif

#ifdef DD
                        qDebug() << "    colorLight not black";
                        qDebug() << "    color" << color.x << "," << color.y << "," << color.z;
                        qDebug() << "    colorLight" << colorLight.x << "," << colorLight.y << "," << colorLight.z;
                        qDebug() << "    pdfLight" << pdfLight;
#endif

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
                    else
                    {

#ifdef MY_DEBUG
            qDebug() << "!!!!!!colorLight is not black false";
#endif
                    }
                }
                else
                {
#ifdef MY_DEBUG
            qDebug() << "!!!!!!color is not black and pdf > 0 false";
#endif
                }
            }
            else
            {
#ifdef MY_DEBUG
            qDebug() << "!!!!!!colorLight not black and pdfLight >= 0 false";
#endif
            }

#ifdef MY_DEBUG
            qDebug() << "!!!!!!sample light finish. Ld:" << Ld.x << "," << Ld.y << "," << Ld.z;
#endif

#ifdef DD
            qDebug() << " Ld" << Ld.x << "," << Ld.y << "," << Ld.z;
#endif

            //Sample BSDF with multiple importance sampling
#ifdef DD
            qDebug() << " sample bsdf";
#endif

#ifdef MY_DEBUG
        qDebug() << "!!!!!!sample bsdf start";
#endif
            if(pdfLight!=0)//pdfLight==0 is delta light or pdf is infinity(manually set to 0) because AbsDot is 0
            {
                Vector3f wi_temp;
                float pdf;
                BxDFType type;
                Color3f color = it.bsdf->Sample_f(wo, &wi_temp, sampler->Get2D(), &pdf, BSDF_ALL, &type);
                color *= AbsDot(wi_temp, it.bsdf->normal);//has to be a separate sentance otherwise wi_temp will not be assigned
#ifdef DD
                qDebug() << "  pdfLight not 0";
                qDebug() << "  wi_temp "  << wi_temp.x << "," << wi_temp.y << "," << wi_temp.z;
                qDebug() << "  it.normalGeometric "  << it.normalGeometric.x << "," << it.normalGeometric.y << "," << it.normalGeometric.z;
                qDebug() << "  color "  << color.x << "," << color.y << "," << color.z;
#endif
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
                        colorLight2 = scene.lights[lightRand]->Le(r);
                    }
#ifdef DD
                    qDebug() << "   color not black and pdf > 0";
                    qDebug() << "   colorLight2 "  << colorLight2.x << "," << colorLight2.y << "," << colorLight2.z;
#endif
                    //addition
                    if(!IsBlack(colorLight2))
                    {
                        if(!sampledSpecular)
                        {
                            float pdfLight2 = scene.lights[lightRand]->Pdf_Li(it, wi_temp);// / (float)lightCount;

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
        qDebug() << "!!!!!!sample bsdf finish. Ld:" << Ld.x << "," << Ld.y << "," << Ld.z;
#endif
            finalColor += it.Le(wo) + Ld * (float)scene.lights.size();
        }
        else
        {

#ifdef MY_DEBUG
            qDebug() << "!!!!!!produceBsdf false";
#endif
            finalColor += it.Le(wo);
        }
#endif
#ifdef MY_DEBUG
        qDebug() << "!!!!!!final color:" << finalColor.x << "," << finalColor.y << "," << finalColor.z;
#endif
#ifdef DD
    qDebug() << ">>>>>>final" << finalColor.x << "," << finalColor.y << "," << finalColor.z;
#endif
    }


    return finalColor;
}
