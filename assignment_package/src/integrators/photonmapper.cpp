#include "photonmapper.h"
#include "scene/lights/diffusearealight.h"
#include "warpfunctions.h"

#define CAUSTIC_ONLY

PhotonMapper::PhotonMapper(int numPhotons, std::vector<Photon> *_photons, std::vector<Photon> *_caustics, Scene *s, std::shared_ptr<Sampler> sampler, int recursionLimit)
    : Integrator(Bounds2i(Point2i(0,0), Point2i(0,0)), s, sampler, recursionLimit), preprocessing(true), numPhotons(numPhotons), photons(_photons), caustics(_caustics)
{}

PhotonMapper::PhotonMapper(Bounds2i bounds, Scene *s, std::shared_ptr<Sampler> sampler, int recursionLimit, KDTree* _kdTree, KDTree* _kdTreeCaustic, float _photonSearchRadius)
    : Integrator(bounds, s, sampler, recursionLimit), preprocessing(false), numPhotons(0), photons(nullptr), kdTree(_kdTree), kdTreeCaustic(_kdTreeCaustic), photonSearchRadius(_photonSearchRadius)
{}

void PhotonMapper::Render()
{
    // PhotonMapper's Render() function has
    // two modes: when preprocessing, it traces
    // a collection of photons through the scene
    // and stores them in the given k-d tree.
    // If not preprocessing, it runs like a regular Integrator
    // and invokes Li().
    if(preprocessing)
    {
        // TODO
        // Determine how many photons to assign to each light source
        // given numPhotons and the intensity of each light.
        // Shoot a number of photons equal to numPhotons from
        // the lights, bouncing them through the scene and pushing
        // back the result of each bounce to the photons vector
        // stored in the PhotonMapper.

        std::vector<DiffuseAreaLight*> DALights;//Only diffuse area lights have intensity at this point
        std::vector<float> intensityPerLight;
        float intensityTotal = 0;

        //distribute photon number
        for(auto item : scene->lights)
        {
            DiffuseAreaLight* daLight = dynamic_cast<DiffuseAreaLight*>(item.get());
            if(daLight!=nullptr)
            {
                DALights.push_back(daLight);
                float intensity = daLight->emittedLight.x + daLight->emittedLight.y + daLight->emittedLight.z;
                intensityTotal += intensity;
                intensityPerLight.push_back(intensity);
            }
        }

        //sending photons
        for(int i = 0;i<DALights.size();i++)//for each light
        {
            float photonNumThisLight = intensityPerLight[i] / intensityTotal * numPhotons;
            for(int j = 0;j<photonNumThisLight;j++)//for each photon
            {
                float pdfShape;//not used
                Intersection itShape = DALights[i]->shape->Sample(sampler->Get2D(), &pdfShape);
                Vector3f directionLocal = WarpFunctions::squareToHemisphereCosine(sampler->Get2D());
                //float pdfDirection = WarpFunctions::squareToHemisphereCosinePDF(directionLocal);//not used
                Vector3f localZ = itShape.normalGeometric;
                Vector3f localX;
                Vector3f localY;
                CoordinateSystem(localZ, &localX, &localY);
                glm::mat3 l2w(localX, localY, localZ);
                Vector3f directionWorld = glm::normalize(l2w * directionLocal);
                Ray r(itShape.point, directionWorld);
                Photon photonThisLight(itShape.point,DALights[i]->emittedLight / photonNumThisLight,-directionWorld);
                //float possibleMax = glm::max(photonThisLight.color.x,glm::max(photonThisLight.color.y,photonThisLight.color.z));
                bool AllSpecularBefore = false;
                Color3f throughput(1.f,1.f,1.f);
                for(int k = 0;k<recursionLimit;k++)//for each recursion
                {
                    Intersection it;
                    if(scene->Intersect(r, &it))
                    {
                        Vector3f wi = -r.direction;
                        if(it.objectHit->ProduceBSDF(&it))
                        {
                            Vector3f wo_temp;
                            float pdf_temp;
                            BxDFType type_temp;
                            Color3f color_temp = it.bsdf->Sample_f(wi, &wo_temp, sampler->Get2D(), &pdf_temp, BSDF_ALL, &type_temp);
                            throughput = color_temp * throughput * AbsDot(wi, it.bsdf->normal) / pdf_temp;

                            //update wi and pos, but leave color before storing
                            //  color
                            //     \     /
                            //   wi \   /
                            //       \ /
                            //      ----- pos
                            photonThisLight.wi = wi;
                            photonThisLight.pos = it.point;

                            if(!(type_temp&BSDF_SPECULAR))//only store non specular photon
                            {
                                if(!AllSpecularBefore&&k!=0)//not caustic and not direct light
                                {
                                    photons->push_back(photonThisLight);
                                }
                                else if(AllSpecularBefore&&k!=0)//caustic
                                {
                                    photons->push_back(photonThisLight);
                                    //also store in another kd tree
                                    caustics->push_back(photonThisLight);
                                }

                                AllSpecularBefore = false;
                            }
                            else//specular
                            {
                                if(k==0) AllSpecularBefore = true;
                            }

                            //update color now
                            photonThisLight.color = color_temp * photonThisLight.color * AbsDot(wi, it.bsdf->normal) / pdf_temp;


                            r = it.SpawnRay(wo_temp);
#define EARLY_TERMINATE
#ifdef EARLY_TERMINATE
                            float throughputMax = glm::max(throughput.x, glm::max(throughput.y, throughput.z));
                            float russian = sampler->Get1D();// * possibleMax;
                            if(k>3&&throughputMax<russian)
                                break;
#endif
                        }
                        else
                            break;
                    }
                    else
                        break;
                }
            }
        }
    }
    else
    {
        Integrator::Render(); // Invokes Li for each ray from a pixel
    }
}

