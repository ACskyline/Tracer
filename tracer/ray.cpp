#include "ray.h"

Ray::Ray()
{

}

Ray::Ray(Point3f ori, Vector3f dir) : origin(ori), direction(dir)
{

}

Ray Ray::transform(glm::mat4 m)
{
    glm::vec4 a = glm::vec4(origin, 1.f);
    glm::vec4 b = glm::vec4(direction, 0.f);
    a = m * a;
    b = m * b;
    Ray result(Point3f(a.x,a.y,a.z),Vector3f(b.x,b.y,b.z));
    return result;
}
