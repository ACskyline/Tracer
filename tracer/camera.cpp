#include "camera.h"
#include <QDebug>

Camera::Camera()
{

}

Camera::Camera(Point3f _eye, Point3f _ref, Vector3f _worldUp,
               float _fovY, float _near, float _far, int _width, int _height) :
    eye(_eye), ref(_ref), worldUp(_worldUp), fovY(_fovY), near(_near), far(_far),
    width(_width), height(_height)
{
    viewProj = glm::perspectiveFov(fovY, (float)width, (float)height, near, far)
            * glm::lookAt(eye, ref, worldUp);
    viewProjInv = glm::inverse(viewProj);
}

Ray Camera::rayCast(Point2f p)
{
    glm::vec4 screen = glm::vec4(2.f * p.x/width - 1.f, 1.f - 2.f * p.y/height, 1, 1) * far;
    glm::vec4 world = viewProjInv * screen;
//    qDebug() << world.x << "," << world.y << "," << world.z << "," << world.w;
    Ray result(eye, glm::normalize(glm::vec3(world.x - eye.x, world.y - eye.y, world.z - eye.z)));
    return result;
}
