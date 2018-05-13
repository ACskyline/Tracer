#include <scene/jsonreader.h>
#include <scene/geometry/cube.h>
#include <scene/geometry/sphere.h>
#include <scene/geometry/mesh.h>
#include <scene/geometry/disc.h>
#include <scene/geometry/squareplane.h>
#include <scene/geometry/implicitsurface.h>
#include <scene/materials/mattematerial.h>
#include <scene/materials/mirrormaterial.h>
#include <scene/materials/transmissivematerial.h>
#include <scene/materials/glassmaterial.h>
#include <scene/materials/plasticmaterial.h>
#include <scene/lights/diffusearealight.h>
#include <iostream>

#include "lenscamera.h"
#include "scene/lights/directionallight.h"
#include "scene/lights/environmentlight.h"

void JSONReader::LoadSceneFromFile(QFile &file, const QStringRef &local_path, Scene &scene)
{
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray rawData = file.readAll();
        // Parse document
        QJsonDocument doc(QJsonDocument::fromJson(rawData));

        // Get JSON object
        QJsonObject json = doc.object();
        QJsonObject sceneObj, camera;
        QJsonArray primitiveList, materialList, lightList, csgnodeList;

        QMap<QString, std::shared_ptr<Material>> mtl_name_to_material;
        QMap<QString, std::shared_ptr<ConstructiveNode>> csg_name_to_node;
        QJsonArray frames = json["frames"].toArray();
        //check scene object for every frame
        foreach(const QJsonValue &frame, frames) {
            QJsonObject sceneObj = frame.toObject()["scene"].toObject();
            //load camera
            if(sceneObj.contains(QString("camera"))) {
                camera = sceneObj["camera"].toObject();
                scene.SetCamera(LoadCamera(camera));
            }
            //load all materials in QMap with mtl name as key and Material itself as value
            if(sceneObj.contains(QString("materials"))){
                materialList = sceneObj["materials"].toArray();
                foreach(const QJsonValue &materialVal, materialList){
                    QJsonObject materialObj = materialVal.toObject();
                    LoadMaterial(materialObj, local_path, &mtl_name_to_material);
                }
            }
            //me
            if(sceneObj.contains(QString("csgnodes"))) {
                csgnodeList = sceneObj["csgnodes"].toArray();
                foreach(const QJsonValue &csgnodeVal, csgnodeList){
                    QJsonObject csgnodeObj = csgnodeVal.toObject();
                    LoadCsgNode(csgnodeObj, local_path, &csg_name_to_node);
                }
            }
            //load primitives and attach materials from QMap
            if(sceneObj.contains(QString("primitives"))) {
                primitiveList = sceneObj["primitives"].toArray();
                foreach(const QJsonValue &primitiveVal, primitiveList){
                    QJsonObject primitiveObj = primitiveVal.toObject();
                    LoadGeometry(primitiveObj, mtl_name_to_material, csg_name_to_node, local_path, &scene.primitives);
                }
            }

            //load lights and attach materials from QMap
            if(sceneObj.contains(QString("lights"))) {
                lightList = sceneObj["lights"].toArray();
                foreach(const QJsonValue &lightVal, lightList){
                    QJsonObject lightObj = lightVal.toObject();
                    LoadLights(lightObj, mtl_name_to_material, local_path, &scene.primitives, &scene.lights);
                }
            }
        }

        for(std::shared_ptr<Primitive> p : scene.primitives)
        {
//            //qDebug() << p->name;
//            if(auto csp = std::dynamic_pointer_cast<ConstructivePrimitive>(p))
//            {
//                if(csp->root)
//                {
//                    //qDebug() << "type" << csp->root->type;
//                    if(csp->root->left)
//                    {
//                        //qDebug() << "left type" << csp->root->left->type;
//                    }
//                    if(csp->root->right)
//                    {
//                        //qDebug() << "right type" << csp->root->right->type;
//                    }
//                }
//                else
//                {
//                    //qDebug() << "no root";
//                }
//            }
            p->shape->create();
        }
        file.close();
    }
}

