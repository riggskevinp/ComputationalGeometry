#include "quadedge.h"

#include <array>
#include <memory>

uint64_t Edge::nextID = 0;

std::shared_ptr<Edge> Edge::makeEdge()
{
    // Edge store as e0.Rot^r
    // r : {0,1,2,3}
    auto edges = std::make_shared<std::array<std::shared_ptr<Edge>,4>>();
    edges->at(0)->setIndex(0);
    edges->at(1)->setIndex(1);
    edges->at(2)->setIndex(2);
    edges->at(3)->setIndex(3);

    edges->at(0)->setNext(edges->at(0));
    edges->at(1)->setNext(edges->at(3));
    edges->at(2)->setNext(edges->at(2));
    edges->at(3)->setNext(edges->at(1));

    edges->at(0)->setID(Edge::nextID);
    edges->at(1)->setID(Edge::nextID+1);
    edges->at(2)->setID(Edge::nextID+2);
    edges->at(3)->setID(Edge::nextID+3);

    Edge::nextID += 4;

    edges->at(0)->edges = edges;
    edges->at(1)->edges = edges;
    edges->at(2)->edges = edges;
    edges->at(3)->edges = edges;

    return edges->at(0);

}

void Edge::splice(std::shared_ptr<Edge> a, std::shared_ptr<Edge> b)
{
    if(a != nullptr && b != nullptr){
        std::shared_ptr<Edge> alpha = a->oNext()->rot();
        std::shared_ptr<Edge> beta = b->oNext()->rot();

        std::shared_ptr<Edge> temp1 = b->oNext();
        std::shared_ptr<Edge> temp2 = a->oNext();
        std::shared_ptr<Edge> temp3 = beta->oNext();
        std::shared_ptr<Edge> temp4 = alpha->oNext();

        a->next = temp1;
        b->next = temp2;
        alpha->next = temp3;
        beta->next = temp4;
    }
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
