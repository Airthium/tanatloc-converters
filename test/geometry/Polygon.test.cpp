#include <catch2/catch.hpp>

#include "../../src/geometry/Polygon.hpp"

TEST_CASE("Polygon") {
  SECTION("Constructor") {
    auto polygon = Polygon();
    polygon.addIndex(1);

    std::vector<uint> indices = polygon.getIndices();
    CHECK(indices[0] == 1);
  }
}