bool JSONReader::LoadCsgNode(QJsonObject &csgnode, const QStringRef &local_path, QMap<QString, std::shared_ptr<ConstructiveNode> > *node_map)
{
    std::shared_ptr<Shape> shape = nullptr;
    //First check what type of geometry we're supposed to load
    QString type;
    if(csgnode.contains(QString("shape"))){
        type = csgnode["shape"].toString();
    }

    if(QString::compare(type, QString("Mesh")) == 0)
    {
        shape = std::make_shared<Mesh>();
        if(csgnode.contains(QString("filename"))) {
            QString objFilePath = csgnode["filename"].toString();
            std::static_pointer_cast<Mesh>(shape)->LoadOBJ(QStringRef(&objFilePath), local_path);
        }
    }
    else if(QString::compare(type, QString("Sphere")) == 0)
    {
        shape = std::make_shared<Sphere>();
    }
    else if(QString::compare(type, QString("SquarePlane")) == 0)
    {
        shape = std::make_shared<SquarePlane>();
    }
    else if(QString::compare(type, QString("Cube")) == 0)
    {
        shape = std::make_shared<Cube>();
    }
    else if(QString::compare(type, QString("Disc")) == 0)
    {
        shape = std::make_shared<Disc>();
    }
    else if(QString::compare(type, QString("ImplicitSurface")) == 0)
    {
        shape = std::make_shared<ImplicitSurface>();
        if(csgnode.contains(QString("SurfaceType"))) {
            std::static_pointer_cast<ImplicitSurface>(shape)->setSurfaceType(csgnode["SurfaceType"].toInt());
        }
    }
//    else
//    {
//        std::cout << "Could not parse the geometry!" << std::endl;
//        return NULL;
//    }

    //load transform to shape
    if(csgnode.contains(QString("transform"))) {
        QJsonObject transform = csgnode["transform"].toObject();
        if(shape.get()!=nullptr)
            shape->transform = LoadTransform(transform);
    }

    //create node
    std::shared_ptr<ConstructiveNode> result = std::make_shared<ConstructiveNode>();

    result->shape = shape;

    //type
    if(csgnode.contains(QString("type"))) {
        int nodeType = csgnode["type"].toInt();
        result->type = nodeType;
    }

    //left child
    if(csgnode.contains(QString("leftchild"))) {
        QString lcStr = csgnode["leftchild"].toString();
        for (auto i = node_map->begin(); i != node_map->end(); ++i) {
            //qDebug() << i.key() << "==" << lcStr << "?";
            if(i.key() == lcStr){
                //qDebug() << "leftC:" << i.key();
                result->left = i.value();
            }
        }
    }

    //right child
    if(csgnode.contains(QString("rightchild"))) {
        QString rcStr = csgnode["rightchild"].toString();
        for (auto i = node_map->begin(); i != node_map->end(); ++i) {
            //qDebug() << i.key() << "==" << rcStr << "?";
            if(i.key() == rcStr){
                //qDebug() << "rightC:" << i.key();
                result->right = i.value();
            }
        }
    }

    QString csgnodeName;

    if(csgnode.contains(QString("name")))
    {
        csgnodeName = csgnode["name"].toString();
        //qDebug() << csgnodeName;
        node_map->insert(csgnodeName, result);
    }
    return true;
}