Color3f PhotonMapper::Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
{

    if(depth<0)
        return Color3f(0);

    Color3f finalColor(0,0,0);

    //1.Normal MIS
    Intersection it;

    if(scene.Intersect(ray, &it))
    {
        Vector3f wo = -ray.direction;

        if(it.objectHit->ProduceBSDF(&it))
        {
#ifndef CAUSTIC_ONLY
//#define MYDEBUG
#ifndef MYDEBUG

            //qDebug() << "normal" << it.bsdf->normal.x << "," << it.bsdf->normal.y << "," << it.bsdf->normal.z;

            int lightCount = scene.lights.size();
            int lightRand = sampler->Get1D() * lightCount;

            Vector3f wi;
            float pdfLight;
            Color3f colorLight = scene.lights[lightRand]->Sample_Li(it, sampler->Get2D(), &wi, &pdfLight);
            pdfLight /= lightCount;


            Color3f Ld(0.f);//final color

            //sample light source with multiple importance sampling
            if(!IsBlack(colorLight) && pdfLight >= 0)//pdfLight==0 is delta light or pdf is infinity(manually set to 0) because AbsDot is 0
            {
                //BSDF
                float pdf = it.bsdf->Pdf(wo, wi);
                //                qDebug() << "w2t row 1" << it.bsdf->worldToTangent[0][0] << "," << it.bsdf->worldToTangent[1][0] << "," << it.bsdf->worldToTangent[2][0];
                //                qDebug() << "w2t row 2" << it.bsdf->worldToTangent[0][1] << "," << it.bsdf->worldToTangent[1][1] << "," << it.bsdf->worldToTangent[2][1];
                //                qDebug() << "w2t row 3" << it.bsdf->worldToTangent[0][2] << "," << it.bsdf->worldToTangent[1][2] << "," << it.bsdf->worldToTangent[2][2];
                //                qDebug() << "pdf" << pdf;
                Color3f color = it.bsdf->f(wo, wi) * AbsDot(wi, it.bsdf->normal);

                if(!IsBlack(color))
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
                        //                        qDebug() << "color" << color.x << "," << color.y << "," << color.z;
                        //                        qDebug() << "colorLight" << colorLight.x << "," << colorLight.y << "," << colorLight.z;
                        //                        qDebug() << "pdfLight" << pdfLight;

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


            //qDebug() << "Ld" << Ld.x << "," << Ld.y << "," << Ld.z;

            //Sample BSDF with multiple importance sampling
            if(pdfLight!=0)//pdfLight==0 is delta light or pdf is infinity(manually set to 0) because AbsDot is 0
            {
                Vector3f wi_temp;
                float pdf;
                BxDFType type;
                Color3f color = it.bsdf->Sample_f(wo, &wi_temp, sampler->Get2D(), &pdf, BSDF_ALL, &type);
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
                        colorLight2 = scene.lights[lightRand]->Le(r);
                    }

                    //addition
                    if(!IsBlack(colorLight2))
                    {
                        if(!sampledSpecular)
                        {
                            float pdfLight2 = scene.lights[lightRand]->Pdf_Li(it, wi_temp) / (float)lightCount;

                            if(pdfLight2 == 0)//either didn't hit the light or pdf is infinity(manually set to 0)
                            {
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

            finalColor += it.Le(wo) + Ld;

            //qDebug() << "final" << finalColor.x << "," << finalColor.y << "," << finalColor.z;

#endif
            //indirect light here
            //
            //
            Color3f phoColor(0.f);
            std::vector<Photon> pho = kdTree->particlesInSphere(it.point, photonSearchRadius, it.normalGeometric);
            //use another tree!! kdTreeCaustic
            float phoNum = pho.size();
            float area = glm::pi<float>() * photonSearchRadius * photonSearchRadius;
            for(int m = 0;m<phoNum;m++)
            {
                phoColor += pho[m].color * it.bsdf->f(wo, pho[m].wi) * AbsDot(pho[m].wi, it.bsdf->normal) / it.bsdf->Pdf(wo, pho[m].wi);
            }
            phoColor /= area;
            finalColor+=phoColor;
#else
            Color3f phoColor(0.f);
            std::vector<Photon> pho = kdTreeCaustic->particlesInSphere(it.point, photonSearchRadius, it.normalGeometric);
            //use another tree!! kdTreeCaustic
            float phoNum = pho.size();
            float area = glm::pi<float>() * photonSearchRadius * photonSearchRadius;
            for(int m = 0;m<phoNum;m++)
            {
                phoColor += pho[m].color * it.bsdf->f(wo, pho[m].wi) * AbsDot(pho[m].wi, it.bsdf->normal) / it.bsdf->Pdf(wo, pho[m].wi);
            }
            phoColor /= area;
            finalColor+=phoColor;
#endif

        }
        else
        {
            finalColor += it.Le(wo);
        }
    }


    return finalColor;
}
