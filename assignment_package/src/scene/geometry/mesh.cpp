#include <scene/geometry/mesh.h>
#include <la.h>
#include <tinyobj/tiny_obj_loader.h>
#include <iostream>

float TriArea(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3)
{
    return glm::length(glm::cross(p1 - p2, p3 - p2)) * 0.5f;
}

float Triangle::Area() const
{
    //TODO
    return TriArea(points[0],points[1],points[2]);
}

float Mesh::Area() const
{
    //TODO
    float sum = 0;
    for(auto item : faces)
    {
        sum += item->Area();
    }
    return sum;
}

Triangle::Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3):
    Triangle(p1, p2, p3, glm::vec3(1), glm::vec3(1), glm::vec3(1), glm::vec2(0), glm::vec2(0), glm::vec2(0))
{
    for(int i = 0; i < 3; i++)
    {
        normals[i] = planeNormal;
    }
}


Triangle::Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec3 &n1, const glm::vec3 &n2, const glm::vec3 &n3):
    Triangle(p1, p2, p3, n1, n2, n3, glm::vec2(0), glm::vec2(0), glm::vec2(0))
{}


Triangle::Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec3 &n1, const glm::vec3 &n2, const glm::vec3 &n3, const glm::vec2 &t1, const glm::vec2 &t2, const glm::vec2 &t3)
{
    planeNormal = glm::normalize(glm::cross(p2 - p1, p3 - p2));
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    normals[0] = n1;
    normals[1] = n2;
    normals[2] = n3;
    uvs[0] = t1;
    uvs[1] = t2;
    uvs[2] = t3;
}

//Returns the interpolation of the triangle's three normals based on the point inside the triangle that is given.
Normal3f Triangle::GetNormal(const Point3f &P) const
{
    float A = TriArea(points[0], points[1], points[2]);
    float A0 = TriArea(points[1], points[2], P);
    float A1 = TriArea(points[0], points[2], P);
    float A2 = TriArea(points[0], points[1], P);
    return glm::normalize(normals[0] * A0/A + normals[1] * A1/A + normals[2] * A2/A);
}

//my intersection
bool Triangle::Intersect(glm::vec3 origin, glm::vec3 direction, float* tResult)
{
    //1. Ray-plane intersection
    float t =  glm::dot(planeNormal, (points[0] - origin)) / glm::dot(planeNormal, direction);
    if(t < 0) return false;

    glm::vec3 P = origin + t * direction;
    //2. Barycentric test
    float S = 0.5f * glm::length(glm::cross(points[0] - points[1], points[0] - points[2]));
    float s1 = 0.5f * glm::length(glm::cross(P - points[1], P - points[2]))/S;
    float s2 = 0.5f * glm::length(glm::cross(P - points[2], P - points[0]))/S;
    float s3 = 0.5f * glm::length(glm::cross(P - points[0], P - points[1]))/S;
    float sum = s1 + s2 + s3;

    if(s1 >= 0 && s1 <= 1 && s2 >= 0 && s2 <= 1 && s3 >= 0 && s3 <= 1 && fequal(sum, 1.0f)){
        *tResult = t;
        return true;
    }
    return false;
}

//The ray in this function is not transformed because it was *already* transformed in Mesh::GetIntersection
bool Triangle::Intersect(const Ray& r, Intersection* isect)
{
    //1. Ray-plane intersection
    float t =  glm::dot(planeNormal, (points[0] - r.origin)) / glm::dot(planeNormal, r.direction);
    if(t < 0) return false;

    glm::vec3 P = r.origin + t * r.direction;
    //2. Barycentric test
    float S = 0.5f * glm::length(glm::cross(points[0] - points[1], points[0] - points[2]));
    float s1 = 0.5f * glm::length(glm::cross(P - points[1], P - points[2]))/S;
    float s2 = 0.5f * glm::length(glm::cross(P - points[2], P - points[0]))/S;
    float s3 = 0.5f * glm::length(glm::cross(P - points[0], P - points[1]))/S;
    float sum = s1 + s2 + s3;

    if(s1 >= 0 && s1 <= 1 && s2 >= 0 && s2 <= 1 && s3 >= 0 && s3 <= 1 && fequal(sum, 1.0f)){
        isect->t = t;
        return true;
    }
    return false;
}


