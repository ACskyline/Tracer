#include "implicitsurface.h"

#ifdef MY_GLOBAL_DEBUG
#define MY_DEBUG
#endif

ImplicitSurface::ImplicitSurface() : SurfaceType(0)
{

}

float ImplicitSurface::Area() const
{
    return 0;
}


bool ImplicitSurface::Intersect(const Ray &r, Intersection *isect)
{
#ifdef MY_DEBUG
    qDebug() << "enter";
#endif

    Ray r_loc = r.GetTransformedCopy(transform.invT());

    Point3f eye = r_loc.origin;
    Vector3f direction = r_loc.direction;

    float length = glm::length(direction);

    float depth = start;

    for(int i = 0;i<maxStep;i++)
    {
        Point3f P = eye + depth/length * direction;
        float dist = (this->*f)(P);

#ifdef MY_DEBUG
        qDebug() << "step:" << i << "=>depth:" << depth << "=>dist:" << dist ;
#endif

        if(dist <= epsilon){

            while(dist<backStep)//0.f)
            {
                P = P - backStep/length * direction;
                dist = (this->*f)(P);
            }

            //found
            depth /= length;
            InitializeIntersection(isect, depth, P);
#ifdef MY_DEBUG
            qDebug() << "true";
#endif
            return true;
        }

        //depth += dist;
        depth += step;

        if(depth > end)
            break;
    }

#ifdef MY_DEBUG
    qDebug() << "false";
#endif
    return false;
}

Point2f ImplicitSurface::GetUVCoordinates(const Point3f &point) const
{
    return Point2f();
}

void ImplicitSurface::ComputeTBN(const Point3f &P, Normal3f *nor, Vector3f *tan, Vector3f *bit) const
{
//    Normal3f n = estimateNormal(P);
//    *nor = glm::normalize(transform.invTransT() * n);
//    Vector3f t, b;
//    CoordinateSystem(n, &t, &b);
//    *tan = glm::normalize(transform.T3() * t);
//    *bit = glm::normalize(transform.T3() * b);
    *nor = glm::normalize(transform.invTransT() * estimateNormal(P));
    CoordinateSystem(*nor, tan, bit);
}

Intersection ImplicitSurface::Sample(const Point2f &xi, Float *pdf) const
{
    return Intersection();
}


Normal3f ImplicitSurface::estimateNormal(Point3f p) const
{
    return glm::normalize(glm::vec3(
                              (this->*f)(glm::vec3(p.x + epsilonNormal, p.y, p.z)) - (this->*f)(glm::vec3(p.x - epsilonNormal, p.y, p.z)),
                              (this->*f)(glm::vec3(p.x, p.y + epsilonNormal, p.z)) - (this->*f)(glm::vec3(p.x, p.y - epsilonNormal, p.z)),
                              (this->*f)(glm::vec3(p.x, p.y, p.z + epsilonNormal)) - (this->*f)(glm::vec3(p.x, p.y, p.z - epsilonNormal))));
}

void ImplicitSurface::setSurfaceType(unsigned int type)
{
    start = START;
    end = END;
    maxStep = MAX_STEP;
    step = STEP;
    backStep = BACKSTEP;
    epsilon = WEELITTLE;
    epsilonNormal = NORMAL_WEELITTLE;

    switch(type)
    {
    case 0:
        f = &ImplicitSurface::Sphere;
        epsilon = 0.0001f;
        break;
    case 1:
        f = &ImplicitSurface::Box;
        break;
    case 2:
        f = &ImplicitSurface::Heart;
        epsilon = 0.0000001f;
        step = 0.01f;
        backStep = 0.7f;//0.5f
        epsilonNormal = 0.007f;//0.005f
        break;
    case 3:
        f = &ImplicitSurface::TangleCube;
        break;
    default:
        f = &ImplicitSurface::Sphere;
        epsilon = 0.0001f;
        break;
    }
}

float ImplicitSurface::Sphere(Vector3f p) const
{
    return glm::length(p) - 1;//radius is 1
}

float ImplicitSurface::Box(Vector3f p) const
{
    glm::vec3 b(1,1,1);
    glm::vec3 d = glm::abs(p) - b;
    return glm::min(glm::max(d.x,glm::max(d.y,d.z)),0.f) + glm::length(glm::max(d,glm::vec3(0, 0, 0)));
}

float ImplicitSurface::Heart(Vector3f p) const
{
    float temp = 2.f*p.x*p.x + p.y*p.y + p.z*p.z - 1.f;

    return temp * temp * temp - 0.1f*p.x*p.x*p.z*p.z*p.z - p.y*p.y*p.z*p.z*p.z;
}

float ImplicitSurface::TangleCube(Vector3f p) const
{
    float temp = p.x * p.x * p.x * p.x - 5.0f * p.x * p.x
            + p.y * p.y * p.y * p.y - 5.0f * p.y * p.y
            + p.z * p.z * p.z * p.z - 5.0f * p.z * p.z
            + 11.8f;

    return temp;//(temp < 0.f) ? -pow(-temp, 1.0 / 8.0) : pow(temp, 1.0 / 8.0);
}
