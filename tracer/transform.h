#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "globalincludes.h"

class Transform
{
public:
    Transform();
    Transform(Vector3f t, Vector3f r, Vector3f s);
    Vector3f T;
    Vector3f S;
    Vector3f R;
    glm::mat4 worldSRT;
    glm::mat4 worldSRTInv;
    glm::mat4 worldSRTInvTrans;

};

#endif // TRANSFORM_H
