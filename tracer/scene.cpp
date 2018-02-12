#include "scene.h"
#include "sphere.h"
#include "squareplane.h"
#include "lambertmaterial.h"
#include "blinnphongmaterial.h"
#include "specularreflectionmaterial.h"
#include "speculartransmissionmaterial.h"
#include "pointlight.h"
#include "arealight.h"
#include <QColor>
#include <QDebug>

Scene::Scene()
{

}

Scene::Scene(Camera _camera)
{
    camera = _camera;
}

bool Scene::getIntersection(Ray r, Intersection *intersection)
{
    float minT = std::numeric_limits<float>::max();
    bool result = false;
    for(int i = 0;i<primitiveList.size();i++)
    {
        Intersection I;
        if(primitiveList.at(i)->getIntersection(r, &I))
        {
            //qDebug() << i << ":" << I.t;
            if(I.t < minT&&I.t > float(BIAS))
            {
                I.objectHit = primitiveList.at(i);//game changer
                minT = I.t;
                *intersection = I;
                result = true;
            }
        }
    }
    return result;
}

void Scene::initializeScene()
{
    //cornellScene();
    //arealightScene();
    //aoScene();
    aoAreaLightCornell();
}

void Scene::aoAreaLightCornell()
{
    std::shared_ptr<Primitive> primSquaB = std::make_shared<Primitive>("backwall");
    std::shared_ptr<SquarePlane> squaB = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0,0,-5),Vector3f(0,0,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaR = std::make_shared<Primitive>("rightwall");
    std::shared_ptr<SquarePlane> squaR = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(5,0,0),Vector3f(0,-90,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaL = std::make_shared<Primitive>("leftwall");
    std::shared_ptr<SquarePlane> squaL = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(-5,0,0),Vector3f(0,90,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaU = std::make_shared<Primitive>("ceiling");
    std::shared_ptr<SquarePlane> squaU = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0,5,0),Vector3f(90,0,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaD = std::make_shared<Primitive>("floor");
    std::shared_ptr<SquarePlane> squaD = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0,-5,0),Vector3f(-90,0,0),Vector3f(10,10,1)));


    std::shared_ptr<Primitive> primSphe1 = std::make_shared<Primitive>("sphere1");
    std::shared_ptr<Sphere> sphe1 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(-2,-5,0),Vector3f(0,0,0),Vector3f(5,5,5)));

    std::shared_ptr<Primitive> primSphe2 = std::make_shared<Primitive>("sphere2");
    std::shared_ptr<Sphere> sphe2 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(-3,0,2),Vector3f(0,0,0),Vector3f(4,4,4)));

    std::shared_ptr<Primitive> primSphe3 = std::make_shared<Primitive>("sphere3");
    std::shared_ptr<Sphere> sphe3 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(-3,0,2),Vector3f(0,0,0),Vector3f(2.5,2.5,2.5)));

    std::shared_ptr<Primitive> primSphe4 = std::make_shared<Primitive>("sphere4");
    std::shared_ptr<Sphere> sphe4 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(3,-2,-3),Vector3f(0,0,0),Vector3f(4,4,4)));

    std::shared_ptr<Primitive> primSphe5 = std::make_shared<Primitive>("sphere5");
    std::shared_ptr<Sphere> sphe5 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(3,-2,-3),Vector3f(0,0,0),Vector3f(1,1,1)));

    std::shared_ptr<Primitive> primSphe6 = std::make_shared<Primitive>("sphere6");
    std::shared_ptr<Sphere> sphe6 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(-3,3.5,-3),Vector3f(0,0,0),Vector3f(3,3,3)));

    std::shared_ptr<Primitive> primSphe7 = std::make_shared<Primitive>("sphere7");
    std::shared_ptr<Sphere> sphe7 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(3,3.5,-3),Vector3f(0,0,0),Vector3f(3,3,3)));

    std::shared_ptr<Primitive> primSphe8 = std::make_shared<Primitive>("sphere8");
    std::shared_ptr<Sphere> sphe8 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(3,3.5,-3),Vector3f(0,0,0),Vector3f(1,1,1)));

    std::shared_ptr<Primitive> primSphe9 = std::make_shared<Primitive>("sphere9");
    std::shared_ptr<Sphere> sphe9 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(0,0,0),Vector3f(0,0,0),Vector3f(2,2,2)));

    std::shared_ptr<LambertMaterial> matLamWhite = std::make_shared<LambertMaterial>("whitediff", Color3f(1,1,1));
    std::shared_ptr<LambertMaterial> matLamRed = std::make_shared<LambertMaterial>("reddiff", Color3f(1,0,0));
    std::shared_ptr<LambertMaterial> matLamGreen = std::make_shared<LambertMaterial>("greendiff", Color3f(0,1,0));
    std::shared_ptr<LambertMaterial> matLamBlue = std::make_shared<LambertMaterial>("bluediff", Color3f(0,0,1));
    std::shared_ptr<LambertMaterial> matLamYellow = std::make_shared<LambertMaterial>("yellowdiff", Color3f(1,1,0));

    std::shared_ptr<BlinnPhongMaterial> matBliPhoPurple = std::make_shared<BlinnPhongMaterial>("purpleSpec", Color3f(0.75,0,0.75), 10);

    std::shared_ptr<SpecularReflectionMaterial> matSpecRef = std::make_shared<SpecularReflectionMaterial>("specRef", Color3f(1,1,1), 10, 0.3);
    std::shared_ptr<SpecularReflectionMaterial> matSpecRef2 = std::make_shared<SpecularReflectionMaterial>("specRef", Color3f(0.25,0.5,1), 10, 0.3);

    std::shared_ptr<SpecularTransmissionMaterial> matSpecTrans = std::make_shared<SpecularTransmissionMaterial>("specTrans", Color3f(1,1,1), 10, 1, 0.7, 0.9);
    std::shared_ptr<SpecularTransmissionMaterial> matSpecTrans2 = std::make_shared<SpecularTransmissionMaterial>("specTrans", Color3f(0.15,1,0.15), 10, 1, 0.4, 0.6);
    std::shared_ptr<SpecularTransmissionMaterial> matSpecTrans3 = std::make_shared<SpecularTransmissionMaterial>("specTrans", Color3f(1,1,1), 10, 1, 0.9, 0.7);

    primSquaB->shape = squaB;
    primSquaR->shape = squaR;
    primSquaL->shape = squaL;
    primSquaU->shape = squaU;
    primSquaD->shape = squaD;

    primSquaB->mat = matLamWhite;
    primSquaR->mat = matLamGreen;
    primSquaL->mat = matLamRed;
    primSquaU->mat = matLamWhite;
    primSquaD->mat = matLamWhite;

    primSphe1->shape = sphe1;
    primSphe2->shape = sphe2;
    primSphe3->shape = sphe3;
    primSphe4->shape = sphe4;
    primSphe5->shape = sphe5;
    primSphe6->shape = sphe6;
    primSphe7->shape = sphe7;
    primSphe8->shape = sphe8;
    primSphe9->shape = sphe9;

    primSphe1->mat = matBliPhoPurple;
    primSphe2->mat = matLamWhite;//matSpecTrans;
    primSphe3->mat = matLamWhite;//matSpecTrans3;
    primSphe4->mat = matLamWhite;//matSpecTrans2;
    primSphe5->mat = matLamWhite;
    primSphe6->mat = matLamWhite;//matSpecRef2;
    primSphe7->mat = matLamWhite;//matSpecTrans2;
    primSphe8->mat = matLamWhite;//matSpecRef2;
    primSphe9->mat = matLamYellow;

    primitiveList.append(primSquaB);
    primitiveList.append(primSquaR);
    primitiveList.append(primSquaL);
    primitiveList.append(primSquaU);
    primitiveList.append(primSquaD);

    primitiveList.append(primSphe1);
    primitiveList.append(primSphe2);
    primitiveList.append(primSphe3);
    primitiveList.append(primSphe4);
    primitiveList.append(primSphe5);
    primitiveList.append(primSphe6);
    primitiveList.append(primSphe7);
    primitiveList.append(primSphe8);
    primitiveList.append(primSphe9);

    //light

    std::shared_ptr<SquarePlane> areaSqua1 = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0, 3.5, 0),Vector3f(90,0,0),Vector3f(1,1,1)));
    std::shared_ptr<AreaLight> lightArea1 = std::make_shared<AreaLight>(Color3f(1,1,1), areaSqua1);

    lightList.append(lightArea1);
}

