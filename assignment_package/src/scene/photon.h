#pragma once
#include <la.h>
#include <globals.h>

class Photon
{
public:
    Point3f pos;
    Color3f color;
    Vector3f wi;

    Photon(Point3f p, Color3f c, Vector3f w)
        : pos(p), color(c), wi(w)
    {}
};
