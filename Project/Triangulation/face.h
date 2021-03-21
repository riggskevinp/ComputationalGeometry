#ifndef FACE_H
#define FACE_H

#include <cstdint>
#include <vector>
#include <memory>

class Edge;

class Face
{
public:
    Face(){
        ID = nextID;
        nextID += 1;
    }

    void addEdge(std::shared_ptr<Edge> e){edges.push_back(e);}

private:
    std::vector<std::shared_ptr<Edge>> edges;
    uint64_t ID;
    static uint64_t nextID;
};


#endif // FACE_H
