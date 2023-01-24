#include <catch2/catch.hpp>

#include "../../../src/dxf/lib/Arc.hpp"

TEST_CASE("DXFArc") {
  SECTION("Constructor") {
    auto arc = DXFArc();
    CHECK(arc.startAngle == 0.);
  }

  SECTION("Equal operator") {
    auto arc1 = DXFArc();
    auto arc2 = DXFArc();

    CHECK(arc1 == arc2);
  }
}