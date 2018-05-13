#include "kdtree.h"

KDNode::KDNode()
    : leftChild(nullptr), rightChild(nullptr), axis(0), minCorner(), maxCorner(), particles()
{}

KDNode::~KDNode()
{
    delete leftChild;
    delete rightChild;
}

KDTree::KDTree()
    : root(nullptr)
{}

KDTree::~KDTree()
{
    delete root;
}

// Comparator functions you can use with std::sort to sort vec3s along the cardinal axes
bool xSort(Photon* a, Photon* b) { return a->pos.x < b->pos.x; }
bool ySort(Photon* a, Photon* b) { return a->pos.y < b->pos.y; }
bool zSort(Photon* a, Photon* b) { return a->pos.z < b->pos.z; }

bool xSortEx(Photon a, Photon b) { return a.pos.x < b.pos.x; }
bool ySortEx(Photon a, Photon b) { return a.pos.y < b.pos.y; }
bool zSortEx(Photon a, Photon b) { return a.pos.z < b.pos.z; }

void KDTree::build(std::vector<Photon> *points)
{
    //TODO
    int size = points->size();

    if(size<=0) return;

    std::sort(points->begin(),points->end(),xSortEx);
    int median = size / 2;

    std::vector<Photon*> left;
    for(int i = 0;i<median;i++)
    {
        left.push_back(&(points->at(i)));
    }
    std::vector<Photon*> right;
    for(int i = median;i<size;i++)
    {
        right.push_back(&(points->at(i)));
    }

    root = new KDNode();
    root->axis = 0;
    getMinMax(points, root->minCorner, root->maxCorner);
    root->leftChild = buildTree(left, root->axis);
    root->rightChild = buildTree(right, root->axis);
    minCorner = root->minCorner;
    maxCorner = root->maxCorner;
    //    qDebug() << root->minCorner.x << ","  << root->minCorner.y << ","  << root->minCorner.z << ","  << root->maxCorner.x << ","  << root->maxCorner.y << ","  << root->maxCorner.z;
}

KDNode* KDTree::buildTree(std::vector<Photon*>& points, int parentAxis)
{
    int size = points.size();

    if(size<=0) return nullptr;

    int median = size / 2;
    KDNode* currentNode = new KDNode();

    currentNode->axis = (parentAxis+1)%3;
    if(size==1)
    {
        currentNode->minCorner = points[0]->pos;
        currentNode->maxCorner = points[0]->pos;
        currentNode->particles.push_back(*points[0]);
    }
    else
    {
        //currentNode->minCorner = parentMin;
        //currentNode->maxCorner = parentMax;
        getMinMax(points, currentNode->minCorner, currentNode->maxCorner);
        switch(parentAxis)
        {
        case 0:
            std::sort(points.begin(),points.end(),ySort);
            //if(left) currentNode->maxCorner.x = parentMedian.x;
            //else currentNode->minCorner.x = parentMedian.x;
            break;
        case 1:
            std::sort(points.begin(),points.end(),zSort);
            //if(left) currentNode->maxCorner.y = parentMedian.y;
            //else currentNode->minCorner.y = parentMedian.y;
            break;
        case 2:
            std::sort(points.begin(),points.end(),xSort);
            //if(left) currentNode->maxCorner.z = parentMedian.z;
            //else currentNode->minCorner.z = parentMedian.z;
            break;
        }
        std::vector<Photon*> left(points.begin(), points.begin()+median);
        std::vector<Photon*> right(points.begin()+median, points.end());
        currentNode->leftChild = buildTree(left, currentNode->axis);
        currentNode->rightChild = buildTree(right, currentNode->axis);
    }
    return currentNode;
}


std::vector<Photon> KDTree::particlesInSphere(glm::vec3 c, float r, glm::vec3 normal)
{
    //TODO
    std::vector<Photon> points;

    traverseParticlesInSphere(root, c, r, points, normal, false);

    //qDebug() << "points:" << points.size();
    //for(auto iter : points)
    //qDebug() << iter.x << "," << iter.y << "," << iter.z << "<<";
    //    auto f = [c](Photon a, Photon b){return glm::length(a.pos-c) < glm::length(b.pos-c);};
    //    std::sort(points.begin(),points.end(),f);
    return points;
}