bool Mesh::Intersect(const Ray& r, Intersection* isect)
{
    Ray r_loc = r.GetTransformedCopy(transform.invT());
    float closest_t = -1;
    Triangle* closestTri = nullptr;

    glm::vec3 o = r_loc.origin;
    glm::vec3 d = r_loc.direction;

#ifdef BVH_DEBUG
    qDebug() << "o" << o.x << "," << o.y << "," << o.z;
    qDebug() << "d" << d.x << "," << d.y << "," << d.z;
#endif

#ifdef BVH_DEBUG_LOCAL
    qDebug() << "o" << o.x << "," << o.y << "," << o.z;
    qDebug() << "d" << d.x << "," << d.y << "," << d.z;
#endif

    if(tree->intersect(o, d, &closest_t, &closestTri))
    {
        if(closestTri!=nullptr&&closest_t>0)//no need to do this additional check
        {
#ifdef BVH_DEBUG_LOCAL
            qDebug() << "hit";
#endif

#ifdef BVH_DEBUG
            qDebug() << "  hit";
            qDebug() << "   normal:" << closestTri->planeNormal.x << "," << closestTri->planeNormal.y << "," << closestTri->planeNormal.z;
            qDebug() << "   p1:" << closestTri->points[0].x << "," << closestTri->points[0].y << "," <<closestTri->points[0].z;
            qDebug() << "   p2:" << closestTri->points[1].x << "," << closestTri->points[1].y << "," <<closestTri->points[1].z;
            qDebug() << "   p3:" << closestTri->points[2].x << "," << closestTri->points[2].y << "," <<closestTri->points[2].z;
#endif

            //qDebug() << closest_t;

            Point3f P = Point3f(closest_t * r_loc.direction + r_loc.origin);
            closestTri->InitializeIntersection(isect, closest_t, P);
            this->InitializeIntersection(isect, closest_t, P);
            return true;
        }

#ifdef BVH_DEBUG_LOCAL
            qDebug() << "no hit";
#endif

#ifdef BVH_DEBUG
        qDebug()<<"  no hit";
#endif
    }
    return false;
}

//bool Mesh::Intersect(const Ray& r, Intersection* isect)
//{
//    Ray r_loc = r.GetTransformedCopy(transform.invT());
//    float closest_t = -1;
//    Triangle* closestTri = nullptr;

//    //qDebug() <<"tree->debug:"<< tree->debug.size() << "---faces:" << faces.size();
//    for(int i = 0; i < tree->debug.size(); i++){
//        Intersection isx;
//        if(tree->debug[i]->Intersect(r_loc.origin, r_loc.direction, &isx.t)){
//            if(isx.t > 0 && (isx.t < closest_t || closest_t < 0)){
//                closest_t = isx.t;
//                closestTri = tree->debug[i];
//            }
//        }
//    }
//    if(closest_t > 0)
//    {
//#ifdef BVH_DEBUG_LOCAL
//        qDebug() << "hit";
//#endif

//        //qDebug() << closest_t;

//        Point3f P = Point3f(closest_t * r_loc.direction + r_loc.origin);
//        closestTri->InitializeIntersection(isect, closest_t, P);
//        this->InitializeIntersection(isect, closest_t, P);
//        return true;
//    }

//#ifdef BVH_DEBUG_LOCAL
//    qDebug() << "no hit";
//#endif
//    return false;
//}

//bool Mesh::Intersect(const Ray& r, Intersection* isect)
//{
//    Ray r_loc = r.GetTransformedCopy(transform.invT());
//    float closest_t = -1;
//    Triangle* closestTri = nullptr;

//    for(int i = 0; i < faces.size(); i++){
//        Intersection isx;
//        if(faces[i]->Intersect(r_loc, &isx)){
//            if(isx.t > 0 && (isx.t < closest_t || closest_t < 0)){
//                closest_t = isx.t;
//                closestTri = faces[i];
//            }
//        }
//    }
//    if(closest_t > 0)
//    {
//#ifdef BVH_DEBUG_LOCAL
//        qDebug() << "hit";
//#endif

//        qDebug() << closest_t;

//        Point3f P = Point3f(closest_t * r_loc.direction + r_loc.origin);
//        closestTri->InitializeIntersection(isect, closest_t, P);
//        this->InitializeIntersection(isect, closest_t, P);
//        return true;
//    }

