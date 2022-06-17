#include <catch2/catch.hpp>

#include "../../src/gmsh/Gmsh.hpp"

TEST_CASE("Gmsh") {

  SECTION("Constructor") { auto mesh = Gmsh(); }

  SECTION("load - no file") {
    auto mesh = Gmsh();

    bool res = mesh.load("");
    CHECK(!res);
  }

  SECTION("load - wrong file") {
    auto mesh = Gmsh();

    bool res = mesh.load("../test/assets/Cube.wrong.msh");
    CHECK(res);
  }

  SECTION("load - correct file") {
    auto mesh = Gmsh();

    bool res = mesh.load("../test/assets/Cube.msh");
    CHECK(res);
  }

  SECTION("load - no vertices") {
    auto mesh = Gmsh();

    bool res = mesh.load("../test/assets/Cube.no_vertices.msh");
    CHECK(!res);
  }

  SECTION("load - no elements") {
    auto mesh = Gmsh();

    bool res = mesh.load("../test/assets/Cube.no_elements.msh");
    CHECK(!res);
  }

  SECTION("load - no triangles") {
    auto mesh = Gmsh();

    bool res = mesh.load("../test/assets/Cube.no_triangles.msh");
    CHECK(res);
  }

  SECTION("load - 2D") {
    auto mesh = Gmsh();

    bool res = mesh.load("../test/assets/Square.msh");
    CHECK(res);
  }

  SECTION("load - 1D") {
    auto mesh = Gmsh();

    bool res = mesh.load("../test/assets/Point.msh");
    CHECK(res);
  }
}
