#include <catch2/catch.hpp>

#include "../../../src/dxf/lib/Vertex.hpp"

TEST_CASE("DXFVertex") {
  SECTION("Constructor") {
    auto arc = DXFVertex();
    CHECK(arc.x == 0.);
  }

  SECTION("Equal operator") {
    auto vertex1 = DXFVertex();
    auto vertex2 = DXFVertex();

    CHECK(vertex1 == vertex2);
  }
}