//#ifdef BVH_DEBUG_LOCAL
//    qDebug() << "no hit";
//#endif
//    return false;
//}

void Triangle::InitializeIntersection(Intersection *isect, float t, Point3f pLocal) const
{
    //isect->point = pLocal;
    isect->uv = GetUVCoordinates(pLocal);
    ComputeTriangleTBN(pLocal, &(isect->normalGeometric), &(isect->tangent), &(isect->bitangent), isect->uv);
    isect->t = t;
}

void Triangle::ComputeTBN(const Point3f &P, Normal3f *nor, Vector3f *tan, Vector3f *bit) const
{
    //Triangle uses ComputeTriangleTBN instead of this function.
}

void Triangle::ComputeTriangleTBN(const Point3f &P, Normal3f *nor, Vector3f *tan, Vector3f *bit, const Point2f &uv) const
{
    *nor = GetNormal(P);
    //TODO: Compute tangent and bitangent based on UV coordinates.

    //t and bit are same for every point on this triangle, why compute them every time??
    //because we are not storing them in every triangle
    //we are storing them only in the itersection and only temporarily
    glm::vec2 deltaUV1 = uvs[1] - uvs[0];
    glm::vec2 deltaUV2 = uvs[2] - uvs[0];
    glm::vec3 deltaP1 = points[1] - points[0];
    glm::vec3 deltaP2 = points[2] - points[0];

    glm::vec3 t = (deltaUV2.y * deltaP1 - deltaUV1.y * deltaP2) / (deltaUV2.y * deltaUV1.x - deltaUV1.y * deltaUV2.x);
    //(deltaUV2.y * deltaUV1.x - deltaUV1.y * deltaUV2.x) can't be 0
    //otherwise it will not be a triangle
    //but deltaUV2.y can
    //when deltaUV2.y is 0, deltaUV1.y can't be 0
    glm::vec3 b = deltaUV2.y==0 ? (deltaP1 - deltaUV1.x * t) / deltaUV1.y : (deltaP2 - deltaUV2.x * t) / deltaUV2.y;

    if(glm::length(t)==0) qDebug() << "fuck me";
    if(glm::length(b)==0) qDebug() << "fuck me again";
    *tan = glm::normalize(t);
    *bit = glm::normalize(b);

    //    qDebug() << "nor" << nor->x << "," << nor->y << "," << nor->z;
    //    qDebug() << "tan" << tan->x << "," << tan->y << "," << tan->z;
    //    qDebug() << "bit" << bit->x << "," << bit->y << "," << bit->z;
}


Mesh::Mesh()
{
    tree = new BVHKDTree();
}

Mesh::~Mesh()
{
    delete tree;
}

void Mesh::InitializeIntersection(Intersection *isect, float t, Point3f pLocal) const
{
    isect->point = Point3f(transform.T() * glm::vec4(pLocal, 1));
    ComputeTBN(pLocal, &(isect->normalGeometric), &(isect->tangent), &(isect->bitangent));
    //can't believe it
    //isect->uv = GetUVCoordinates(pLocal);
    //isect->t = t;
}

void Mesh::ComputeTBN(const Point3f &P, Normal3f *nor, Vector3f *tan, Vector3f *bit) const
{
    *nor = glm::normalize(transform.invTransT() * (*nor));
    //TODO: Transform nor, tan, and bit into world space
    *tan = glm::normalize(glm::vec3(transform.T() * glm::vec4(*tan, 0.f)));
    *bit = glm::normalize(glm::vec3(transform.T() * glm::vec4(*bit, 0.f)));
}


Point2f Mesh::GetUVCoordinates(const Point3f &point) const
{
    //useless...
    return glm::vec2();
}


Point2f Triangle::GetUVCoordinates(const Point3f &point) const
{
    float A = TriArea(points[0], points[1], points[2]);
    float A0 = TriArea(points[1], points[2], point);
    float A1 = TriArea(points[0], points[2], point);
    float A2 = TriArea(points[0], points[1], point);
    return uvs[0] * A0/A + uvs[1] * A1/A + uvs[2] * A2/A;
}

