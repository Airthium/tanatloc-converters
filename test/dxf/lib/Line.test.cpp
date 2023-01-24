#include <catch2/catch.hpp>

#include "../../../src/dxf/lib/Line.hpp"

TEST_CASE("DXFLine") {
  SECTION("Constructor") {
    auto line = DXFLine();
    CHECK(line.x1 == 0.);
  }

  SECTION("In X-Y") {
    auto line = DXFLine();
    CHECK(line.inXY());
  }

  SECTION("Equal operator") {
    auto line1 = DXFLine();
    auto line2 = DXFLine();

    CHECK(line1 == line2);
  }
}