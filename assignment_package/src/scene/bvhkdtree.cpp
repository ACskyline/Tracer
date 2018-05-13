#include "bvhkdtree.h"

BVHKDNode::BVHKDNode() : leftChild(nullptr), rightChild(nullptr), axis(0)
{

}

BVHKDNode::~BVHKDNode()
{
    delete leftChild;
    delete rightChild;
}

BVHKDTree::BVHKDTree() : root(nullptr)
{

}

BVHKDTree::~BVHKDTree()
{
    delete root;
}

bool xSort(Triangle* a, Triangle* b) { return a->points[0].x < b->points[0].x; }
bool ySort(Triangle* a, Triangle* b) { return a->points[0].y < b->points[0].y; }
bool zSort(Triangle* a, Triangle* b) { return a->points[0].z < b->points[0].z; }

void BVHKDTree::build(std::vector<Triangle*> &triangles)
{
    //TODO
    int size = triangles.size();

    if(size<=0) return;

    std::sort(triangles.begin(),triangles.end(),xSort);
    int median = size / 2;

    std::vector<Triangle*> left;
    for(int i = 0;i<median;i++)
    {
        left.push_back(triangles.at(i));
    }
    std::vector<Triangle*> right;
    for(int i = median;i<size;i++)
    {
        right.push_back(triangles.at(i));
    }

    root = new BVHKDNode();
    root->axis = 0;
    getMinMaxCorner(triangles, &root->minCorner, &root->maxCorner);
    root->leftChild = buildTree(left, root->axis);
    root->rightChild = buildTree(right, root->axis);
    minCorner = root->minCorner;
    maxCorner = root->maxCorner;
}


BVHKDNode* BVHKDTree::buildTree(std::vector<Triangle*>& triangles, int parentAxis)
{
    int size = triangles.size();

    if(size<=0) return nullptr;

    int median = size / 2;
    BVHKDNode* currentNode = new BVHKDNode();

    currentNode->axis = (parentAxis+1)%3;
    if(size==1)
    {
        getMinMaxCorner(triangles[0], &currentNode->minCorner, &currentNode->maxCorner);
        currentNode->triangles.push_back(triangles[0]);
        debug.push_back(triangles[0]);
    }
    else
    {
        getMinMaxCorner(triangles, &currentNode->minCorner, &currentNode->maxCorner);
        switch(parentAxis)
        {
        case 0:
            std::sort(triangles.begin(),triangles.end(),ySort);
            break;
        case 1:
            std::sort(triangles.begin(),triangles.end(),zSort);
            break;
        case 2:
            std::sort(triangles.begin(),triangles.end(),xSort);
            break;
        }
        std::vector<Triangle*> left(triangles.begin(), triangles.begin()+median);
        std::vector<Triangle*> right(triangles.begin()+median, triangles.end());
        currentNode->leftChild = buildTree(left, currentNode->axis);
        currentNode->rightChild = buildTree(right, currentNode->axis);
    }
    return currentNode;
}

void BVHKDTree::getMinMaxCorner(Triangle* t, glm::vec3* min, glm::vec3* max)
{
    float minX = t->points[0].x;
    float minY = t->points[0].y;
    float minZ = t->points[0].z;

    float maxX = t->points[0].x;
    float maxY = t->points[0].y;
    float maxZ = t->points[0].z;

    for(int i = 1;i<3;i++)
    {
        if(t->points[i].x < minX) minX = t->points[i].x;
        if(t->points[i].y < minY) minY = t->points[i].y;
        if(t->points[i].z < minZ) minZ = t->points[i].z;

        if(t->points[i].x > maxX) maxX = t->points[i].x;
        if(t->points[i].y > maxY) maxY = t->points[i].y;
        if(t->points[i].z > maxZ) maxZ = t->points[i].z;
    }

    *min = glm::vec3(minX, minY, minZ);
    *max = glm::vec3(maxX, maxY, maxZ);
}


void BVHKDTree::getMinMaxCorner(std::vector<Triangle*> &triangles, glm::vec3* min, glm::vec3* max)
{
    if(triangles.size()<=0)
        return;

    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();
    float maxZ = std::numeric_limits<float>::min();

    for(auto item : triangles)
    {
        for(int i = 0;i<3;i++)
        {
            if(item->points[i].x < minX) minX = item->points[i].x;
            if(item->points[i].y < minY) minY = item->points[i].y;
            if(item->points[i].z < minZ) minZ = item->points[i].z;

            if(item->points[i].x > maxX) maxX = item->points[i].x;
            if(item->points[i].y > maxY) maxY = item->points[i].y;
            if(item->points[i].z > maxZ) maxZ = item->points[i].z;
        }
    }

    *min = glm::vec3(minX, minY, minZ);
    *max = glm::vec3(maxX, maxY, maxZ);
}

void BVHKDTree::clear()
{
    delete root;
    root = nullptr;
}

bool BVHKDTree::intersect(glm::vec3 origin, glm::vec3 direction, float *t, Triangle **tri)
{
    return intersectTraverse(root, origin, direction, t, tri);
}

