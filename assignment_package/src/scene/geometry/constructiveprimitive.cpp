#include "constructiveprimitive.h"

//#define MY_DEBUG

ConstructiveNode::ConstructiveNode(unsigned short Type) : type(Type)
{

}

ConstructiveNode::~ConstructiveNode()
{

}

ConstructivePrimitive::ConstructivePrimitive()
{

}

ConstructivePrimitive::~ConstructivePrimitive()
{

}

bool ConstructivePrimitive::Intersect(const Ray &r, Intersection *isect) const
{
#ifdef MY_DEBUG
    qDebug() << ">enter intersect";
#endif

    std::vector<ConstructiveSegment> cS = traverseIntersect(root, r, 1);
    bool result = false;

    if(cS.empty())
    {
#ifdef MY_DEBUG
        qDebug() << ">empty";
#endif
        result = false;
    }
    else
    {
#ifdef MY_DEBUG
        for(auto item : cS)
        {
            qDebug() << ">cS:" << item.inout << ":" << item.cH1.it.t << "->" << item.cH2.it.t;
        }
#endif
        //        //this situation does not exist
        //        if(cS.size()==1&&cS[0].inout==1)
        //            *isect = cS[0].cH2.it;

        for(int i = 0;i<cS.size()-1;i++)
        {
            if(cS[i].inout!=cS[i+1].inout)
            {
                *isect = cS[i].cH2.it;
                //*isect = cS[i+1].cH1.it;
                isect->objectHit = this;
                result = true;
                break;
            }
        }
    }
#ifdef MY_DEBUG
    qDebug() << ">leave intersect:" << result;
#endif

    return result;
}

std::vector<ConstructiveSegment> ConstructivePrimitive::traverseIntersect(std::shared_ptr<ConstructiveNode> n, const Ray &r, unsigned short leftright) const
{
    //only support convex primitive
    //meaning concave mesh is not supported in the tree

    if(n.get()==nullptr)
        return std::vector<ConstructiveSegment>();

    //leaf node
    if(n->left==nullptr&&n->right==nullptr)
    {
        Ray ray = r;
        Intersection isect;
        std::vector<ConstructiveSegment> CS;
        Intersection atLeastTwoHit_1_Isect, atLeastTwoHit_2_Isect;
        atLeastTwoHit_1_Isect.t = 0;
        atLeastTwoHit_2_Isect.t = std::numeric_limits<float>::max();
        ConstructiveHit atLeastTwoHit_1(atLeastTwoHit_1_Isect, -1, leftright);
        ConstructiveHit atLeastTwoHit_2(atLeastTwoHit_2_Isect, -1, leftright);
        ConstructiveHit last = atLeastTwoHit_1;//last is always the start of a segment
        while(n->shape->Intersect(ray, &isect))
        {
            ray = isect.SpawnRay(ray.direction);
            if(glm::dot(isect.normalGeometric, ray.direction)>0)//the segment is in (shooting outwards)
            {
                isect.t = glm::length(isect.point - r.origin) / glm::length(r.direction);//two ray have different origins
                ConstructiveHit cH(isect, 2, leftright);
                CS.push_back(ConstructiveSegment(last, cH, 1));

                Intersection newIsect = isect;
                newIsect.point = ray.origin;
                //newIsect.normalGeometric = -newIsect.normalGeometric;
                //newIsect.tangent = -newIsect.tangent;
                //newIsect.bitangent = -newIsect.bitangent;
                newIsect.t = glm::length(ray.origin - r.origin) / glm::length(r.direction);//two ray have different origins
                ConstructiveHit newHit(newIsect, 1, leftright);
                last = newHit;
            }
            else//the segment is out (shooting inwards)
            {
                isect.t = glm::length(isect.point - r.origin) / glm::length(r.direction);//two ray have different origins
                ConstructiveHit cH(isect, 2, leftright);
                CS.push_back(ConstructiveSegment(last, cH, 2));

                Intersection newIsect = isect;
                newIsect.point = ray.origin;
                //newIsect.normalGeometric = -newIsect.normalGeometric;
                //newIsect.tangent = -newIsect.tangent;
                //newIsect.bitangent = -newIsect.bitangent;
                newIsect.t = glm::length(ray.origin - r.origin) / glm::length(r.direction);//two ray have different origins
                ConstructiveHit newHit(newIsect, 1, leftright);
                last = newHit;
            }
        }
        ConstructiveSegment atLeastOneSegment(last, atLeastTwoHit_2, 2);//last segment is always out
        CS.push_back(atLeastOneSegment);
        return CS;
    }

    std::vector<ConstructiveSegment> leftCS;
    std::vector<ConstructiveSegment> rightCS;

#ifdef MY_DEBUG
    qDebug() << ">>left";
#endif
    if(n->left.get()!=nullptr) leftCS = traverseIntersect(n->left, r, 1);
    if(leftCS.empty()&&n->type!=1) return leftCS;//left is empty and not union
#ifdef MY_DEBUG
    qDebug() << ">>right";
#endif
    if(n->right.get()!=nullptr) rightCS = traverseIntersect(n->right, r, 2);

    //regular non-leaf node
    std::vector<ConstructiveSegment> CS = combineHits(leftCS, rightCS, n->type);
    return CS;
}

