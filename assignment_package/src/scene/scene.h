#pragma once
#include <QList>
#include <raytracing/film.h>
#include <scene/camera.h>
#include <scene/lights/light.h>

class Primitive;
class Material;
class Light;

class Scene
{
public:
    Scene();
    QList<std::shared_ptr<Primitive>> primitives;
    QList<std::shared_ptr<Material>> materials;
    QList<std::shared_ptr<Light>> lights;
    std::shared_ptr<Camera> camera;
    Film film;

    void SetCamera(std::shared_ptr<Camera> c);

    void CreateTestScene();
    void CreateMyScene();//me
    void Clear();

    bool Intersect(const Ray& ray, Intersection* isect) const;
};
