#ifndef VERTEX_H
#define VERTEX_H

#include <utility>
#include <cstdint>
#include <vector>
#include <memory>

class Edge;

class Vertex
{
public:
    Vertex(){
        ID = nextID;
        nextID += 1;
    }

    Vertex(double x, double y){
        p.first = x;
        p.second = y;
        ID = nextID;
        nextID += 1;
    }

    double getX() const {return p.first;}
    void setX(double x){p.first = x;}
    double getY() const {return p.second;}
    void setY(double y){p.second = y;}
    uint64_t getID() const {return ID;}
    void addEdge(std::shared_ptr<Edge> e){edges.push_back(e);}



private:
    std::vector<std::shared_ptr<Edge>> edges;
    std::pair<double,double> p;
    uint64_t ID;
    static uint64_t nextID;

};


#endif // VERTEX_H