std::vector<ConstructiveSegment> ConstructivePrimitive::combineHits(std::vector<ConstructiveSegment> &LCS, std::vector<ConstructiveSegment> &RCS, unsigned short type) const
{
#ifdef MY_DEBUG
    qDebug() << ">>>enter combineSegments:" << LCS.size() << "," << RCS.size();
    for(auto item : LCS)
    {
        qDebug() << ">>>LCS:" << item.inout << ":" << item.cH1.it.t << "->" << item.cH2.it.t;
    }
    for(auto item : RCS)
    {
        qDebug() << ">>>RCS:" << item.inout << ":" << item.cH1.it.t << "->" << item.cH2.it.t;
    }
#endif

    std::vector<ConstructiveSegment> CS;

    int i, j;
    int lSize = LCS.size(), rSize = RCS.size();
    for(i = 0, j=0;i<lSize&&j<rSize;)
    {
#ifdef MY_DEBUG
        qDebug() << ">>>i:" << i << ",j:" << j;
#endif
        ConstructiveSegment &lS = LCS[i];
        ConstructiveSegment &rS = RCS[j];
        ConstructiveHit &ln = LCS[i].cH1;
        ConstructiveHit &lf = LCS[i].cH2;
        ConstructiveHit &rn = RCS[j].cH1;
        ConstructiveHit &rf = RCS[j].cH2;

#ifdef MY_DEBUG
        qDebug() << ">>>ln:" << ln.it.t << ",lf:" << lf.it.t << ",LS:" << lS.inout;
        qDebug() << ">>>rn:" << rn.it.t << ",rf:" << rf.it.t << ",RS:" << rS.inout;
#endif

        if(ln.it.t == lf.it.t)//ln=lf
        {
            i++;//this situation will update index without condition
            continue;
        }
        else if(rn.it.t == rf.it.t)//rn=rf
        {
            j++;//this situation will update index without condition
            continue;
        }
        else if(ln.it.t == rn.it.t &&
                ln.it.t < lf.it.t &&
                lf.it.t == rf.it.t)//ln=rn<lf=rf
        {
#ifdef MY_DEBUG
            qDebug() << ">>>situation 0:";
#endif
            switch(type)
            {
            case 0://intersect
                if(lS.inout==1&&rS.inout==1)//l in, r in
                {
                    CS.push_back(ConstructiveSegment(ln, lf, 1));
                }
                else
                {
                    CS.push_back(ConstructiveSegment(ln, lf, 2));
                }
                break;
            case 1://union
                if(lS.inout==2&&rS.inout==2)//l out, r out
                {
                    CS.push_back(ConstructiveSegment(ln, lf, 2));
                }
                else
                {
                    CS.push_back(ConstructiveSegment(ln, lf, 1));
                }
                break;
            case 2://difference
                if(lS.inout==1&&rS.inout==2)//l in, r out
                {
                    CS.push_back(ConstructiveSegment(ln, lf, 1));
                }
                else
                {
                    CS.push_back(ConstructiveSegment(ln, lf, 2));
                }
                break;
            default:
                break;
            }
        }
        else if(ln.it.t <= rn.it.t &&
                rn.it.t <= lf.it.t &&
                lf.it.t <= rf.it.t)//ln-rn-lf-rf
        {
#ifdef MY_DEBUG
            qDebug() << ">>>situation 1:";
#endif
            switch(type)
            {
            case 0://intersect
                if(lS.inout==1&&rS.inout==1)//l in, r in
                {
                    //CS.push_back(ConstructiveSegment(ln, rn, 2));
                    CS.push_back(ConstructiveSegment(rn, lf, 1));//in
                    //CS.push_back(ConstructiveSegment(lf, rf, 2));
                }
                else
                {
                    //CS.push_back(ConstructiveSegment(ln, rf, 2));
                    CS.push_back(ConstructiveSegment(rn, lf, 2));
                }
                break;
            case 1://union
                if(lS.inout==2&&rS.inout==2)//l out, r out
                {
                    CS.push_back(ConstructiveSegment(rn, lf, 2));
                }
                else
                {
                    CS.push_back(ConstructiveSegment(rn, lf, 1));
                }
                break;
            case 2://difference
                if(lS.inout==1&&rS.inout==2)//l in, r out
                {
                    ConstructiveHit rnTemp = rn;//avoid modify original data in the vector incase it is used in the following loop
                    //rnTemp.it.point += -rnTemp.it.normalGeometric*RayEpsilon;
                    rnTemp.it.normalGeometric = -rn.it.normalGeometric;
                    CS.push_back(ConstructiveSegment(rnTemp, lf, 1));
                }
                else
                {
                    ConstructiveHit rnTemp = rn;//avoid modify original data in the vector incase it is used in the following loop
                    //rnTemp.it.point += -rnTemp.it.normalGeometric*RayEpsilon;
                    rnTemp.it.normalGeometric = -rn.it.normalGeometric;
                    CS.push_back(ConstructiveSegment(rnTemp, lf, 2));
                }
                break;
            default:
                break;
            }
        }
        else if(rn.it.t <= ln.it.t &&
                ln.it.t <= rf.it.t &&
                rf.it.t <= lf.it.t)//rn-ln-rf-lf
        {
#ifdef MY_DEBUG
            qDebug() << ">>>situation 2:";
#endif
            switch(type)
            {
            case 0://intersect
                if(lS.inout==1&&rS.inout==1)//l in, r in
                {
                    //CS.push_back(ConstructiveSegment(rn, ln, 2));
                    CS.push_back(ConstructiveSegment(ln, rf, 1));//in
                    //CS.push_back(ConstructiveSegment(rf, lf, 2));
                }
                else
                {
                    //CS.push_back(ConstructiveSegment(rn, lf, 2));
                    CS.push_back(ConstructiveSegment(ln, rf, 2));
                }
                break;
            case 1://union
                if(lS.inout==2&&rS.inout==2)//l out, r out
                {
                    CS.push_back(ConstructiveSegment(ln, rf, 2));
                }
                else
                {
                    CS.push_back(ConstructiveSegment(ln, rf, 1));
                }
                break;
            case 2://difference
                if(lS.inout==1&&rS.inout==2)//l in, r out
                {
                    ConstructiveHit rfTemp = rf;
                    //rfTemp.it.point += -rfTemp.it.normalGeometric*RayEpsilon;
                    rfTemp.it.normalGeometric = -rfTemp.it.normalGeometric;
                    CS.push_back(ConstructiveSegment(ln, rfTemp, 1));
                }
                else
                {
                    ConstructiveHit rfTemp = rf;
                    //rfTemp.it.point += -rfTemp.it.normalGeometric*RayEpsilon;
                    rfTemp.it.normalGeometric = -rfTemp.it.normalGeometric;
                    CS.push_back(ConstructiveSegment(ln, rfTemp, 2));
                }
                break;
            default:
                break;
            }
        }
        else if(ln.it.t <= rn.it.t &&
                rn.it.t <= rf.it.t &&
                rf.it.t <= lf.it.t)//ln-rn-rf-lf
        {
#ifdef MY_DEBUG
            qDebug() << ">>>situation 3:";
#endif
            switch(type)
            {
            case 0://intersect
                if(lS.inout==1&&rS.inout==1)//l in, r in
                {
                    //CS.push_back(ConstructiveSegment(ln, rn, 2));
                    CS.push_back(ConstructiveSegment(rn, rf, 1));//in
                    //CS.push_back(ConstructiveSegment(rf, lf, 2));
                }
                else
                {
                    //CS.push_back(ConstructiveSegment(ln, lf, 2));
                    CS.push_back(ConstructiveSegment(rn, rf, 2));
                }
                break;
            case 1://union
                if(lS.inout==2&&rS.inout==2)//l out, r out
                {
                    CS.push_back(ConstructiveSegment(rn, rf, 2));
                }
                else
                {
                    CS.push_back(ConstructiveSegment(rn, rf, 1));
                }
                break;
            case 2://difference
                if(lS.inout==1&&rS.inout==2)//l in, r out
                {
                    ConstructiveHit rnTemp = rn;
                    ConstructiveHit rfTemp = rf;
                    //rnTemp.it.point += -rnTemp.it.normalGeometric*RayEpsilon;
                    rnTemp.it.normalGeometric = -rnTemp.it.normalGeometric;
                    //rfTemp.it.point += -rfTemp.it.normalGeometric*RayEpsilon;
                    rfTemp.it.normalGeometric = -rfTemp.it.normalGeometric;
                    CS.push_back(ConstructiveSegment(rnTemp, rfTemp, 1));
                }
                else
                {
                    ConstructiveHit rnTemp = rn;
                    ConstructiveHit rfTemp = rf;
                    //rnTemp.it.point += -rnTemp.it.normalGeometric*RayEpsilon;
                    rnTemp.it.normalGeometric = -rnTemp.it.normalGeometric;
                    //rfTemp.it.point += -rfTemp.it.normalGeometric*RayEpsilon;
                    rfTemp.it.normalGeometric = -rfTemp.it.normalGeometric;
                    CS.push_back(ConstructiveSegment(rnTemp, rfTemp, 2));
                }
                break;
            default:
                break;
            }
        }
        else if(rn.it.t <= ln.it.t &&
                ln.it.t <= lf.it.t &&
                lf.it.t <= rf.it.t)//rn-ln-lf-rf
        {
#ifdef MY_DEBUG
            qDebug() << ">>>situation 4:";
#endif
            switch(type)
            {
            case 0://intersect
                if(lS.inout==1&&rS.inout==1)//l in, r in
                {
                    //CS.push_back(ConstructiveSegment(rn, ln, 2));
                    CS.push_back(ConstructiveSegment(ln, lf, 1));//in
                    //CS.push_back(ConstructiveSegment(lf, rf, 2));
                }
                else
                {
                    //CS.push_back(ConstructiveSegment(rn, rf, 2));
                    CS.push_back(ConstructiveSegment(ln, lf, 2));
                }
                break;
            case 1://union
                if(lS.inout==2&&rS.inout==2)//l out, r out
                {
                    CS.push_back(ConstructiveSegment(ln, lf, 2));
                }
                else
                {
                    CS.push_back(ConstructiveSegment(ln, lf, 1));
                }
                break;
            case 2://difference
                if(lS.inout==1&&rS.inout==2)//l in, r out
                {
                    CS.push_back(ConstructiveSegment(ln, lf, 1));
                }
                else
                {
                    CS.push_back(ConstructiveSegment(ln, lf, 2));
                }
                break;
            default:
                break;
            }
        }
        else if(ln.it.t <= lf.it.t &&
                lf.it.t <= rn.it.t &&
                rn.it.t <= rf.it.t)//ln-lf-rn-rf
        {
#ifdef MY_DEBUG
            qDebug() << ">>>situation 5:";
#endif
            switch(type)
            {
            case 0://intersect
//                if(lS.inout==1&&rS.inout==1)//l in, r in
//                {
//                    CS.push_back(ConstructiveSegment(ln, rf, 2));
//                }
//                else
//                {
//                    CS.push_back(ConstructiveSegment(ln, rf, 2));
//                }
                break;
            case 1:
                break;
            case 2:
                break;
            default:
                break;
            }
        }
        else if(rn.it.t <= rf.it.t &&
                rf.it.t <= ln.it.t &&
                ln.it.t <= lf.it.t)//rn-rf-ln-lf
        {
#ifdef MY_DEBUG
            qDebug() << ">>>situation 6:";
#endif
            switch(type)
            {
            case 0://intersect
//                if(lS.inout==1&&rS.inout==1)//l in, r in
//                {
//                    CS.push_back(ConstructiveSegment(rn, lf, 2));
//                }
//                else
//                {
//                    CS.push_back(ConstructiveSegment(rn, lf, 2));
//                }
                break;
            case 1:
                break;
            case 2:
                break;
            default:
                break;
            }
        }

        if(i+1<lSize&&j+1<rSize)//update the index of the next nearest segment
        {
            if(LCS[i+1].cH1.it.t<RCS[j+1].cH1.it.t)
                i++;
            else
                j++;
        }
        else if(i+1<lSize&&j+1>=rSize)
        {
            i++;
        }
        else if(j+1<rSize&&i+1>=lSize)
        {
            j++;
        }
        else if(i+1>=lSize&&j+1>=rSize)
        {
            break;
        }

#ifdef MY_DEBUG
        qDebug() << ">>>i:" << i << ",j:" << j;
#endif
    }

#ifdef MY_DEBUG
    qDebug() << ">>>leave combineSegments:" << CS.size();
    for(auto item : CS)
    {
        qDebug() << ">>>CS:" << item.inout << ":" << item.cH1.it.t << "->" << item.cH2.it.t;
    }
#endif

    return CS;
}
