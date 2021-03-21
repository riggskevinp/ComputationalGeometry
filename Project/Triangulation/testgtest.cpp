// Add sources to test

#include "gtest/gtest.h"

#include "quadedge.h"


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
    std::cout << "Before creation" << std::endl;
    auto v1 = std::make_shared<Vertex>(Vertex(2.0,3.0));
    auto v2 = std::make_shared<Vertex>(Vertex(4.0,5.0));
    std::cout << "After vertex before edges" << std::endl;

    auto a = Edge::makeEdge();
    std::cout << "After makeEdge before setOrg set Dest" << std::endl;
    a->setOrg(v1);
    a->setDest(v2);

    EXPECT_EQ(a->getOrg()->getID(),0);
    EXPECT_EQ(a->getDest()->getID(), 1);

}

// Tests that Foo does Xyz.
TEST_F(QuadTest, ThreeSites) {
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

    // Close the triangle


}



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
