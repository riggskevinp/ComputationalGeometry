#include "vertex.h"


uint64_t Vertex::nextID = 0;

bool Vertex::compareVertices(const Vertex& a, const Vertex& b)
{
    if(a.getX() != b.getX()){
        return a.getX() < b.getX();
    } else{
        return a.getY() < b.getY();
    }
}
