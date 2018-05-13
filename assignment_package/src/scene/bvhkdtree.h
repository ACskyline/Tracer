#ifndef BVHKDTREE_H
#define BVHKDTREE_H

#include "geometry/mesh.h"


class Triangle;

class BVHKDNode
{
public:
    BVHKDNode();
    ~BVHKDNode();

    BVHKDNode* leftChild;
    BVHKDNode* rightChild;
    unsigned int axis;
    glm::vec3 minCorner, maxCorner;
    std::vector<Triangle*> triangles;
};

class BVHKDTree
{
private:
    void getMinMaxCorner(Triangle* t, glm::vec3* min, glm::vec3* max);
    void getMinMaxCorner(std::vector<Triangle*> &triangles, glm::vec3* min, glm::vec3* max);
    bool intersectTraverse(BVHKDNode* n, glm::vec3 origin, glm::vec3 direction, float *t, Triangle **tri);
    bool boxRayIntersection(glm::vec3 origin, glm::vec3 direction, glm::vec3 maxAABB, glm::vec3 minAABB);
    BVHKDNode* buildTree(std::vector<Triangle*>& triangles, int parentAxis);
public:
    BVHKDTree();
    ~BVHKDTree();
    void build(std::vector<Triangle*> &triangles);
    void clear();

    bool intersect(glm::vec3 origin, glm::vec3 direction, float *t, Triangle **tri);

    BVHKDNode* root;
    glm::vec3 minCorner, maxCorner;
    std::vector<Triangle*> debug;
};

#endif // BVHKDTREE_H
