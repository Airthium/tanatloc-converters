#include <catch2/catch.hpp>

#include "../../../src/dxf/lib/Polyline.hpp"

TEST_CASE("DXFPolyline") {
  SECTION("Constructor") {
    auto line = DXFPolyline();
    CHECK(line.vertices.size() == 0);
  }

  SECTION("In X-Y - empty") {
    auto polyline = DXFPolyline();

    CHECK(polyline.inXY());
  }

  SECTION("In X-Y - false") {
    auto polyline = DXFPolyline();

    auto vertex1 = DXFVertex();
    vertex1.x = 0.;
    vertex1.y = 0.;
    vertex1.z = 1.;
    auto vertex2 = DXFVertex();
    vertex2.x = 1.;
    vertex2.y = 0.;
    vertex2.z = 0.;

    polyline.vertices.push_back(vertex1);
    polyline.vertices.push_back(vertex2);

    CHECK(polyline.inXY() == false);
  }

  SECTION("In X-Y - true") {
    auto polyline = DXFPolyline();

    auto vertex1 = DXFVertex();
    vertex1.x = 0.;
    vertex1.y = 0.;
    vertex1.z = 1.;
    auto vertex2 = DXFVertex();
    vertex2.x = 1.;
    vertex2.y = 0.;
    vertex2.z = 1.;

    polyline.vertices.push_back(vertex1);
    polyline.vertices.push_back(vertex2);

    CHECK(polyline.inXY() == true);
  }

  SECTION("Equal operator") {
    auto polyline1 = DXFPolyline();
    auto polyline2 = DXFPolyline();

    CHECK(polyline1 == polyline2);
  }
}