bool JSONReader::LoadGeometry(QJsonObject &geometry, QMap<QString, std::shared_ptr<Material>> mtl_map, QMap<QString, std::shared_ptr<ConstructiveNode>> node_map, const QStringRef &local_path, QList<std::shared_ptr<Primitive>> *primitives)
{
    std::shared_ptr<Shape> shape = nullptr;
    //First check what type of geometry we're supposed to load
    QString type;
    if(geometry.contains(QString("shape"))){
        type = geometry["shape"].toString();
    }

    if(QString::compare(type, QString("Mesh")) == 0)
    {
        shape = std::make_shared<Mesh>();
        if(geometry.contains(QString("filename"))) {
            QString objFilePath = geometry["filename"].toString();
            std::static_pointer_cast<Mesh>(shape)->LoadOBJ(QStringRef(&objFilePath), local_path);
        }
    }
    else if(QString::compare(type, QString("Sphere")) == 0)
    {
        shape = std::make_shared<Sphere>();
    }
    else if(QString::compare(type, QString("SquarePlane")) == 0)
    {
        shape = std::make_shared<SquarePlane>();
    }
    else if(QString::compare(type, QString("Cube")) == 0)
    {
        shape = std::make_shared<Cube>();
    }
    else if(QString::compare(type, QString("Disc")) == 0)
    {
        shape = std::make_shared<Disc>();
    }
    else if(QString::compare(type, QString("ImplicitSurface")) == 0)
    {
        shape = std::make_shared<ImplicitSurface>();
        if(geometry.contains(QString("SurfaceType"))) {
            std::static_pointer_cast<ImplicitSurface>(shape)->setSurfaceType(geometry["SurfaceType"].toInt());
        }
    }
    else
    {
        std::cout << "Could not parse the geometry!" << std::endl;
        return NULL;
    }

    //load transform to shape
    if(geometry.contains(QString("transform"))) {
        QJsonObject transform = geometry["transform"].toObject();
        shape->transform = LoadTransform(transform);
    }

    std::shared_ptr<Primitive> primitive = std::make_shared<Primitive>();

    if(geometry.contains(QString("CsgNode"))){
        QString nodeName = geometry["CsgNode"].toString();
        std::shared_ptr<ConstructivePrimitive> csgPrimitive = std::make_shared<ConstructivePrimitive>();
        //qDebug() << node_map.size();
        for(auto i = node_map.begin();i!=node_map.end();i++)
        {
            //qDebug() << i.key();
            if(i.key()==nodeName)
                csgPrimitive->root = i.value();
        }
        primitive = std::static_pointer_cast<Primitive>(csgPrimitive);
    }

    primitive->shape = shape;

    QMap<QString, std::shared_ptr<Material>>::iterator i;
    if(geometry.contains(QString("material"))) {
        QString material_name = geometry["material"].toString();
        for (i = mtl_map.begin(); i != mtl_map.end(); ++i) {
            if(i.key() == material_name){
                primitive->material = i.value();
            }
        }
    }

    if(geometry.contains(QString("name"))) primitive->name = geometry["name"].toString();

    (*primitives).append(primitive);
    return true;
}

