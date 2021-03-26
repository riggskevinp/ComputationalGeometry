#ifndef QUADEDGE_H
#define QUADEDGE_H

#include <memory>
#include <array>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "vertex.h"
#include "face.h"

class Edge
{
public:
    Edge(){};

    // Getters Setters
    std::shared_ptr<Vertex> getOrg(){return this->origin;}
    void setOrg(std::shared_ptr<Vertex> p){
        this->origin = p;
        p->addEdge(this->sym()->sym());//->edges->at(index));
    }

    std::shared_ptr<Vertex> getDest(){return sym()->getOrg();}
    void setDest(std::shared_ptr<Vertex> p){
        sym()->setOrg(p);
        p->addEdge(sym());
    }

    std::shared_ptr<Face> getLeft(){return rotf()->fac;}
    void setLeft(std::shared_ptr<Face> f){
        rotf()->fac = f;
        f->addEdge(this->sym()->sym());//->edges->at(index));
    }

    std::shared_ptr<Face> getRight(){return invRotf()->fac;}
    void setRight(std::shared_ptr<Face> f){
        invRotf()->fac = f;
        f->addEdge(sym());
    }

    void setNext(std::shared_ptr<Edge> e){this->oNext = e;}
    void setID(uint64_t i){this->ID = i;}
    uint64_t getID(){return this->ID;}
    void setIndex(uint16_t i){this->index = i;}
    uint16_t getIndex(){return this->index;}

    // Topological Operators
    static std::shared_ptr<Edge> makeEdge();
    static void splice(std::shared_ptr<Edge> a, std::shared_ptr<Edge> b);


    // Topological Operators for Delaunay Diagrams
    static std::shared_ptr<Edge> connect(std::shared_ptr<Edge> a, std::shared_ptr<Edge> b/* and Side?*/);
    static void deleteEdge(std::shared_ptr<Edge> e);
    void swap(std::shared_ptr<Edge> e);

    // Tests
    static double inCircle(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d);
    static double CCW(const Vertex& a, const Vertex& b, const Vertex& c);
    static double rightOf(const Vertex& x, Edge& e){return CCW(x, *(e.getDest()), *(e.getOrg()));}
    static double leftOf(const Vertex& x, Edge& e){return CCW(x, *(e.getOrg()), *(e.getDest()));}
    static double valid(Edge& e, Edge& b1){ return rightOf(*(e.getDest()),b1);}


    // QuadEdge
    std::shared_ptr<Edge> rotf(){return rot;}
    std::shared_ptr<Edge> rot = nullptr;
    std::shared_ptr<Edge> invRotf(){return rot->rot->rot;}
    std::shared_ptr<Edge> sym(){return rot->rot;}
    std::shared_ptr<Edge> oNextf(){return oNext;}
    std::shared_ptr<Edge> oNext = nullptr;
    std::shared_ptr<Edge> oPrev(){return rot->oNext->rotf();}
    std::shared_ptr<Edge> dNext(){return sym()->oNext->sym();}
    std::shared_ptr<Edge> dPrev(){return invRotf()->oNext->invRotf();}
    std::shared_ptr<Edge> lNext(){return invRotf()->oNext->rotf();}
    std::shared_ptr<Edge> lPrev(){return oNext->sym();}
    std::shared_ptr<Edge> rNext(){return rotf()->oNext->invRotf();}
    std::shared_ptr<Edge> rPrev(){return sym()->oNext;}

    // Visualizers
    static void displayPoints(std::vector<Vertex> s, cv::Mat& m);
    static void visualizeHull(std::pair<std::shared_ptr<Edge>,std::shared_ptr<Edge>> hull, cv::Mat m);

private:
    std::shared_ptr<Vertex> origin = nullptr;
    std::shared_ptr<Face> fac = nullptr;
    uint64_t ID;
    uint16_t index;
    static uint64_t nextID;


};

#endif // QUADEDGE_H
