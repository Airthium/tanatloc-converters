#include <catch2/catch.hpp>

#include "../../src/geometry/Vertex.hpp"

TEST_CASE("Vertex") {
  SECTION("Construtor 1") {
    Vertex vertex = Vertex();
    CHECK(vertex.X() == 0);
    CHECK(vertex.Y() == 0);
    CHECK(vertex.Z() == 0);
  }

  SECTION("Construtor 2") {
    double x = 1.;
    double y = 2.;
    double z = 3;
    Vertex vertex = Vertex(x, y, z);
    CHECK(vertex.X() == x);
    CHECK(vertex.Y() == y);
    CHECK(vertex.Z() == z);
  }

  SECTION("Vertex setCoordinates") {
    double x = 1.;
    double y = 2.;
    double z = 3.;
    Vertex vertex = Vertex();
    vertex.setX(x);
    vertex.setY(y);
    vertex.setZ(z);
    CHECK(vertex.X() == x);
    CHECK(vertex.Y() == y);
    CHECK(vertex.Z() == z);
  }
}
