#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <utility>
#include <cstdint>
#include <vector>
#include <memory>

#include "quadedge.h"
#include "csv.h"

class Delaunay
{
public:
    Delaunay();
    static std::pair<std::shared_ptr<Edge>,std::shared_ptr<Edge>> divideAndConquer(std::vector<Vertex> s);
    static std::vector<Vertex> readCSV(std::string filename);
    static std::vector<Vertex> generateRandomPoints(uint64_t count, std::string filename);
    static std::pair<std::shared_ptr<Edge>,std::shared_ptr<Edge>> divideAndConquerAlt(std::vector<Vertex> s, bool even);
    static std::shared_ptr<Edge> locate(const Vertex& x, std::shared_ptr<Edge> e);
};

#endif // DELAUNAY_H
