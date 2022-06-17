#include <catch2/catch.hpp>

#include "../../src/dxf/DXFConverter.hpp"

TEST_CASE("DXFConverter") {
  SECTION("Constructor") { auto dxfConverter = DXFConverter(); }

  SECTION("setInput") {
    auto dxfConverter = DXFConverter();

    dxfConverter.setInput("input");
  }

  SECTION("convert") {
    auto dxfConverter = DXFConverter();
    dxfConverter.setInput("./test/assets/pipe.dxf");
    dxfConverter.convert();
  }
}