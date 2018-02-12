#include "arealight.h"
#include "sampler.h"
#include "disk.h"
#include "squareplane.h"
#include "sphere.h"
#include "speculartransmissionmaterial.h"

AreaLight::AreaLight()
{

}

AreaLight::AreaLight(Color3f col, std::shared_ptr<Shape> sha) : Light(col), shape(sha)
{

}

float AreaLight::isVisible(Point3f p, Intersection *it, Ray *out, Scene &scene)
{
    float result = 0;
    glm::vec3 end(0,0,0);

    Sampler sam;
    int numSamples = SAMPLE;
    WarpMethod wp = NONE;
    if(dynamic_cast<Disk*>(shape.get())!=nullptr) wp = DISK_CONCENTRIC;
    if(dynamic_cast<Sphere*>(shape.get())!=nullptr) wp = SPHERE;

    sam.generateSamples(numSamples, GRID, wp);
    const std::vector<float>& samples = sam.peekSamples();

    for(int i = 0;i<samples.size();i+=3)
    {
        glm::vec3 positionLocal(samples[i],samples[i+1],samples[i+2]);
        glm::vec3 position = glm::vec3(shape->transform.worldSRT * glm::vec4(positionLocal, 1.f));

        Ray r(p + float(BIAS) * glm::normalize(position - p), glm::normalize(position - p));
        bool hit = scene.getIntersection(r, it);

        if(hit)//hit
        {
            if(glm::length(it->point-r.origin) > glm::length(position-r.origin))//not blocked
            {
                end += glm::normalize(position - p);
                result += 1;
            }
            else//blocked
            {
                if(dynamic_cast<SpecularTransmissionMaterial*>(it->objectHit->mat.get())!=nullptr)//refractive material
                {
                    Ray R(it->point + float(BIAS)*r.direction, r.direction);
                    Intersection It;
                    if(isVisible(it->point, &It, &R, scene))
                    {
                        end += glm::normalize(position - p);
                        result += 1;
                    }
                    else
                    {
                        //do nothing
                    }
                }
                else//other material
                {
                    //do nothing
                }
            }
        }
        else//not hit
        {
            end += glm::normalize(position - p);
            result += 1;
        }
    }
    end /= result;
    *out = Ray(p, glm::normalize(end));
    result /= numSamples;

    return result;
}
