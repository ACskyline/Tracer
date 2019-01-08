#define _USE_MATH_DEFINES
#include "warpfunctions.h"
#include <math.h>

Point3f WarpFunctions::squareToDiskUniform(const Point2f &sample)
{
    //TODO
    return glm::sqrt(sample.y) * glm::vec3(glm::cos(2.f*glm::pi<float>()*sample.x), glm::sin(2.f*glm::pi<float>()*sample.x), 0.f);
    //throw std::runtime_error("You haven't yet implemented uniform disk warpi<float>()ng!");
}

Point3f WarpFunctions::squareToDiskConcentric(const Point2f &sample)
{
    //TODO
    float phi = 0;
    float a = 2.f*sample.x - 1.f;
    float b = 2.f*sample.y - 1.f;
    float r = 0;

    if(a>-b)
    {
        if(a>b)
        {
            r = a;
            phi = (glm::pi<float>() / 4.f) * (b / a);
        }
        else
        {
            r = b;
            phi = (glm::pi<float>() / 4.f) * (2.f - a / b);
        }
    }
    else
    {
        if(a<b)
        {
            r = -a;
            phi = (glm::pi<float>() / 4.f) * (4.f + (b/a));
        }
        else
        {
            r = -b;
            if(b!=0)
                phi = (glm::pi<float>()/4.f) * (6.f - (a/b));
            else
                phi = 0;
        }
    }
    return glm::vec3(r*glm::cos(phi), r*glm::sin(phi), 0);
    //throw std::runtime_error("You haven't yet implemented concentric disk warpi<float>()ng!");
}

float WarpFunctions::squareToDiskPDF(const Point3f &sample)
{
    //TODOS
    return 1.f/glm::pi<float>();
    //return 0;
}

Point3f WarpFunctions::squareToSphereUniform(const Point2f &sample)
{
    //TODO
    return glm::vec3(glm::cos(2*glm::pi<float>()*sample.y)*2*glm::sqrt(sample.x*(1-sample.x)),
                     glm::sin(2*glm::pi<float>()*sample.y)*2*glm::sqrt(sample.x*(1-sample.x)),
                     1-2.f*sample.x);
    //z is up
    //throw std::runtime_error("You haven't yet implemented uniform sphere warpi<float>()ng!");
}

float WarpFunctions::squareToSphereUniformPDF(const Point3f &sample)
{
    //TODO
    return 1.f/(4*glm::pi<float>());
    //return 0;
}

Point3f WarpFunctions::squareToSphereCapUniform(const Point2f &sample, float thetaMin)
{
    //TODO
    float z = glm::cos(glm::radians(180 - thetaMin)) + sample.x*(1-glm::cos(glm::radians(180 - thetaMin)));
    return glm::vec3(glm::cos(2*glm::pi<float>()*sample.y)*glm::sqrt(1-z*z),
                     glm::sin(2*glm::pi<float>()*sample.y)*glm::sqrt(1-z*z),
                     z);
    //throw std::runtime_error("You haven't yet implemented sphere cap warpi<float>()ng!");
}

float WarpFunctions::squareToSphereCapUniformPDF(const Point3f &sample, float thetaMin)
{
    //TODO
    return 1.f/(2*glm::pi<float>()*(1 - glm::cos(glm::radians(180 - thetaMin))));
    //return 0;
}

Point3f WarpFunctions::squareToHemisphereUniform(const Point2f &sample)
{
    //TODO
    return glm::vec3(glm::cos(2*glm::pi<float>()*sample.y)*glm::sqrt(1-sample.x*sample.x),
                     glm::sin(2*glm::pi<float>()*sample.y)*glm::sqrt(1-sample.x*sample.x),
                     sample.x);
    //throw std::runtime_error("You haven't yet implemented uniform hemisphere warpi<float>()ng!");
}

float WarpFunctions::squareToHemisphereUniformPDF(const Point3f &sample)
{
    //TODO
    return 1.f/(2*glm::pi<float>());
    //return 0;
}

Point3f WarpFunctions::squareToHemisphereCosine(const Point2f &sample)
{
    //TODO
    glm::vec3 temp = squareToDiskConcentric(sample);//squareToDiskUniform(sample);
    temp.z = glm::sqrt(1 - temp.x*temp.x - temp.y*temp.y);
    return temp;
    //throw std::runtime_error("You haven't yet implemented cosine-weighted hemisphere warpi<float>()ng!");
}

float WarpFunctions::squareToHemisphereCosinePDF(const Point3f &sample)
{
    //TODO
    return glm::abs(glm::dot(sample, glm::vec3(0,0,1))) / glm::pi<float>();
    //return 0;
}