void Scene::aoScene()
{
    std::shared_ptr<Primitive> primSquaB = std::make_shared<Primitive>("backwall");
    std::shared_ptr<SquarePlane> squaB = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0,0,-5),Vector3f(0,0,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaR = std::make_shared<Primitive>("rightwall");
    std::shared_ptr<SquarePlane> squaR = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(5,0,0),Vector3f(0,-90,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaL = std::make_shared<Primitive>("leftwall");
    std::shared_ptr<SquarePlane> squaL = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(-5,0,0),Vector3f(0,90,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaU = std::make_shared<Primitive>("ceiling");
    std::shared_ptr<SquarePlane> squaU = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0,5,0),Vector3f(90,0,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaD = std::make_shared<Primitive>("floor");
    std::shared_ptr<SquarePlane> squaD = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0,-5,0),Vector3f(-90,0,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primBoxF = std::make_shared<Primitive>("front");
    std::shared_ptr<SquarePlane> squaBF = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(-4,-4,0.5),Vector3f(0,0,0),Vector3f(1,1,1)));

    std::shared_ptr<Primitive> primBoxB = std::make_shared<Primitive>("back");
    std::shared_ptr<SquarePlane> squaBB = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(-4,-4,-0.5),Vector3f(180,0,0),Vector3f(1,1,1)));

    std::shared_ptr<Primitive> primBoxR = std::make_shared<Primitive>("right");
    std::shared_ptr<SquarePlane> squaBR = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0.5-4,-4,0),Vector3f(0,90,0),Vector3f(1,1,1)));

    std::shared_ptr<Primitive> primBoxL = std::make_shared<Primitive>("left");
    std::shared_ptr<SquarePlane> squaBL = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0.5-4,-4,0),Vector3f(0,-90,0),Vector3f(1,1,1)));

    std::shared_ptr<Primitive> primBoxU = std::make_shared<Primitive>("top");
    std::shared_ptr<SquarePlane> squaBU = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(-4,0.5-4,0),Vector3f(-90,0,0),Vector3f(1,1,1)));

    std::shared_ptr<Primitive> primBoxD = std::make_shared<Primitive>("bottom");
    std::shared_ptr<SquarePlane> squaBD = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(-4,-0.5-4,0),Vector3f(90,0,0),Vector3f(1,1,1)));

    std::shared_ptr<Primitive> primSphe1 = std::make_shared<Primitive>("sphere1");
    std::shared_ptr<Sphere> sphe1 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(1.5,-4.3,0),Vector3f(0,0,0),Vector3f(2,1,2)));

    std::shared_ptr<LambertMaterial> matLamWhite = std::make_shared<LambertMaterial>("whitediff", Color3f(1,1,1));

    primSquaB->shape = squaB;
    primSquaR->shape = squaR;
    primSquaL->shape = squaL;
    primSquaU->shape = squaU;
    primSquaD->shape = squaD;

    primSquaB->mat = matLamWhite;
    primSquaR->mat = matLamWhite;
    primSquaL->mat = matLamWhite;
    primSquaU->mat = matLamWhite;
    primSquaD->mat = matLamWhite;

    primBoxF->shape = squaBF;
    primBoxB->shape = squaBB;
    primBoxR->shape = squaBR;
    primBoxL->shape = squaBL;
    primBoxU->shape = squaBU;
    primBoxD->shape = squaBD;

    primBoxF->mat = matLamWhite;
    primBoxB->mat = matLamWhite;
    primBoxR->mat = matLamWhite;
    primBoxL->mat = matLamWhite;
    primBoxU->mat = matLamWhite;
    primBoxD->mat = matLamWhite;

    primSphe1->shape = sphe1;
    primSphe1->mat = matLamWhite;

    primitiveList.append(primSquaB);
    primitiveList.append(primSquaR);
    primitiveList.append(primSquaL);
    primitiveList.append(primSquaU);
    primitiveList.append(primSquaD);

    primitiveList.append(primBoxF);
    primitiveList.append(primBoxB);
    primitiveList.append(primBoxR);
    primitiveList.append(primBoxL);
    primitiveList.append(primBoxU);
    primitiveList.append(primBoxD);

    primitiveList.append(primSphe1);
}