bool JSONReader::LoadLights(QJsonObject &geometry, QMap<QString, std::shared_ptr<Material>> mtl_map, const QStringRef &local_path, QList<std::shared_ptr<Primitive>> *primitives, QList<std::shared_ptr<Light>> *lights)
{
    std::shared_ptr<Shape> shape = nullptr;
    //First check what type of geometry we're supposed to load
    QString type;
    if(geometry.contains(QString("shape"))){
        type = geometry["shape"].toString();
    }

    if(QString::compare(type, QString("Mesh")) == 0)
    {
        shape = std::make_shared<Mesh>();
        if(geometry.contains(QString("filename"))) {
            QString objFilePath = geometry["filename"].toString();
            std::static_pointer_cast<Mesh>(shape)->LoadOBJ(QStringRef(&objFilePath), local_path);
        }
    }
    else if(QString::compare(type, QString("Sphere")) == 0)
    {
        shape = std::make_shared<Sphere>();
    }
    else if(QString::compare(type, QString("SquarePlane")) == 0)
    {
        shape = std::make_shared<SquarePlane>();
    }
    else if(QString::compare(type, QString("Cube")) == 0)
    {
        shape = std::make_shared<Cube>();
    }
    else if(QString::compare(type, QString("Disc")) == 0)
    {
        shape = std::make_shared<Disc>();
    }
    else
    {
        shape = std::make_shared<SquarePlane>();//create whatever you want just to use the transform and keep codes tidy
//        std::cout << "Could not parse the geometry!" << std::endl;
//        return NULL;
    }

    //load transform to shape
    if(geometry.contains(QString("transform"))) {
        QJsonObject transform = geometry["transform"].toObject();
        shape->transform = LoadTransform(transform);
    }

    //load light type
    std::shared_ptr<Light> lightType = nullptr;
    QString lgtType;
    if(geometry.contains(QString("type"))){
        lgtType = geometry["type"].toString();
    }

    if(QString::compare(lgtType, QString("DiffuseAreaLight")) == 0)
    {
        Color3f lightColor = ToVec3(geometry["lightColor"].toArray());
        Float intensity = static_cast< float >(geometry["intensity"].toDouble());
        bool twoSided = geometry.contains(QString("twoSided")) ? geometry["twoSided"].toBool() : false;
        lightType = std::make_shared<DiffuseAreaLight>(shape->transform, lightColor * intensity, shape, twoSided);

        auto primitive = std::make_shared<Primitive>(shape, nullptr,  std::static_pointer_cast<AreaLight>(lightType));
        QMap<QString, std::shared_ptr<Material>>::iterator i;
        if(geometry.contains(QString("material"))) {
            QString material_name = geometry["material"].toString();
            for (i = mtl_map.begin(); i != mtl_map.end(); ++i) {
                if(i.key() == material_name){
                    primitive->material = i.value();
                }
            }
        }

        primitive->name = geometry["name"].toString();
        (*primitives).append(primitive);
    }
    else if(QString::compare(lgtType, QString("DirectionalLight")) == 0)
    {
        Color3f lightColor = ToVec3(geometry["lightColor"].toArray());
        Float intensity = static_cast< float >(geometry["intensity"].toDouble());
        Vector3f direction = ToVec3(geometry["direction"].toArray());
        lightType = std::make_shared<DirectionalLight>(shape->transform, lightColor * intensity, direction);
    }
    else if(QString::compare(lgtType, QString("EnvironmentLight")) == 0)
    {
        Color3f lightColor = ToVec3(geometry["lightColor"].toArray());
        Float intensity = static_cast< float >(geometry["intensity"].toDouble());
        std::shared_ptr<QImage> textureMap;
        if(geometry.contains(QString("textureMap"))) {
            QString img_filepath = local_path.toString().append(geometry["textureMap"].toString());
            textureMap = std::make_shared<QImage>(img_filepath);
        }
        lightType = std::make_shared<EnvironmentLight>(shape->transform, textureMap, lightColor * intensity);
    }
    else
    {
        std::cout << "Could not parse the geometry!" << std::endl;
        return NULL;
    }




    if(geometry.contains(QString("name")))
    {
        lightType->name = geometry["name"].toString();
    }
    (*lights).append(lightType);
    return true;
}

