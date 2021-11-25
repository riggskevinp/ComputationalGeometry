#include "delaunay.h"

#include <iostream>
#include <fstream>
#include <random>

Delaunay::Delaunay()
{

}

std::pair<std::shared_ptr<Edge>, std::shared_ptr<Edge>> Delaunay::divideAndConquer(std::vector<Vertex> s)
{
    std::pair<std::shared_ptr<Edge>, std::shared_ptr<Edge>> res;


    if(s.size() == 2){
        auto a = Edge::makeEdge();
        a->setOrg(std::make_shared<Vertex>(s.at(0)));
        a->setDest(std::make_shared<Vertex>(s.at(1)));
        res.first = a;
        res.second = a->sym();
        return res;
    }else if(s.size() == 3){
        // create edges a connecting s1 to s2 and b connecting s2 to s3
        auto a = Edge::makeEdge();
        auto b = Edge::makeEdge();
        Edge::splice(a->sym(),b);

        a->setOrg(std::make_shared<Vertex>(s.at(0)));
        b->setOrg(std::make_shared<Vertex>(s.at(1)));
        a->setDest(b->getOrg());
        b->setDest(std::make_shared<Vertex>(s.at(2)));

        // Close the triangle
        if(Edge::CCW(s.at(0), s.at(1), s.at(2)) >= 0){
            auto c = Edge::connect(b,a);
            res.first = a;
            res.second = b->sym();
            return res;
        }else if(Edge::CCW(s.at(0), s.at(2), s.at(1)) >= 0){
            auto c = Edge::connect(b,a);
            res.first = c->sym();
            res.second = c;
            return res;
        }else{
            res.first = a;
            res.second = b->sym();
            return res;
        }
    } else{
        // Split S in half
        std::size_t const half_size = s.size() / 2;
        std::vector<Vertex> leftHalf(s.begin(), s.begin() + half_size);
        std::vector<Vertex> rightHalf(s.begin() + half_size, s.end());
        // [ldo,ldi] = Delaunay[L]; [rdi,rdo] = Delaunay[R]
        std::pair<std::shared_ptr<Edge>, std::shared_ptr<Edge>> left = divideAndConquer(leftHalf);
        std::pair<std::shared_ptr<Edge>, std::shared_ptr<Edge>> right = divideAndConquer(rightHalf);
        std::shared_ptr<Edge> ldo = left.first;
        std::shared_ptr<Edge> ldi = left.second;
        std::shared_ptr<Edge> rdi = right.first;
        std::shared_ptr<Edge> rdo = right.second;
        while(true){
            // Compute the lower common tangent of L and R
            if(Edge::leftOf(*(rdi->getOrg()), *(ldi)) > 0){
                // leftOf(rdi.Org, ldi) then ldi = ldi.Lnext
                ldi = ldi->lNext();
            } else if(Edge::rightOf(*(ldi->getOrg()), *(rdi)) > 0){
                rdi = rdi->rPrev();
            } else{
                //exit
                // does this mean break? continue?
                break;
            }
        }

        // Create a first cross edge base1 from rdi.Org to ldi.Org
        auto base1 = Edge::connect(rdi->sym(), ldi);
        if(ldi->getOrg()->getID() == ldo->getOrg()->getID()){
            //*(ldo) = *(base1->sym());
            ldo = base1->sym();
        }
        if(rdi->getOrg()->getID() == rdo->getOrg()->getID()){
            //*(rdo) = *(base1);
            rdo = base1;
        }

        while(true){
            // Merge Loop
            // Locate the first L point (lcand.Dest) to be encountered by the rising bubble,
            // and delete L edges out of base1.Dest that fail the circle test.
            auto lcand = base1->sym()->oNext;
            if(Edge::valid(*(lcand), *(base1)) > 0){
                while(Edge::inCircle(*(base1->getDest()), *(base1->getOrg()), *(lcand->getDest()), *(lcand->oNext->getDest())) > 0){
                      auto t = lcand->oNext;
                      Edge::deleteEdge(lcand);
                      *(lcand) = *(t);
                }
            }
            // Symmetrically, locate the first R point to be hit, and delete R edges
            auto rcand = base1->oPrev();
            if(Edge::valid(*(rcand),*(base1)) > 0){
                while(Edge::inCircle(*(base1->getDest()), *(base1->getOrg()), *(rcand->getDest()), *(rcand->oPrev()->getDest())) > 0){
                    auto t = rcand->oPrev();
                    Edge::deleteEdge(rcand);
                    *(rcand) = *(t);
                }
            }
            // If both lcand and rcand are invalid, then base1 is the upper common tangent:
            if((Edge::valid(*(lcand), *(base1)) <= 0) && (Edge::valid(*(rcand), *(base1)) <= 0)){
                //exit?
                break;
            }
            // The next cross edge is to be connected to either lcand.Dest or rcand.Dest
            // If both are valid, then choose the appropriate on using the InCircle test
            if((Edge::valid(*(lcand), *(base1)) <= 0) ||
                    ((Edge::valid(*(lcand), *(base1)) > 0) && Edge::inCircle(*(lcand->getDest()), *(lcand->getOrg()), *(rcand->getOrg()), *(rcand->getDest())) > 0)){
                // Add cross edge base1 from rcand.Dest to base1.Dest
                base1 = Edge::connect(rcand,base1->sym());
            } else{
                // Add cross edge base1 from base1.org to lcand.Dest
                base1 = Edge::connect(base1->sym(), lcand->sym());
            }
        }
        res.first.swap(ldo);
        res.second.swap(rdo);
        return res;
    }

    return res;
}