void Scene::arealightScene()
{
    std::shared_ptr<Primitive> primSquaB = std::make_shared<Primitive>("backwall");
    std::shared_ptr<SquarePlane> squaB = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0,0,-5),Vector3f(0,0,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaR = std::make_shared<Primitive>("rightwall");
    std::shared_ptr<SquarePlane> squaR = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(5,0,0),Vector3f(0,-90,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaL = std::make_shared<Primitive>("leftwall");
    std::shared_ptr<SquarePlane> squaL = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(-5,0,0),Vector3f(0,90,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaU = std::make_shared<Primitive>("ceiling");
    std::shared_ptr<SquarePlane> squaU = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0,5,0),Vector3f(90,0,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaD = std::make_shared<Primitive>("floor");
    std::shared_ptr<SquarePlane> squaD = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0,-5,0),Vector3f(-90,0,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSphe1 = std::make_shared<Primitive>("sphere1");
    std::shared_ptr<Sphere> sphe1 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(-2,0,0),Vector3f(0,0,0),Vector3f(1,1,1)));

    std::shared_ptr<LambertMaterial> matLamWhite = std::make_shared<LambertMaterial>("whitediff", Color3f(1,1,1));

    primSquaB->shape = squaB;
    primSquaR->shape = squaR;
    primSquaL->shape = squaL;
    primSquaU->shape = squaU;
    primSquaD->shape = squaD;

    primSquaB->mat = matLamWhite;
    primSquaR->mat = matLamWhite;
    primSquaL->mat = matLamWhite;
    primSquaU->mat = matLamWhite;
    primSquaD->mat = matLamWhite;

    primSphe1->shape = sphe1;
    primSphe1->mat = matLamWhite;

    primitiveList.append(primSquaB);
    primitiveList.append(primSquaR);
    primitiveList.append(primSquaL);
    primitiveList.append(primSquaU);
    primitiveList.append(primSquaD);
    primitiveList.append(primSphe1);

    std::shared_ptr<SquarePlane> areaSqua = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0, 2.5, 0),Vector3f(90,0,0),Vector3f(1,1,1)));
    std::shared_ptr<AreaLight> lightArea1 = std::make_shared<AreaLight>(Color3f(1,1,1), areaSqua);
    lightList.append(lightArea1);
}

