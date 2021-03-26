#include "quadedge.h"

#include <array>
#include <memory>
#include <set>


uint64_t Edge::nextID = 0;

std::shared_ptr<Edge> Edge::makeEdge()
{
    auto e1 = std::make_shared<Edge>();
    auto e2 = std::make_shared<Edge>();
    auto e3 = std::make_shared<Edge>();
    auto e4 = std::make_shared<Edge>();

    e1->setID(Edge::nextID);
    e2->setID(Edge::nextID+1);
    e3->setID(Edge::nextID+2);
    e4->setID(Edge::nextID+3);
    Edge::nextID += 4;

    e1->oNext = e1;
    e2->oNext = e2;
    e3->oNext = e4;
    e4->oNext = e3;

    e1->rot = e3;
    e2->rot = e4;
    e3->rot = e2;
    e4->rot = e1;

    return e1;

}

void Edge::splice(std::shared_ptr<Edge> a, std::shared_ptr<Edge> b)
{
    if(a != nullptr && b != nullptr){
        a->oNext.swap(b->oNext);
        a->oNext->rotf()->oNext.swap(b->oNext->rotf()->oNext);
    }
}

std::shared_ptr<Edge> Edge::connect(std::shared_ptr<Edge> a, std::shared_ptr<Edge> b)
{
    auto e = Edge::makeEdge();
    e->setOrg(a->getDest());
    e->setDest(b->getOrg());
    Edge::splice(e, a->lNext());
    Edge::splice(e->sym(), b);
    return e;
}

void Edge::deleteEdge(std::shared_ptr<Edge> e)
{
    if(e != nullptr){
        splice(e, e->oPrev());
        splice(e->sym(), e->sym()->oPrev());
    }
}

void Edge::swap(std::shared_ptr<Edge> e)
{
    if(e != nullptr){
        std::shared_ptr<Edge> a = e->oPrev();
        std::shared_ptr<Edge> b = e->sym()->oPrev();
        splice(e,a);
        splice(e->sym(),b);
        splice(e, a->lNext());
        splice(e->sym(), b->lNext());
        e->setOrg(a->getDest());
        e->setDest(b->getDest());
    }
}

double Edge::inCircle(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d)
{
    // As demonstrated in predicates.c incircle()
    double adx, ady, bdx, bdy, cdx, cdy;
    double abdet, bcdet, cadet;
    double alift, blift, clift;

    adx = a.getX() - d.getX();
    ady = a.getY() - d.getY();
    bdx = b.getX() - d.getX();
    bdy = b.getY() - d.getY();
    cdx = c.getX() - d.getX();
    cdy = c.getY() - d.getY();

    abdet = adx * bdy - bdx * ady;
    bcdet = bdx * cdy - cdx * bdy;
    cadet = cdx * ady - adx * cdy;
    alift = adx * adx + ady * ady;
    blift = bdx * bdx + bdy * bdy;
    clift = cdx * cdx + cdy * cdy;

    return alift * bcdet + blift * cadet + clift * abdet;
}

double Edge::CCW(const Vertex &a, const Vertex &b, const Vertex &c)
{
    // As demonstrated in predicates.c orient2dfast()
    double acx, bcx, acy, bcy;

    acx = a.getX() - c.getX();
    bcx = b.getX() - c.getX();
    acy = a.getY() - c.getY();
    bcy = b.getY() - c.getY();

    return acx * bcy - acy * bcx;
}

void Edge::displayPoints(std::vector<Vertex> s, cv::Mat& m)
{
    //int height = 500;
    //int width = 500;

    //auto img = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    for(auto p: s){
        cv::circle(m, p.getPoint(), 0, cv::Scalar(0,0,255),5);
    }
    cv::imshow("Vertices", m);

}

void Edge::visualizeHull(std::pair<std::shared_ptr<Edge>, std::shared_ptr<Edge> > hull, cv::Mat m)
{
    std::set<uint64_t> vertVisited;
    std::shared_ptr<Edge> temp = hull.first;
    while((temp->getOrg()->getID() != hull.second->getOrg()->getID()) && (vertVisited.find(temp->getOrg()->getID()) == vertVisited.end())){
        cv::arrowedLine(m, temp->getOrg()->getPoint(), temp->getDest()->getPoint(), cv::Scalar(0,255,0), 5);
        vertVisited.insert(temp->getOrg()->getID());
        temp = temp->lNext();
    }
    temp = hull.second->oPrev();
    vertVisited.clear();
    while(temp->getOrg()->getID() != hull.first->getOrg()->getID() && (vertVisited.find(temp->getOrg()->getID()) == vertVisited.end())){
        cv::arrowedLine(m, temp->getOrg()->getPoint(), temp->getDest()->getPoint(), cv::Scalar(255,0,0), 1);
        temp = temp->lNext();
    }
}


