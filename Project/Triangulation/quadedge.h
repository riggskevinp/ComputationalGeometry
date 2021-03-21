#ifndef QUADEDGE_H
#define QUADEDGE_H

#include <memory>
#include <array>

#include "vertex.h"
#include "face.h"

class Edge
{
public:
    Edge(){};

    // Getters Setters
    std::shared_ptr<Vertex> getOrg(){return this->vert;}
    void setOrg(std::shared_ptr<Vertex> p){
        this->vert = p;
        p->addEdge(this->edges->at(index));
    }

    std::shared_ptr<Vertex> getDest(){return sym()->getOrg();}
    void setDest(std::shared_ptr<Vertex> p){
        sym()->setOrg(p);
        p->addEdge(sym());
    }

    std::shared_ptr<Face> getLeft(){return rot()->fac;}
    void setLeft(std::shared_ptr<Face> f){
        rot()->fac = f;
        f->addEdge(this->edges->at(index));
    }

    std::shared_ptr<Face> getRight(){return invRot()->fac;}
    void setRight(std::shared_ptr<Face> f){
        invRot()->fac = f;
        f->addEdge(sym());
    }

    void setNext(std::shared_ptr<Edge> e){this->next = e;}
    std::shared_ptr<Edge> getNext(){return this->next;}
    void setID(uint64_t i){this->ID = i;}
    uint64_t getID(){return this->ID;}
    void setIndex(uint16_t i){this->index = i;}
    uint16_t getIndex(){return this->index;}

    // Topological Operators
    static std::shared_ptr<Edge> makeEdge();
    static void splice(std::shared_ptr<Edge> a, std::shared_ptr<Edge> b);


    // Topological Operators for Delaunay Diagrams
    std::shared_ptr<Edge> connect(std::shared_ptr<Edge> a, std::shared_ptr<Edge> b/* and Side?*/);
    void deleteEdge(std::shared_ptr<Edge> e);
    void swap(std::shared_ptr<Edge> e);

    // Tests
    static double inCircle(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d);
    static bool CCW();
    static bool rightOf();
    static bool leftOf();

    // QuadEdge
    std::shared_ptr<Edge> rot(){return edges->at((index+1)%4);}
    std::shared_ptr<Edge> invRot(){return edges->at((index+3)%4);}
    std::shared_ptr<Edge> sym(){return edges->at((index+2)%4);}
    std::shared_ptr<Edge> oNext(){return next;}
    std::shared_ptr<Edge> oPrev(){return edges->at((index+1)%4)->next->rot();}
    std::shared_ptr<Edge> dNext(){return sym()->oNext()->sym();}
    std::shared_ptr<Edge> dPrev(){return invRot()->oNext()->invRot();}
    std::shared_ptr<Edge> lNext(){return invRot()->oNext()->rot();}
    std::shared_ptr<Edge> lPrev(){return oNext()->sym();}
    std::shared_ptr<Edge> rNext(){return rot()->oNext()->invRot();}
    std::shared_ptr<Edge> rPrev(){return sym()->oNext();}


private:
    std::shared_ptr<Vertex> vert = nullptr;
    std::shared_ptr<Face> fac = nullptr;
    std::shared_ptr<Edge> next = nullptr;
    uint64_t ID;
    uint16_t index;
    std::shared_ptr<std::array<std::shared_ptr<Edge>,4>> edges = nullptr;
    static uint64_t nextID;


};

#endif // QUADEDGE_H