void Scene::cornellScene()
{
    std::shared_ptr<Primitive> primSquaB = std::make_shared<Primitive>("backwall");
    std::shared_ptr<SquarePlane> squaB = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0,0,-5),Vector3f(0,0,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaR = std::make_shared<Primitive>("rightwall");
    std::shared_ptr<SquarePlane> squaR = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(5,0,0),Vector3f(0,-90,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaL = std::make_shared<Primitive>("leftwall");
    std::shared_ptr<SquarePlane> squaL = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(-5,0,0),Vector3f(0,90,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaU = std::make_shared<Primitive>("ceiling");
    std::shared_ptr<SquarePlane> squaU = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0,5,0),Vector3f(90,0,0),Vector3f(10,10,1)));

    std::shared_ptr<Primitive> primSquaD = std::make_shared<Primitive>("floor");
    std::shared_ptr<SquarePlane> squaD = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0,-5,0),Vector3f(-90,0,0),Vector3f(10,10,1)));


    std::shared_ptr<Primitive> primSphe1 = std::make_shared<Primitive>("sphere1");
    std::shared_ptr<Sphere> sphe1 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(-2,-5,0),Vector3f(0,0,0),Vector3f(5,5,5)));

    std::shared_ptr<Primitive> primSphe2 = std::make_shared<Primitive>("sphere2");
    std::shared_ptr<Sphere> sphe2 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(-3,0,2),Vector3f(0,0,0),Vector3f(4,4,4)));

    std::shared_ptr<Primitive> primSphe3 = std::make_shared<Primitive>("sphere3");
    std::shared_ptr<Sphere> sphe3 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(-3,0,2),Vector3f(0,0,0),Vector3f(2.5,2.5,2.5)));

    std::shared_ptr<Primitive> primSphe4 = std::make_shared<Primitive>("sphere4");
    std::shared_ptr<Sphere> sphe4 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(3,-2,-3),Vector3f(0,0,0),Vector3f(4,4,4)));

    std::shared_ptr<Primitive> primSphe5 = std::make_shared<Primitive>("sphere5");
    std::shared_ptr<Sphere> sphe5 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(3,-2,-3),Vector3f(0,0,0),Vector3f(1,1,1)));

    std::shared_ptr<Primitive> primSphe6 = std::make_shared<Primitive>("sphere6");
    std::shared_ptr<Sphere> sphe6 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(-3,3.5,-3),Vector3f(0,0,0),Vector3f(3,3,3)));

    std::shared_ptr<Primitive> primSphe7 = std::make_shared<Primitive>("sphere7");
    std::shared_ptr<Sphere> sphe7 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(3,3.5,-3),Vector3f(0,0,0),Vector3f(3,3,3)));

    std::shared_ptr<Primitive> primSphe8 = std::make_shared<Primitive>("sphere8");
    std::shared_ptr<Sphere> sphe8 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(3,3.5,-3),Vector3f(0,0,0),Vector3f(1,1,1)));

    std::shared_ptr<Primitive> primSphe9 = std::make_shared<Primitive>("sphere9");
    std::shared_ptr<Sphere> sphe9 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(0,0,0),Vector3f(0,0,0),Vector3f(2,2,2)));

    std::shared_ptr<LambertMaterial> matLamWhite = std::make_shared<LambertMaterial>("whitediff", Color3f(1,1,1));
    std::shared_ptr<LambertMaterial> matLamRed = std::make_shared<LambertMaterial>("reddiff", Color3f(1,0,0));
    std::shared_ptr<LambertMaterial> matLamGreen = std::make_shared<LambertMaterial>("greendiff", Color3f(0,1,0));
    std::shared_ptr<LambertMaterial> matLamBlue = std::make_shared<LambertMaterial>("bluediff", Color3f(0,0,1));
    std::shared_ptr<LambertMaterial> matLamYellow = std::make_shared<LambertMaterial>("yellowdiff", Color3f(1,1,0));

    std::shared_ptr<BlinnPhongMaterial> matBliPhoPurple = std::make_shared<BlinnPhongMaterial>("purpleSpec", Color3f(0.75,0,0.75), 10);

    std::shared_ptr<SpecularReflectionMaterial> matSpecRef = std::make_shared<SpecularReflectionMaterial>("specRef", Color3f(1,1,1), 10, 0.3);
    std::shared_ptr<SpecularReflectionMaterial> matSpecRef2 = std::make_shared<SpecularReflectionMaterial>("specRef", Color3f(0.25,0.5,1), 10, 0.3);

    std::shared_ptr<SpecularTransmissionMaterial> matSpecTrans = std::make_shared<SpecularTransmissionMaterial>("specTrans", Color3f(1,1,1), 10, 1, 0.7, 0.9);
    std::shared_ptr<SpecularTransmissionMaterial> matSpecTrans2 = std::make_shared<SpecularTransmissionMaterial>("specTrans", Color3f(0.15,1,0.15), 10, 1, 0.4, 0.6);
    std::shared_ptr<SpecularTransmissionMaterial> matSpecTrans3 = std::make_shared<SpecularTransmissionMaterial>("specTrans", Color3f(1,1,1), 10, 1, 0.9, 0.7);

    primSquaB->shape = squaB;
    primSquaR->shape = squaR;
    primSquaL->shape = squaL;
    primSquaU->shape = squaU;
    primSquaD->shape = squaD;

    primSquaB->mat = matLamWhite;
    primSquaR->mat = matLamGreen;
    primSquaL->mat = matLamRed;
    primSquaU->mat = matLamWhite;
    primSquaD->mat = matLamWhite;

    primSphe1->shape = sphe1;
    primSphe2->shape = sphe2;
    primSphe3->shape = sphe3;
    primSphe4->shape = sphe4;
    primSphe5->shape = sphe5;
    primSphe6->shape = sphe6;
    primSphe7->shape = sphe7;
    primSphe8->shape = sphe8;
    primSphe9->shape = sphe9;

    primSphe1->mat = matBliPhoPurple;
    primSphe2->mat = matSpecTrans;
    primSphe3->mat = matSpecTrans3;
    primSphe4->mat = matSpecTrans2;
    primSphe5->mat = matLamWhite;
    primSphe6->mat = matSpecRef2;
    primSphe7->mat = matSpecTrans2;
    primSphe8->mat = matSpecRef2;
    primSphe9->mat = matLamYellow;

    primitiveList.append(primSquaB);
    primitiveList.append(primSquaR);
    primitiveList.append(primSquaL);
    primitiveList.append(primSquaU);
    primitiveList.append(primSquaD);

    primitiveList.append(primSphe1);
    primitiveList.append(primSphe2);
    primitiveList.append(primSphe3);
    primitiveList.append(primSphe4);
    primitiveList.append(primSphe5);
    primitiveList.append(primSphe6);
    primitiveList.append(primSphe7);
    primitiveList.append(primSphe8);
    primitiveList.append(primSphe9);

    //light
    std::shared_ptr<PointLight> lightPoint1 = std::make_shared<PointLight>(Color3f(1,1,1), Point3f(0,4.9,0));
    lightList.append(lightPoint1);
}

