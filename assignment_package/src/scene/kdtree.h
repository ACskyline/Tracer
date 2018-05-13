#pragma once
#include <la.h>
#include <scene/photon.h>

#define THICKNESS 0.0001

class KDNode
{
public:
    KDNode();
    ~KDNode();

    KDNode* leftChild;
    KDNode* rightChild;
    unsigned int axis; // Which axis split this node represents
    glm::vec3 minCorner, maxCorner; // The world-space bounds of this node
    std::vector<Photon> particles; // A collection of pointers to the particles contained in this node.
};

class KDTree
{
private:
    void getMinMax(std::vector<Photon> *points, glm::vec3 &min, glm::vec3 &max);
    void getMinMax(std::vector<Photon*>& points, glm::vec3 &min, glm::vec3 &max);
    bool pointSphereIntersection(glm::vec3 c, float r, glm::vec3 p);
    int boxSphereIntersection(glm::vec3 c, float r, glm::vec3 min, glm::vec3 max);
    void traverseParticlesInSphere(KDNode *n, glm::vec3 c, float r, std::vector<Photon> &points, glm::vec3 normal, bool completelyIn);
    KDNode* buildTree(std::vector<Photon*>& points, int parentAxis);
public:
    KDTree();
    ~KDTree();
    void build(std::vector<Photon> *points);
    void clear();


    std::vector<Photon> particlesInSphere(glm::vec3 c, float r, glm::vec3 normal); // Returns all the points contained within a sphere with center c and radius r

    KDNode* root;
    glm::vec3 minCorner, maxCorner; // For visualization purposes
};