std::vector<Vertex> Delaunay::readCSV(std::string filename)
{
    io::CSVReader<2> in(filename);
    in.read_header(io::ignore_extra_column, "x", "y");
    double x, y;
    std::vector<Vertex> S;

    while(in.read_row(x, y)){
        S.push_back(Vertex(x,y));
    }
    return S;
}

std::vector<Vertex> Delaunay::generateRandomPoints(uint64_t count, std::string filename)
{
    std::vector<Vertex> S;
    std::ofstream oFile;
    oFile.open(filename);
    oFile << "x,y" << std::endl;

    std::uniform_real_distribution<double> uniformDouble(0, 500);
    std::default_random_engine dre;

    for(uint64_t i = 0; i < count; i++){
        double x = uniformDouble(dre);
        double y = uniformDouble(dre);
        S.push_back(Vertex(x,y));
        oFile << x << "," << y << std::endl;
    }
    oFile.close();
    return S;

}

std::pair<std::shared_ptr<Edge>, std::shared_ptr<Edge> > Delaunay::divideAndConquerAlt(std::vector<Vertex> s, bool even)
{
    std::pair<std::shared_ptr<Edge>, std::shared_ptr<Edge>> res;


    if(s.size() == 2){

        if(even){
            std::sort(s.begin(),s.end(), Vertex::compareVertices);
        } else{
            std::sort(s.begin(),s.end(), Vertex::compareVerticesHoriz);
        }
        auto a = Edge::makeEdge();
        a->setOrg(std::make_shared<Vertex>(s.at(0)));
        a->setDest(std::make_shared<Vertex>(s.at(1)));
        res.first = a;
        res.second = a->sym();
        return res;
    }else if(s.size() == 3){
        if(even){
            std::sort(s.begin(),s.end(), Vertex::compareVertices);
        } else{
            std::sort(s.begin(),s.end(), Vertex::compareVerticesHoriz);
        }
        // create edges a connecting s1 to s2 and b connecting s2 to s3
        auto a = Edge::makeEdge();
        auto b = Edge::makeEdge();
        Edge::splice(a->sym(),b);

        a->setOrg(std::make_shared<Vertex>(s.at(0)));
        b->setOrg(std::make_shared<Vertex>(s.at(1)));
        a->setDest(b->getOrg());
        b->setDest(std::make_shared<Vertex>(s.at(2)));

        // Close the triangle
        if(Edge::CCW(s.at(0), s.at(1), s.at(2)) > 0){
            auto c = Edge::connect(b,a);
            res.first = a;
            res.second = b->sym();
            return res;
        }else if(Edge::CCW(s.at(0), s.at(2), s.at(1)) > 0){
            auto c = Edge::connect(b,a);
            res.first = c->sym();
            res.second = c;
            return res;
        }else{
            res.first = a;
            res.second = b->sym();
            return res;
        }
    } else{
        std::size_t const half_size = s.size() / 2;
        // https://www.geeksforgeeks.org/stdnth_element-in-cpp/
        if(even){
            //std::sort(s.begin(),s.end(), Vertex::compareVertices);
            std::nth_element(s.begin(), s.begin() + half_size, s.end(), Vertex::compareVertices);
        } else{
            //std::sort(s.begin(),s.end(), Vertex::compareVerticesHoriz);
            std::nth_element(s.begin(), s.begin() + half_size, s.end(), Vertex::compareVerticesHoriz);
        }


        // Split S in half
        std::vector<Vertex> leftHalf(s.begin(), s.begin() + half_size);
        std::vector<Vertex> rightHalf(s.begin() + half_size, s.end());
        // [ldo,ldi] = Delaunay[L]; [rdi,rdo] = Delaunay[R]
        std::pair<std::shared_ptr<Edge>, std::shared_ptr<Edge>> left = divideAndConquerAlt(leftHalf,!even);
        std::pair<std::shared_ptr<Edge>, std::shared_ptr<Edge>> right = divideAndConquerAlt(rightHalf, !even);
        std::shared_ptr<Edge> ldo = left.first;
        std::shared_ptr<Edge> ldi = left.second;
        std::shared_ptr<Edge> rdi = right.first;
        std::shared_ptr<Edge> rdo = right.second;

        if(even){
            //Adjust ldo/ldi/rdi/rdo

            while(Vertex::compareVertices(*(ldo->rPrev()->getOrg()),*(ldo->getOrg()))){
                ldo = ldo->rPrev();
            }
            while(Vertex::compareVertices(*(ldi->getOrg()), *(ldi->lPrev()->getOrg()))){
                ldi = ldi->lPrev();
            }
            while(Vertex::compareVertices(*(rdi->rPrev()->getOrg()), *(rdi->getOrg()))){
                rdi = rdi->rPrev();
            }
            while(Vertex::compareVertices(*(rdo->getOrg()), *(rdo->lPrev()->getOrg()))){
                rdo = rdo->lPrev();
            }

        }else{
            //Adjust ldo/ldi/rdi/rdo
            while(Vertex::compareVerticesHoriz(*(ldo->rNext()->getOrg()),*(ldo->getOrg()))){
                ldo = ldo->rNext();
            }
            while(Vertex::compareVerticesHoriz(*(ldi->getOrg()), *(ldi->lNext()->getOrg()))){
                ldi = ldi->lNext();
            }
            while(Vertex::compareVerticesHoriz(*(rdi->rNext()->getOrg()), *(rdi->getOrg()))){
                rdi = rdi->rNext();
            }
            while(Vertex::compareVerticesHoriz(*(rdo->getOrg()), *(rdo->lNext()->getOrg()))){
                rdo = rdo->lNext();
            }
        }

        while(true){
            // Compute the lower common tangent of L and R
            if(Edge::leftOf(*(rdi->getOrg()), *(ldi)) > 0){
                // leftOf(rdi.Org, ldi) then ldi = ldi.Lnext
                ldi = ldi->lNext();
            } else if(Edge::rightOf(*(ldi->getOrg()), *(rdi)) > 0){
                rdi = rdi->rPrev();
            } else{
                //exit
                // does this mean break? continue?
                break;
            }
        }

        // Create a first cross edge base1 from rdi.Org to ldi.Org
        auto base1 = Edge::connect(rdi->sym(), ldi);
        if(ldi->getOrg()->getID() == ldo->getOrg()->getID()){
            //*(ldo) = *(base1->sym());
            ldo = base1->sym();
        }
        if(rdi->getOrg()->getID() == rdo->getOrg()->getID()){
            //*(rdo) = *(base1);
            rdo = base1;
        }

        while(true){
            // Merge Loop
            // Locate the first L point (lcand.Dest) to be encountered by the rising bubble,
            // and delete L edges out of base1.Dest that fail the circle test.
            auto lcand = base1->sym()->oNext;
            if(Edge::valid(*(lcand), *(base1)) > 0){
                while(Edge::inCircle(*(base1->getDest()), *(base1->getOrg()), *(lcand->getDest()), *(lcand->oNext->getDest())) > 0){
                      auto t = lcand->oNext;
                      Edge::deleteEdge(lcand);
                      *(lcand) = *(t);
                }
            }
            // Symmetrically, locate the first R point to be hit, and delete R edges
            auto rcand = base1->oPrev();
            if(Edge::valid(*(rcand),*(base1)) > 0){
                while(Edge::inCircle(*(base1->getDest()), *(base1->getOrg()), *(rcand->getDest()), *(rcand->oPrev()->getDest())) > 0){
                    auto t = rcand->oPrev();
                    Edge::deleteEdge(rcand);
                    *(rcand) = *(t);
                }
            }
            // If both lcand and rcand are invalid, then base1 is the upper common tangent:
            if((Edge::valid(*(lcand), *(base1)) <= 0) && (Edge::valid(*(rcand), *(base1)) <= 0)){
                //exit?
                break;
            }
            // The next cross edge is to be connected to either lcand.Dest or rcand.Dest
            // If both are valid, then choose the appropriate on using the InCircle test
            if((Edge::valid(*(lcand), *(base1)) <= 0) ||
                    ((Edge::valid(*(lcand), *(base1)) > 0) && Edge::inCircle(*(lcand->getDest()), *(lcand->getOrg()), *(rcand->getOrg()), *(rcand->getDest())) > 0)){
                // Add cross edge base1 from rcand.Dest to base1.Dest
                base1 = Edge::connect(rcand,base1->sym());
            } else{
                // Add cross edge base1 from base1.org to lcand.Dest
                base1 = Edge::connect(base1->sym(), lcand->sym());
            }
        }

        res.first.swap(ldo);
        res.second.swap(rdo);
        return res;
    }

    return res;
}

std::shared_ptr<Edge> Delaunay::locate(const Vertex& x, std::shared_ptr<Edge> e)
{
    while(true){

        if(e->getOrg()->getID() == x.getID() || x.getID() == e->getDest()->getID()){
            return e;
        } else if(Edge::rightOf(x, *(e)) > 0.0){
            e = e->sym();
        } else if(Edge::rightOf(x, *(e->oNext)) < 0.0){
            e = e->oNext;
        } else if(Edge::rightOf(x,*(e->dPrev())) < 0.0){
            e = e->dPrev();
        } else{
            return e;
        }
    }
}
