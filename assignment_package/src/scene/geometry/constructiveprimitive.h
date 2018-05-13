#ifndef CONSTRUCTIVEPRIMITIVE_H
#define CONSTRUCTIVEPRIMITIVE_H

#include "primitive.h"

struct ConstructiveHit
{
    Intersection it;
    unsigned short inout;//1 in, 2 out
    unsigned short leftright;//1 left, 2 right
    ConstructiveHit(Intersection _it, unsigned short _inout, unsigned short _leftright)
    {
        it = _it;
        inout = _inout;
        leftright = _leftright;
    }
};

struct ConstructiveSegment
{
    ConstructiveHit cH1;
    ConstructiveHit cH2;
    unsigned short inout;//1 in, 2 out
    ConstructiveSegment(ConstructiveHit _cH1, ConstructiveHit _cH2, unsigned short _inout) : cH1(_cH1), cH2(_cH2), inout(_inout)
    {

    }
};


class ConstructiveNode
{
public:
    ConstructiveNode(unsigned short Type = -1);
    ~ConstructiveNode();

    std::shared_ptr<Shape> shape;
    unsigned short type;
    std::shared_ptr<ConstructiveNode> left;
    std::shared_ptr<ConstructiveNode> right;
};

class ConstructivePrimitive : public Primitive
{
public:
    ConstructivePrimitive();
    ~ConstructivePrimitive();

    std::shared_ptr<ConstructiveNode> root;

    bool Intersect(const Ray &r, Intersection *isect) const;
    std::vector<ConstructiveSegment> traverseIntersect(std::shared_ptr<ConstructiveNode> n, const Ray &r, unsigned short leftright) const;
    std::vector<ConstructiveSegment> combineHits(std::vector<ConstructiveSegment> &LCS, std::vector<ConstructiveSegment> &RCS, unsigned short type) const;
};

#endif // CONSTRUCTIVEPRIMITIVE_H
