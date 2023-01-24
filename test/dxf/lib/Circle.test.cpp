#include <catch2/catch.hpp>

#include "../../../src/dxf/lib/Circle.hpp"

TEST_CASE("DXFCircle") {
  SECTION("Constructor") {
    auto circle = DXFCircle();
    CHECK(circle.x == 0.);
  }

  SECTION("Equal operator") {
    auto circle1 = DXFCircle();
    auto circle2 = DXFCircle();

    CHECK(circle1 == circle2);
  }
}