bool JSONReader::LoadMaterial(QJsonObject &material, const QStringRef &local_path, QMap<QString, std::shared_ptr<Material> > *mtl_map)
{
    QString type;

    //First check what type of material we're supposed to load
    if(material.contains(QString("type"))) type = material["type"].toString();

    if(QString::compare(type, QString("MatteMaterial")) == 0)
    {
        std::shared_ptr<QImage> textureMap;
        std::shared_ptr<QImage> normalMap;
        Color3f Kd = ToVec3(material["Kd"].toArray());
        Float sigma = static_cast< float >(material["sigma"].toDouble());
        if(material.contains(QString("textureMap"))) {
            QString img_filepath = local_path.toString().append(material["textureMap"].toString());
            textureMap = std::make_shared<QImage>(img_filepath);
        }
        if(material.contains(QString("normalMap"))) {
            QString img_filepath = local_path.toString().append(material["normalMap"].toString());
            normalMap = std::make_shared<QImage>(img_filepath);
        }
        auto result = std::make_shared<MatteMaterial>(Kd, sigma, textureMap, normalMap);
        mtl_map->insert(material["name"].toString(), result);
    }
    else if(QString::compare(type, QString("MirrorMaterial")) == 0)
    {
        std::shared_ptr<QImage> roughnessMap;
        std::shared_ptr<QImage> textureMap;
        std::shared_ptr<QImage> normalMap;
        Color3f Kr = ToVec3(material["Kr"].toArray());
        float roughness = 0.f;
        if(material.contains(QString("roughness"))) {
            roughness = material["roughness"].toDouble();
        }
        if(material.contains(QString("roughnessMap"))) {
            QString img_filepath = local_path.toString().append(material["roughnessMap"].toString());
            roughnessMap = std::make_shared<QImage>(img_filepath);
        }
        if(material.contains(QString("textureMap"))) {
            QString img_filepath = local_path.toString().append(material["textureMap"].toString());
            textureMap = std::make_shared<QImage>(img_filepath);
        }
        if(material.contains(QString("normalMap"))) {
            QString img_filepath = local_path.toString().append(material["normalMap"].toString());
            normalMap = std::make_shared<QImage>(img_filepath);
        }
        auto result = std::make_shared<MirrorMaterial>(Kr, roughness, roughnessMap, textureMap, normalMap);
        mtl_map->insert(material["name"].toString(), result);
    }
    else if(QString::compare(type, QString("TransmissiveMaterial")) == 0)
    {
        std::shared_ptr<QImage> textureMap;
        std::shared_ptr<QImage> normalMap;
        Color3f Kt = ToVec3(material["Kt"].toArray());
        float eta = material["eta"].toDouble();
        if(material.contains(QString("textureMap"))) {
            QString img_filepath = local_path.toString().append(material["textureMap"].toString());
            textureMap = std::make_shared<QImage>(img_filepath);
        }
        if(material.contains(QString("normalMap"))) {
            QString img_filepath = local_path.toString().append(material["normalMap"].toString());
            normalMap = std::make_shared<QImage>(img_filepath);
        }
        auto result = std::make_shared<TransmissiveMaterial>(Kt, eta, textureMap, normalMap);
        mtl_map->insert(material["name"].toString(), result);
    }
    else if(QString::compare(type, QString("GlassMaterial")) == 0)
    {
        std::shared_ptr<QImage> textureMapRefl;
        std::shared_ptr<QImage> textureMapTransmit;
        std::shared_ptr<QImage> normalMap;
        Color3f Kr = ToVec3(material["Kr"].toArray());
        Color3f Kt = ToVec3(material["Kt"].toArray());
        float eta = material["eta"].toDouble();
        if(material.contains(QString("textureMapRefl"))) {
            QString img_filepath = local_path.toString().append(material["textureMapRefl"].toString());
            textureMapRefl = std::make_shared<QImage>(img_filepath);
        }
        if(material.contains(QString("textureMapTransmit"))) {
            QString img_filepath = local_path.toString().append(material["textureMapTransmit"].toString());
            textureMapTransmit = std::make_shared<QImage>(img_filepath);
        }
        if(material.contains(QString("normalMap"))) {
            QString img_filepath = local_path.toString().append(material["normalMap"].toString());
            normalMap = std::make_shared<QImage>(img_filepath);
        }
        auto result = std::make_shared<GlassMaterial>(Kr, Kt, eta, textureMapRefl, textureMapTransmit, normalMap);
        mtl_map->insert(material["name"].toString(), result);
    }
    else if(QString::compare(type, QString("PlasticMaterial")) == 0)
    {
        std::shared_ptr<QImage> roughnessMap;
        std::shared_ptr<QImage> textureMapDiffuse;
        std::shared_ptr<QImage> textureMapSpecular;
        std::shared_ptr<QImage> normalMap;
        Color3f Kd = ToVec3(material["Kd"].toArray());
        Color3f Ks = ToVec3(material["Ks"].toArray());
        float roughness = material["roughness"].toDouble();
        if(material.contains(QString("roughnessMap"))) {
            QString img_filepath = local_path.toString().append(material["roughnessMap"].toString());
            roughnessMap = std::make_shared<QImage>(img_filepath);
        }
        if(material.contains(QString("textureMapDiffuse"))) {
            QString img_filepath = local_path.toString().append(material["textureMapDiffuse"].toString());
            textureMapDiffuse = std::make_shared<QImage>(img_filepath);
        }
        if(material.contains(QString("textureMapSpecular"))) {
            QString img_filepath = local_path.toString().append(material["textureMapSpecular"].toString());
            textureMapSpecular = std::make_shared<QImage>(img_filepath);
        }
        if(material.contains(QString("normalMap"))) {
            QString img_filepath = local_path.toString().append(material["normalMap"].toString());
            normalMap = std::make_shared<QImage>(img_filepath);
        }
        auto result = std::make_shared<PlasticMaterial>(Kd, Ks, roughness, roughnessMap, textureMapDiffuse, textureMapSpecular, normalMap);
        mtl_map->insert(material["name"].toString(), result);
    }
    else
    {
        std::cout << "Could not parse the material!" << std::endl;
        return false;
    }

    return true;
}

