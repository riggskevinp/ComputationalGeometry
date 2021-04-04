#ifndef VERTEX_H
#define VERTEX_H

#include <utility>
#include <cstdint>
#include <vector>
#include <memory>

#include <opencv2/core/types.hpp>

class Edge;

class Vertex
{
public:
    Vertex(){
        ID = nextID;
        nextID += 1;
    }

    Vertex(double x, double y){
        cp.x = x;
        cp.y = y;
        ID = nextID;
        nextID += 1;
    }

    double getX() const {return cp.x;}
    void setX(double x){cp.x = x;}
    double getY() const {return cp.y;}
    void setY(double y){cp.y = y;}
    uint64_t getID() const {return ID;}
    void addEdge(std::shared_ptr<Edge> e){edges.push_back(e);}
    cv::Point2d getPoint()const{return cp;}
    static bool compareVertices(const Vertex& a, const Vertex& b);
    static bool compareVerticesHoriz(const Vertex& a, const Vertex& b);


private:
    std::vector<std::shared_ptr<Edge>> edges;
    cv::Point2d cp;
    uint64_t ID;
    static uint64_t nextID;

};


#endif // VERTEX_H
