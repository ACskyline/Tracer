#ifndef IMPLICITSURFACE_H
#define IMPLICITSURFACE_H

#include "shape.h"

#define WEELITTLE 0.01f
#define NORMAL_WEELITTLE 0.01f
#define START 0
#define END 100000
#define MAX_STEP 10000
#define STEP 0.05f
#define BACKSTEP 0.1f

class ImplicitSurface : public Shape
{
private:
    unsigned int SurfaceType;
    float start;
    float end;
    float maxStep;
    float step;
    float backStep;
    float epsilon;
    float epsilonNormal;

    float Sphere(Vector3f p) const;
    float Box(Vector3f p) const;
    float Heart(Vector3f p) const;
    float TangleCube(Vector3f p) const;
    Normal3f estimateNormal(Point3f p) const;
    float (ImplicitSurface::*f)(Vector3f) const;

public:

    ImplicitSurface();

    virtual bool Intersect(const Ray &ray, Intersection *isect);
    virtual Point2f GetUVCoordinates(const Point3f &point) const;
    virtual void ComputeTBN(const Point3f& P, Normal3f* nor, Vector3f* tan, Vector3f* bit) const;

    virtual float Area() const;

    virtual Intersection Sample(const Point2f &xi, Float *pdf) const;

    void create();
    void setSurfaceType(unsigned int type);
};

#endif // IMPLICITSURFACE_H
