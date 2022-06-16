#include <catch2/catch.hpp>

#include "../../src/dxf/DXFConverter.hpp"

TEST_CASE("DXFConverter") {
  SECTION("Constructor") { DXFConverter dxfConverter = DXFConverter(); }

  SECTION("setInput") {
    DXFConverter dxfConverter = DXFConverter();

    dxfConverter.setInput("input");
  }

  SECTION("convert") {
    DXFConverter dxfConverter = DXFConverter();
    dxfConverter.setInput("./test/assets/pipe.dxf");
    dxfConverter.convert();
  }
}