// Add sources to test

#include "gtest/gtest.h"

#include "quadedge.h"
#include "delaunay.h"


// The fixture for testing class Foo.
class QuadTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  QuadTest() {
     // You can do set-up work for each test here.
  }

  ~QuadTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(QuadTest, TwoSitesOneEdge) {
  //const std::string input_filepath = "this/package/testdata/myinputfile.dat";
  //const std::string output_filepath = "this/package/testdata/myoutputfile.dat";
  //Foo f;
  //EXPECT_EQ(f.Bar(input_filepath, output_filepath), 0);
    auto v1 = std::make_shared<Vertex>(Vertex(2.0,3.0));
    auto v2 = std::make_shared<Vertex>(Vertex(4.0,5.0));


    auto a = Edge::makeEdge();
    a->setOrg(v1);
    a->setDest(v2);

    EXPECT_EQ(a->getOrg()->getID(),0);
    EXPECT_EQ(a->getDest()->getID(), 1);

}


TEST_F(QuadTest, ThreeSitesCCW) {
  // Sorted order
    auto v1 = std::make_shared<Vertex>(Vertex(2.0,3.0));
    auto v2 = std::make_shared<Vertex>(Vertex(4.0,3.0));
    auto v3 = std::make_shared<Vertex>(Vertex(4.0,5.0));

    // create edges a connecting s1 to s2 and b connecting s2 to s3
    auto a = Edge::makeEdge();
    auto b = Edge::makeEdge();
    Edge::splice(a->sym(),b);

    a->setOrg(v1);
    b->setOrg(v2);
    a->setDest(b->getOrg());
    b->setDest(v3);

    EXPECT_GT(Edge::CCW(*v1, *v2, *v3), 0);

    // Close the triangle
    if(Edge::CCW(*v1, *v2, *v3) > 0){
        auto c = Edge::connect(b,a);
        std::cout << "le origin " << a->getOrg()->getID() << " re origin " << b->sym()->getOrg()->getID() << std::endl;
        std::cout << "le destination " << a->getDest()->getID() << " re destination " << b->sym()->getDest()->getID() << std::endl;
    }


}

TEST_F(QuadTest, ThreeSitesCollinear) {
  // Sorted order
    auto v1 = std::make_shared<Vertex>(Vertex(2.0,3.0));
    auto v2 = std::make_shared<Vertex>(Vertex(4.0,5.0));
    auto v3 = std::make_shared<Vertex>(Vertex(6.0,7.0));

    // create edges a connecting s1 to s2 and b connecting s2 to s3
    auto a = Edge::makeEdge();
    auto b = Edge::makeEdge();
    Edge::splice(a->sym(),b);

    a->setOrg(v1);
    b->setOrg(v2);
    a->setDest(b->getOrg());
    b->setDest(v3);

    EXPECT_EQ(Edge::CCW(*v1, *v2, *v3), 0);

    // Close the triangle
    if(Edge::CCW(*v1, *v2, *v3) == 0){
        std::cout << "a origin " << a->getOrg()->getID() << " a destination " << a->getDest()->getID() << std::endl;
        std::cout << "b origin " << b->getOrg()->getID() << " b destination " << b->getDest()->getID() << std::endl;
    }


}

TEST_F(QuadTest, ThreeSitesCW) {
  // Sorted order
    auto v1 = std::make_shared<Vertex>(Vertex(4.0,5.0));
    auto v2 = std::make_shared<Vertex>(Vertex(4.0,3.0));
    auto v3 = std::make_shared<Vertex>(Vertex(2.0,3.0));

    // create edges a connecting s1 to s2 and b connecting s2 to s3
    auto a = Edge::makeEdge();
    auto b = Edge::makeEdge();
    Edge::splice(a->sym(),b);

    a->setOrg(v1);
    b->setOrg(v2);
    a->setDest(b->getOrg());
    b->setDest(v3);

    EXPECT_LT(Edge::CCW(*v1, *v2, *v3), 0);

    // Close the triangle and visualize
    if(Edge::CCW(*v1, *v2, *v3) < 0){
        auto c = Edge::connect(b,a);
        std::cout << "le origin " << c->sym()->getOrg()->getID() << " re origin " << c->getOrg()->getID() << std::endl;
        std::cout << "le destination " << c->sym()->getDest()->getID() << " re destination " << c->getDest()->getID() << std::endl;
    }
}

TEST_F(QuadTest, CCW) {
  // Sorted order
    auto v1 = std::make_shared<Vertex>(Vertex(1.0,1.0));
    auto v2 = std::make_shared<Vertex>(Vertex(2.0,1.0));
    auto v3 = std::make_shared<Vertex>(Vertex(2.0,2.0));

    EXPECT_GT(Edge::CCW(*v1, *v2, *v3), 0);
    EXPECT_LT(Edge::CCW(*v1, *v3, *v2), 0);
}

