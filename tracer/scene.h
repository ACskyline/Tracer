#ifndef SCENE_H
#define SCENE_H

#include <QList>
#include <QImage>
#include "globalincludes.h"
#include "camera.h"
#include "primitive.h"
#include "light.h"

class Primitive;
class Light;

class Scene
{
public:
    Scene();
    Scene(Camera _camera);
    Camera camera;
    QList<std::shared_ptr<Primitive>> primitiveList;
    QList<std::shared_ptr<Light>> lightList;
    bool getIntersection(Ray r, Intersection* intersection);
    QImage renderScene();
    void initializeScene();
    void testScene();
    void cornellScene();
    void arealightScene();
    void aoScene();
    void aoAreaLightCornell();
};

#endif // SCENE_H
