#include <catch2/catch.hpp>

#include "../../src/geometry/Triangle.hpp"

TEST_CASE("Triangle") {
  SECTION("Constructor 1") {
    Triangle triangle = Triangle();
    CHECK(triangle.I1() == 0);
    CHECK(triangle.I2() == 0);
    CHECK(triangle.I3() == 0);

    std::vector<uint> indices = triangle.getIndices();
    CHECK(indices[0] == 0);
    CHECK(indices[1] == 0);
    CHECK(indices[2] == 0);

    CHECK(triangle.Label() == 0);
  }

  SECTION("Constructor 2") {
    uint index1 = 1;
    uint index2 = 2;
    uint index3 = 3;
    uint label = 4;
    Triangle triangle = Triangle(index1, index2, index3, label);
    CHECK(triangle.I1() == index1);
    CHECK(triangle.I2() == index2);
    CHECK(triangle.I3() == index3);

    std::vector<uint> indices = triangle.getIndices();
    CHECK(indices[0] == index1);
    CHECK(indices[1] == index2);
    CHECK(indices[2] == index3);

    CHECK(triangle.Label() == label);
  }

  SECTION("setIndices") {
    uint index1 = 1;
    uint index2 = 2;
    uint index3 = 3;
    Triangle triangle = Triangle();
    triangle.setI1(index1);
    triangle.setI2(index2);
    triangle.setI3(index3);
    CHECK(triangle.I1() == index1);
    CHECK(triangle.I2() == index2);
    CHECK(triangle.I3() == index3);

    std::vector<uint> indices = triangle.getIndices();
    CHECK(indices[0] == index1);
    CHECK(indices[1] == index2);
    CHECK(indices[2] == index3);

    CHECK(triangle.Label() == 0);
  }

  SECTION("Triangle setLabel") {
    uint label = 4;
    Triangle triangle = Triangle();
    triangle.setLabel(label);
    CHECK(triangle.I1() == 0);
    CHECK(triangle.I2() == 0);
    CHECK(triangle.I3() == 0);

    std::vector<uint> indices = triangle.getIndices();
    CHECK(indices[0] == 0);
    CHECK(indices[1] == 0);
    CHECK(indices[2] == 0);

    CHECK(triangle.Label() == label);
  }
}