TEST_F(QuadTest, InCircle) {
  // Sorted order
    auto v1 = std::make_shared<Vertex>(Vertex(1.0,1.0));
    auto v2 = std::make_shared<Vertex>(Vertex(3.0,1.0));
    auto v3 = std::make_shared<Vertex>(Vertex(1.0,3.0));
    auto v4 = std::make_shared<Vertex>(Vertex(10.0,10.0));
    auto v5 = std::make_shared<Vertex>(Vertex(2.5,2.5));

    EXPECT_LT(Edge::inCircle(*v1, *v2, *v3, *v4), 0);
    EXPECT_GT(Edge::inCircle(*v1, *v3, *v2, *v4), 0);
    EXPECT_GT(Edge::inCircle(*v1, *v2, *v3, *v5), 0);
    EXPECT_LT(Edge::inCircle(*v1, *v3, *v2, *v5), 0);
}

TEST(Delaunay, readCSV){
    std::string filename = "test.csv";
    std::vector<Vertex> S = Delaunay::readCSV(filename);
    std::sort(S.begin(),S.end(), Vertex::compareVertices);
    for(auto v : S){
        std::cout << v.getX() << "," << v.getY() << std::endl;
    }
}

TEST(Delaunay, randPointsandGenCSV){
    std::string filename = "genTest.csv";
    std::vector<Vertex> S = Delaunay::generateRandomPoints(10,filename);
    for(auto v : S){
        std::cout << v.getX() << "," << v.getY() << std::endl;
    }
}

TEST(Delaunay, divideAndConquerA3){
    std::string filename = "genTest.csv";
    std::vector<Vertex> S = Delaunay::generateRandomPoints(3,filename);
    std::sort(S.begin(),S.end(), Vertex::compareVertices);
    std::pair<std::shared_ptr<Edge>,std::shared_ptr<Edge>> res = Delaunay::divideAndConquer(S);
}

TEST(Delaunay, divideAndConquerARand10){
    std::string filename = "genTest.csv";
    std::vector<Vertex> S = Delaunay::generateRandomPoints(10,filename);
    std::sort(S.begin(),S.end(), Vertex::compareVertices);
    std::pair<std::shared_ptr<Edge>,std::shared_ptr<Edge>> res = Delaunay::divideAndConquer(S);
    auto img = cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
    Edge::visualizeHull(res,img);
    Edge::displayPoints(S, img);
    cv::imwrite("DAndCARand10.jpg", img);
}

TEST(Delaunay, divideAndConquerAEdgeCase10){
    std::string filename = "test.csv";
    std::vector<Vertex> S = Delaunay::readCSV(filename);
    std::sort(S.begin(),S.end(), Vertex::compareVertices);
    std::pair<std::shared_ptr<Edge>,std::shared_ptr<Edge>> res = Delaunay::divideAndConquer(S);
}

TEST(Delaunay, divideAndConquerAltRand){
    std::string filename = "genTest.csv";
    std::vector<Vertex> S = Delaunay::generateRandomPoints(100,filename);
    std::sort(S.begin(),S.end(), Vertex::compareVertices);
    std::pair<std::shared_ptr<Edge>,std::shared_ptr<Edge>> res = Delaunay::divideAndConquerAlt(S, true);
    auto img = cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
    Edge::visualizeHull(res,img);
    Edge::displayPoints(S, img);
    cv::imwrite("DAndCAltRand.jpg", img);
}

TEST(Delaunay, locate){
    auto v1 = std::make_shared<Vertex>(Vertex(0.0,0.0));
    auto v2 = std::make_shared<Vertex>(Vertex(0.0,500.0));
    auto v3 = std::make_shared<Vertex>(Vertex(500.0,0.0));
    //auto v0 = std::make_shared<Vertex>(Vertex(400.0,430.0));
    auto v4 = std::make_shared<Vertex>(Vertex(500.0,500.0));
    std::vector<Vertex> S;
    S.push_back(*v1);
    S.push_back(*v4);
    S.push_back(*v3);
    S.push_back(*v2);
    //S.push_back(*v0);
    std::sort(S.begin(),S.end(), Vertex::compareVertices);
    std::pair<std::shared_ptr<Edge>,std::shared_ptr<Edge>> res = Delaunay::divideAndConquer(S);
    auto img = cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
    Edge::visualizeHull(res,img);
    auto v6 = std::make_shared<Vertex>(Vertex(10.0,5.0));
    auto v5 = std::make_shared<Vertex>(Vertex(280.0,290.0));
    S.push_back(*v6);
    S.push_back(*v5);
    Edge::displayPoints(S, img);
    cv::imwrite("locate.jpg", img);

    //std::cout << "edge: " << Delaunay::locate(*v6, res.first->sym())->getID() << std::endl;
    //std::cout << "edge: " << Delaunay::locate(*v5, res.first)->getID() << std::endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