void KDTree::traverseParticlesInSphere(KDNode *n, glm::vec3 c, float r, std::vector<Photon> &points, glm::vec3 normal, bool completelyIn)
{
    if(n==nullptr) return;

    //qDebug() << "node:" << n->minCorner.x << ","  << n->minCorner.y << ","  << n->minCorner.z << ","  << n->maxCorner.x << ","  << n->maxCorner.y << ","  << n->maxCorner.z;
    //qDebug() << "sphere:" << c.x << "," << c.y << "," << c.z << ":" << r;

    if(completelyIn)
    {
        if(n->particles.size()>0)
        {
            for(auto iter : n->particles)
            {
                if(//pointSphereIntersection(c, r, iter.pos) && //unnucessary since one node contains only one particle and it is already checked by using minCorner/maxCorner
                        glm::abs(glm::dot(iter.pos-c,normal)) < THICKNESS)//not sphere but disk
                    points.push_back(iter);
            }
        }
        traverseParticlesInSphere(n->leftChild, c, r, points, normal, true);
        traverseParticlesInSphere(n->rightChild, c, r, points, normal, true);
    }
    else
    {
        int intersect = boxSphereIntersection(c,r,n->minCorner,n->maxCorner);
        if(intersect>0)
        {
            //assuming particles are stored only on leaf nodes and there is only 1 or 0 particles per leaf node
            if(n->particles.size()>0)
            {
                for(auto iter : n->particles)
                {
                    if(//pointSphereIntersection(c, r, iter.pos) && //unnucessary since one node contains only one particle and it is already checked by using minCorner/maxCorner
                            glm::abs(glm::dot(iter.pos-c,normal)) < THICKNESS)//not sphere but disk
                        points.push_back(iter);
                }
            }
            //qDebug() << n->axis;
            if(intersect==2)
            {
                traverseParticlesInSphere(n->leftChild, c, r, points, normal,true);
                traverseParticlesInSphere(n->rightChild, c, r, points, normal,true);
            }
            else
            {
                traverseParticlesInSphere(n->leftChild, c, r, points, normal,false);
                traverseParticlesInSphere(n->rightChild, c, r, points, normal,false);
            }
        }
    }
}

int KDTree::boxSphereIntersection(glm::vec3 c, float r, glm::vec3 min, glm::vec3 max)
{
    int result = 0;
    glm::vec3 cNear = glm::clamp(c, min, max);
    if( r*r > (cNear.x-c.x)*(cNear.x-c.x)
            + (cNear.y-c.y)*(cNear.y-c.y)
            + (cNear.z-c.z)*(cNear.z-c.z))
    {
        result++;
        glm::vec3 temp = max - min;
        if(temp.x*temp.x+temp.y*temp.y+temp.z*temp.z < r*r&&
           pointSphereIntersection(c, r, min)&&
           pointSphereIntersection(c, r, max)&&
           pointSphereIntersection(c, r, glm::vec3(min.x, min.y, max.z))&&
           pointSphereIntersection(c, r, glm::vec3(min.x, max.z, min.z))&&
           pointSphereIntersection(c, r, glm::vec3(max.x, min.y, min.z))&&
           pointSphereIntersection(c, r, glm::vec3(min.x, max.y, max.z))&&
           pointSphereIntersection(c, r, glm::vec3(max.x, min.y, max.z))&&
           pointSphereIntersection(c, r, glm::vec3(max.x, max.y, min.z)))
        {
            result++;
        }
    }
    return result;
}

bool KDTree::pointSphereIntersection(glm::vec3 c, float r, glm::vec3 p)
{
    return r*r > (p.x-c.x)*(p.x-c.x)
            + (p.y-c.y)*(p.y-c.y)
            + (p.z-c.z)*(p.z-c.z);
}

void KDTree::getMinMax(std::vector<Photon> *points, glm::vec3 &min, glm::vec3 &max)
{
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();
    float maxZ = std::numeric_limits<float>::min();
    for (auto iter : *points)
    {
        if(iter.pos.x < minX) minX = iter.pos.x;
        if(iter.pos.y < minY) minY = iter.pos.y;
        if(iter.pos.z < minZ) minZ = iter.pos.z;

        if(iter.pos.x > maxX) maxX = iter.pos.x;
        if(iter.pos.y > maxY) maxY = iter.pos.y;
        if(iter.pos.z > maxZ) maxZ = iter.pos.z;
    }
    min.x = minX;
    min.y = minY;
    min.z = minZ;

    max.x = maxX;
    max.y = maxY;
    max.z = maxZ;
}

void KDTree::getMinMax(std::vector<Photon*>& points, glm::vec3 &min, glm::vec3 &max)
{
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();
    float maxZ = std::numeric_limits<float>::min();
    for (auto iter : points)
    {
        if(iter->pos.x < minX) minX = iter->pos.x;
        if(iter->pos.y < minY) minY = iter->pos.y;
        if(iter->pos.z < minZ) minZ = iter->pos.z;

        if(iter->pos.x > maxX) maxX = iter->pos.x;
        if(iter->pos.y > maxY) maxY = iter->pos.y;
        if(iter->pos.z > maxZ) maxZ = iter->pos.z;
    }
    min.x = minX;
    min.y = minY;
    min.z = minZ;

    max.x = maxX;
    max.y = maxY;
    max.z = maxZ;
}

void KDTree::clear()
{
    delete root;
    root = nullptr;
}
