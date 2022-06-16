#include <catch2/catch.hpp>

#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Solid.hxx>

#include "../../src/occ/Triangulation.hpp"

TEST_CASE("Triangulation") {
  SECTION("Constructor") { Triangulation triangulation = Triangulation(); }
}