void Mesh::LoadOBJ(const QStringRef &filename, const QStringRef &local_path)
{
    QString filepath = local_path.toString(); filepath.append(filename);
    std::vector<tinyobj::shape_t> shapes; std::vector<tinyobj::material_t> materials;
    std::string errors = tinyobj::LoadObj(shapes, materials, filepath.toStdString().c_str());
    std::cout << errors << std::endl;
    if(errors.size() == 0)
    {
        //Read the information from the vector of shape_ts
        for(unsigned int i = 0; i < shapes.size(); i++)
        {
            std::vector<float> &positions = shapes[i].mesh.positions;
            std::vector<float> &normals = shapes[i].mesh.normals;
            std::vector<float> &uvs = shapes[i].mesh.texcoords;
            std::vector<unsigned int> &indices = shapes[i].mesh.indices;
            for(unsigned int j = 0; j < indices.size(); j += 3)
            {
                glm::vec3 p1(positions[indices[j]*3], positions[indices[j]*3+1], positions[indices[j]*3+2]);
                glm::vec3 p2(positions[indices[j+1]*3], positions[indices[j+1]*3+1], positions[indices[j+1]*3+2]);
                glm::vec3 p3(positions[indices[j+2]*3], positions[indices[j+2]*3+1], positions[indices[j+2]*3+2]);

                Triangle* t = new Triangle(p1, p2, p3);

                //me////////////////////////////////
                //                if(p1.x<minAABB.x) minAABB.x = p1.x;
                //                if(p1.y<minAABB.y) minAABB.y = p1.y;
                //                if(p1.z<minAABB.z) minAABB.z = p1.z;
                //                if(p1.x>maxAABB.x) maxAABB.x = p1.x;
                //                if(p1.y>maxAABB.y) maxAABB.y = p1.y;
                //                if(p1.z>maxAABB.z) maxAABB.z = p1.z;

                //                if(p2.x<minAABB.x) minAABB.x = p2.x;
                //                if(p2.y<minAABB.y) minAABB.y = p2.y;
                //                if(p2.z<minAABB.z) minAABB.z = p2.z;
                //                if(p2.x>maxAABB.x) maxAABB.x = p2.x;
                //                if(p2.y>maxAABB.y) maxAABB.y = p2.y;
                //                if(p2.z>maxAABB.z) maxAABB.z = p2.z;

                //                if(p3.x<minAABB.x) minAABB.x = p3.x;
                //                if(p3.y<minAABB.y) minAABB.y = p3.y;
                //                if(p3.z<minAABB.z) minAABB.z = p3.z;
                //                if(p3.x>maxAABB.x) maxAABB.x = p3.x;
                //                if(p3.y>maxAABB.y) maxAABB.y = p3.y;
                //                if(p3.z>maxAABB.z) maxAABB.z = p3.z;
                //me////////////////////////////////

                if(normals.size() > 0)
                {
                    glm::vec3 n1(normals[indices[j]*3], normals[indices[j]*3+1], normals[indices[j]*3+2]);
                    glm::vec3 n2(normals[indices[j+1]*3], normals[indices[j+1]*3+1], normals[indices[j+1]*3+2]);
                    glm::vec3 n3(normals[indices[j+2]*3], normals[indices[j+2]*3+1], normals[indices[j+2]*3+2]);
                    t->normals[0] = n1;
                    t->normals[1] = n2;
                    t->normals[2] = n3;
                }
                if(uvs.size() > 0)
                {
                    glm::vec2 t1(uvs[indices[j]*2], uvs[indices[j]*2+1]);
                    glm::vec2 t2(uvs[indices[j+1]*2], uvs[indices[j+1]*2+1]);
                    glm::vec2 t3(uvs[indices[j+2]*2], uvs[indices[j+2]*2+1]);
                    t->uvs[0] = t1;
                    t->uvs[1] = t2;
                    t->uvs[2] = t3;
                }
                this->faces.push_back(t);
            }
        }
        //TODO: .mtl file loading
    }
    else
    {
        //An error loading the OBJ occurred!
        std::cout << errors << std::endl;
    }

    //mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
    tree->build(faces);
    //mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
}

Intersection Mesh::Sample(const Point2f &xi, Float *pdf) const
{
    //modify here if want mesh light
    return Intersection();
}

Intersection Triangle::Sample(const Point2f &xi, Float *pdf) const
{
    //modify here if want mesh light
    return Intersection();
}
