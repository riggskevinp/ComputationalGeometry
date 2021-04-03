#include <iostream>
#include <string.h>

#include "delaunay.h"




int main(int argc, char *argv[])
{
    static std::pair<std::shared_ptr<Edge>,std::shared_ptr<Edge>> res;
    if(argc == 2){
        static std::vector<Vertex> input = Delaunay::readCSV(argv[1]);
        std::sort(input.begin(),input.end(), Vertex::compareVertices);
        std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
        res = Delaunay::divideAndConquer(input);
        // stop timer
        std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
        // display results
        std::cout << "Algorithm time for input size n = "
                  << input.size()
                  << ", "
                  << std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count()
                  << " seconds" << std::endl;
        Edge::traverseHull(res);
    } else {
        std::chrono::time_point<std::chrono::system_clock> startTime;
        std::chrono::time_point<std::chrono::system_clock> endTime;

        static std::vector<Vertex> input = Delaunay::generateRandomPoints(10000, "genTest10K");
        std::sort(input.begin(),input.end(), Vertex::compareVertices);
        startTime = std::chrono::system_clock::now();
        res = Delaunay::divideAndConquer(input);
        endTime = std::chrono::system_clock::now();
        std::cout << "Algorithm time for input size n = "
                  << input.size()
                  << ", "
                  << std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count()
                  << " seconds" << std::endl;

        input = Delaunay::generateRandomPoints(100000, "genTest100K");
        std::sort(input.begin(),input.end(), Vertex::compareVertices);
        startTime = std::chrono::system_clock::now();
        res = Delaunay::divideAndConquer(input);
        endTime = std::chrono::system_clock::now();
        std::cout << "Algorithm time for input size n = "
                  << input.size()
                  << ", "
                  << std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count()
                  << " seconds" << std::endl;

        input = Delaunay::generateRandomPoints(1000000, "genTest1M");
        std::sort(input.begin(),input.end(), Vertex::compareVertices);
        startTime = std::chrono::system_clock::now();
        res = Delaunay::divideAndConquer(input);
        endTime = std::chrono::system_clock::now();
        std::cout << "Algorithm time for input size n = "
                  << input.size()
                  << ", "
                  << std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count()
                  << " seconds" << std::endl;

    }
    return 0;
}