std::shared_ptr<Camera> JSONReader::LoadCamera(QJsonObject& camera)
{
    std::shared_ptr<Camera> result;
    if(camera.contains(QString("lensRadius"))&&camera.contains(QString("focalDistance")))
    {
        float lensRadius = camera["lensRadius"].toDouble();
        float focalDistance = camera["focalDistance"].toDouble();
        result = std::static_pointer_cast<Camera>(std::make_shared<LensCamera>(lensRadius, focalDistance));
    }
    else
    {
        result = std::make_shared<Camera>();
    }

    if(camera.contains(QString("target"))) result->ref = ToVec3(camera["target"].toArray());
    if(camera.contains(QString("eye"))) result->eye = ToVec3(camera["eye"].toArray());
    if(camera.contains(QString("worldUp"))) result->world_up = ToVec3(camera["worldUp"].toArray());
    if(camera.contains(QString("width"))) result->width = camera["width"].toDouble();
    if(camera.contains(QString("height"))) result->height = camera["height"].toDouble();
    if(camera.contains(QString("fov"))) result->fovy = camera["fov"].toDouble();
    if(camera.contains(QString("nearClip"))) result->near_clip = camera["nearClip"].toDouble();
    if(camera.contains(QString("farClip"))) result->far_clip = camera["farClip"].toDouble();

    result->RecomputeAttributes();
    return result;
}

Transform JSONReader::LoadTransform(QJsonObject &transform)
{
    Vector3f t, r, s;
    s = Vector3f(1,1,1);
    if(transform.contains(QString("translate"))) t = ToVec3(transform["translate"].toArray());
    if(transform.contains(QString("rotate"))) r = ToVec3(transform["rotate"].toArray());
    if(transform.contains(QString("scale"))) s = ToVec3(transform["scale"].toArray());
    return Transform(t, r, s);
}

glm::vec3 JSONReader::ToVec3(const QJsonArray &s)
{
    glm::vec3 result(s.at(0).toDouble(), s.at(1).toDouble(), s.at(2).toDouble());
    return result;
}

glm::vec3 JSONReader::ToVec3(const QStringRef &s)
{
    glm::vec3 result;
    int start_idx;
    int end_idx = -1;
    for(int i = 0; i < 3; i++){
        start_idx = ++end_idx;
        while(end_idx < s.length() && s.at(end_idx) != QChar(' '))
        {
            end_idx++;
        }
        result[i] = s.mid(start_idx, end_idx - start_idx).toFloat();
    }
    return result;
}