bool BVHKDTree::intersectTraverse(BVHKDNode *n, glm::vec3 origin, glm::vec3 direction, float *t, Triangle **tri)
{
    if(n==nullptr)
        return false;

    if(boxRayIntersection(origin, direction, n->maxCorner, n->minCorner))
    {
        bool result = false;
        bool resultTemp =  false;

        resultTemp = intersectTraverse(n->leftChild, origin, direction, t, tri);
        result = result || resultTemp;

        resultTemp = intersectTraverse(n->rightChild, origin, direction, t, tri);
        result = result || resultTemp;

        for(auto item : n->triangles)
        {
            float tempT = -1;
            if(item->Intersect(origin, direction, &tempT))
            {
                if(tempT>0 && (tempT<*t || *t<0))
                {
                    *t = tempT;
                    *tri = item;
                    result = result || true;
                }
            }
        }

        return result;
    }
    return false;
}

bool BVHKDTree::boxRayIntersection(glm::vec3 origin, glm::vec3 direction, glm::vec3 maxAABB, glm::vec3 minAABB)
{
#ifdef BVH_DEBUG
    qDebug() << " origin" << origin.x << "," << origin.y << "," << origin.z;
    qDebug() << " direction" << direction.x << "," << direction.y << "," << direction.z;
    qDebug() << " maxAABB" << maxAABB.x << "," << maxAABB.y << "," << maxAABB.z;
    qDebug() << " minAABB" << minAABB.x << "," << minAABB.y << "," << minAABB.z;
#endif

    if(origin.x<maxAABB.x && origin.y<maxAABB.y && origin.z<maxAABB.z &&
            origin.x>minAABB.x && origin.y>minAABB.y && origin.z>minAABB.z)
        return true;

    if(direction.x < 0)
    {
        glm::vec3 normal(1,0,0);
        float t = glm::dot(maxAABB - origin, normal) / glm::dot(direction , normal);
        if(t>0)
        {
            glm::vec3 p = origin + direction * t;
            if(p.y>=minAABB.y&&p.y<=maxAABB.y&&p.z>=minAABB.z&&p.z<=maxAABB.z)
            {
#ifdef BVH_DEBUG
                qDebug() << " BoxHit";
#endif
                return true;
            }
        }
    }
    else if(direction.x > 0)
    {
        glm::vec3 normal(-1,0,0);
        float t = glm::dot(minAABB - origin, normal) / glm::dot(direction , normal);
        if(t>0)
        {
            glm::vec3 p = origin + direction * t;
            if(p.y>=minAABB.y&&p.y<=maxAABB.y&&p.z>=minAABB.z&&p.z<=maxAABB.z)
            {
#ifdef BVH_DEBUG
                qDebug() << " BoxHit";
#endif
                return true;
            }
        }
    }

    if(direction.y < 0)
    {
        glm::vec3 normal(0,1,0);
        float t = glm::dot(maxAABB - origin, normal) / glm::dot(direction , normal);
        if(t>0)
        {
            glm::vec3 p = origin + direction * t;
            if(p.x>=minAABB.x&&p.x<=maxAABB.x&&p.z>=minAABB.z&&p.z<=maxAABB.z)
            {
#ifdef BVH_DEBUG
                qDebug() << " BoxHit";
#endif
                return true;

            }
        }

    }
    else if(direction.y > 0)
    {
        glm::vec3 normal(0,-1,0);
        float t = glm::dot(minAABB - origin, normal) / glm::dot(direction , normal);
        if(t>0)
        {
            glm::vec3 p = origin + direction * t;
            if(p.x>=minAABB.x&&p.x<=maxAABB.x&&p.z>=minAABB.z&&p.z<=maxAABB.z)
            {
#ifdef BVH_DEBUG
                qDebug() << " BoxHit";
#endif
                return true;
            }
        }
    }

    if(direction.z < 0)
    {
        glm::vec3 normal(0,0,1);
        float t = glm::dot(maxAABB - origin, normal) / glm::dot(direction , normal);
        if(t>0)
        {
            glm::vec3 p = origin + direction * t;
            if(p.y>=minAABB.y&&p.y<=maxAABB.y&&p.x>=minAABB.x&&p.x<=maxAABB.x)
            {
#ifdef BVH_DEBUG
                qDebug() << " BoxHit";
#endif
                return true;
            }
        }
    }
    else if(direction.z > 0)
    {
        glm::vec3 normal(0,0,-1);
        float t = glm::dot(minAABB - origin, normal) / glm::dot(direction , normal);
        if(t>0)
        {
            glm::vec3 p = origin + direction * t;
            if(p.y>=minAABB.y&&p.y<=maxAABB.y&&p.x>=minAABB.x&&p.x<=maxAABB.x)
            {
#ifdef BVH_DEBUG
                qDebug() << " BoxHit";
#endif
                return true;
            }
        }
    }

#ifdef BVH_DEBUG
    qDebug() << " No BoxHit";
#endif
    return false;
}
