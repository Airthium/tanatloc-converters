#include <catch2/catch.hpp>

#include "../../../src/dxf/lib/Polyline.hpp"

TEST_CASE("DXFPolyline") {
  SECTION("Constructor") {
    auto line = DXFPolyline();
    CHECK(line.vertices.size() == 0);
  }

  SECTION("Equal operator - empty") {
    auto polyline1 = DXFPolyline();
    auto polyline2 = DXFPolyline();

    CHECK(polyline1 == polyline2);
  }

  SECTION("Equal operator - false") {
    auto polyline1 = DXFPolyline();
    auto polyline2 = DXFPolyline();

    auto vertex1_1 = DXFVertex();
    vertex1_1.x = 1.;
    auto vertex1_2 = DXFVertex();
    vertex1_2.x = 2.;

    polyline1.vertices.push_back(vertex1_1);
    polyline2.vertices.push_back(vertex1_2);

    CHECK(!(polyline1 == polyline2));
  }

  SECTION("Equal operator - true") {
    auto polyline1 = DXFPolyline();
    auto polyline2 = DXFPolyline();

    auto vertex1_1 = DXFVertex();
    auto vertex1_2 = DXFVertex();

    polyline1.vertices.push_back(vertex1_1);
    polyline2.vertices.push_back(vertex1_2);

    CHECK(polyline1 == polyline2);
  }
}