void Scene::testScene()
{
    std::shared_ptr<Primitive> primSphe = std::make_shared<Primitive>("test");
    std::shared_ptr<Sphere> sphe = std::make_shared<Sphere>(0.5f,Transform(Vector3f(0,0,1),Vector3f(0,0,45),Vector3f(2,1,1)));
    std::shared_ptr<LambertMaterial> matLam1 = std::make_shared<LambertMaterial>("lam1", Color3f(0.8,0.3,0.5));

    std::shared_ptr<Primitive> primSphe2 = std::make_shared<Primitive>("test1");
    std::shared_ptr<Sphere> sphe2 = std::make_shared<Sphere>(0.5f,Transform(Vector3f(0,0,0),Vector3f(0,0,0),Vector3f(.5f,.5f,5.f)));
    std::shared_ptr<LambertMaterial> matLam2 = std::make_shared<LambertMaterial>("lam2", Color3f(0.3,0.8,0.5));

    std::shared_ptr<Primitive> primSqua = std::make_shared<Primitive>("test2");
    std::shared_ptr<SquarePlane> squa = std::make_shared<SquarePlane>(1.f,Transform(Vector3f(0,-0.5,0),Vector3f(-90,0,0),Vector3f(5,5,1)));
    std::shared_ptr<LambertMaterial> matLam3 = std::make_shared<LambertMaterial>("lam3", Color3f(0.3,0.5,0.8));

    primSphe->shape = sphe;
    primSphe2->shape = sphe2;
    primSqua->shape = squa;

    primSphe->mat = matLam1;
    primSphe2->mat = matLam2;
    primSqua->mat = matLam3;

    primitiveList.append(primSphe);
    primitiveList.append(primSphe2);
    primitiveList.append(primSqua);

    //light
    std::shared_ptr<PointLight> lightPoint1 = std::make_shared<PointLight>(Color3f(1,1,1), Point3f(-5,5,5));
    lightList.append(lightPoint1);
}

QImage Scene::renderScene()
{
    QImage result(camera.width,camera.height,QImage::Format_RGB32);
    result.fill(QColor(0,0,0));
    for(int i = 0;i<camera.width;++i)
    {
        for(int j = 0;j<camera.height;++j)
        {
            Ray r = camera.rayCast(Point2f(i,j));
            qDebug() << i <<","<<j;
            Intersection it;
            bool b = getIntersection(r, &it);
            if(b)
            {
                qDebug() << it.objectHit->name;
                glm::vec3 c = it.objectHit->mat->getScatteredColorEX(it, r, *this);
                c = glm::clamp(c, glm::vec3(0.f,0.f,0.f), glm::vec3(1.f,1.f,1.f));
                c = c * 255.f;
                result.setPixel(i, j, qRgb(c.r, c.g, c.b));
            }
        }
    }
    return result;
}
