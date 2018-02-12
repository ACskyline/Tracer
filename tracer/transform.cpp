#include "transform.h"

Transform::Transform()
{

}

Transform::Transform(Vector3f t, Vector3f r, Vector3f s) :
    T(t), R(r), S(s)
{
    worldSRT = glm::translate(glm::mat4(),T)
            * glm::rotate(glm::mat4(), R.x, glm::vec3(1,0,0))
            * glm::rotate(glm::mat4(), R.y, glm::vec3(0,1,0))
            * glm::rotate(glm::mat4(), R.z, glm::vec3(0,0,1))
            * glm::scale(glm::mat4(),S);
    worldSRTInv = glm::inverse(worldSRT);
    worldSRTInvTrans = glm::transpose(worldSRTInv);
}
