#include "environmentlight.h"
#include "warpfunctions.h"

Color3f EnvironmentLight::Sample_Li(const Intersection &ref, const Point2f &xi, Vector3f *wi, Float *pdf) const
{
    Point3f w = WarpFunctions::squareToSphereUniform(xi);
    *pdf = WarpFunctions::squareToSphereUniformPDF(w);

    //light coordinates, y points to up
    w = Point3f(w.y, w.z, w.x);
    *wi = glm::normalize(transform.T3() * w);
    float Phi = glm::atan(w.x, w.z) < 0 ? glm::atan(w.x, w.z) + 2 * Pi : glm::atan(w.x, w.z);
    float Theta = glm::acos(glm::clamp(w.y, -1.f, 1.f));

    Point2f uv_coord(Phi*Inv2Pi, Theta*InvPi);
    Color3f result = L;
#ifdef MY_DEBUG
    qDebug() << w.x << "," << w.y << "," << w.z << ":" << uv_coord.x << "," << uv_coord.y;
#endif
    if(textureMap.get())
    {
        int X = glm::min(textureMap->width() * uv_coord.x, textureMap->width() - 1.0f);
        int Y = glm::min(textureMap->height() * (uv_coord.y), textureMap->height() - 1.0f);
        QColor color = textureMap->pixel(X, Y);
        result *= Color3f(color.red(), color.green(), color.blue())/255.0f;
    }
#ifdef MY_DEBUG
    //qDebug() << glm::acos(0.f);
    qDebug() << "sample light";
    qDebug() << result.x << "," << result.y << "," << result.z;
#endif
    return result;
}

float EnvironmentLight::Pdf_Li(const Intersection &ref, const Vector3f &wi) const
{
    //light coordinates, y points to up
    Vector3f w = Vector3f(transform.invT() * glm::vec4(wi, 0));
    float Phi = glm::atan(w.x, w.z) < 0 ? glm::atan(w.x, w.z) + 2 * Pi : glm::atan(w.x, w.z);
    float Theta = glm::acos(glm::clamp(w.y, -1.f, 1.f));
    float sinTheta = glm::sin(Theta);
    if(sinTheta==0) return 0;

    return WarpFunctions::squareToSphereUniformPDF(Point3f(w.z,w.x,w.y));
}

Color3f EnvironmentLight::Le(const Ray &r) const
{
    Color3f result = L;
    Vector3f w = glm::normalize(Vector3f(transform.invT() * glm::vec4(r.direction,0)));

    //light coordinates, y points to up
    float Phi = glm::atan(w.x, w.z) < 0 ? glm::atan(w.x, w.z) + 2 * Pi : glm::atan(w.x, w.z);
    float Theta = glm::acos(glm::clamp(w.y, -1.f, 1.f));

    Point2f uv_coord(Phi*Inv2Pi, Theta*InvPi);


    if(textureMap.get())
    {
        int X = glm::min(textureMap->width() * uv_coord.x, textureMap->width() - 1.0f);
        int Y = glm::min(textureMap->height() * (uv_coord.y), textureMap->height() - 1.0f);
        QColor color = textureMap->pixel(X, Y);
        result *= Color3f(color.red(), color.green(), color.blue())/255.0f;
    }
#ifdef MY_DEBUG
    qDebug() << "sample bsdf but did not hit anything";
    qDebug() << result.x << "," << result.y << "," << result.z;
#endif
    return result;
}

