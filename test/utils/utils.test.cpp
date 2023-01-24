#include <catch2/catch.hpp>

#include "../../src/utils/utils.hpp"

TEST_CASE("Utils") {
  SECTION("uuid") {
    auto uuid = Utils::uuid();
    CHECK(uuid.size());
  }

  SECTION("generateRandom") {
    auto random = Utils::generateRandom();
    CHECK(random);
  }

  SECTION("uintToBuffer") {
    auto buffer = std::vector<unsigned char>();
    Utils::uintToBuffer(1, buffer);

    CHECK(buffer.size());
  }

  SECTION("floatToBuffer") {
    auto buffer = std::vector<unsigned char>();
    Utils::floatToBuffer(1., buffer);

    CHECK(buffer.size());
  }

  SECTION("findIndex") {
    auto indices = std::vector<std::pair<uint, uint>>();
    int index = Utils::findIndex(1, indices);
    CHECK(index == -1);

    indices.push_back({1, 2});
    index = Utils::findIndex(1, indices);
    CHECK(index == 2);
  }

  SECTION("minMax - tetrahedron") {
    auto tetrahedra = std::vector<Tetrahedron>();
    Tetrahedron tetrahedron;
    tetrahedra.push_back(tetrahedron);

    auto mm = Utils::minMax(tetrahedra);
    CHECK(mm.at(0) == 0);
    CHECK(mm.at(1) == 0);
  }

  SECTION("minMax - triangle") {
    auto triangles = std::vector<Triangle>();
    Triangle triangle;
    triangles.push_back(triangle);

    auto mm = Utils::minMax(triangles);
    CHECK(mm.at(0) == 0);
    CHECK(mm.at(1) == 0);
  }

  SECTION("minMax - polygon") {
    auto polygons = std::vector<Polygon>();
    Polygon polygon;
    polygon.addIndex(1);
    polygons.push_back(polygon);

    auto mm = Utils::minMax(polygons);
    CHECK(mm.at(0) == 0);
    CHECK(mm.at(1) == 1);
  }

  SECTION("minMax - vertex") {
    auto vertices = std::vector<Vertex>();
    Vertex vertex;
    vertices.push_back(vertex);

    auto mm = Utils::minMax(vertices);
    CHECK(mm.at(0).X() == 0.);
    CHECK(mm.at(1).X() == 0.);
  }

  SECTION("minMax - values") {
    auto values = std::vector<double>();
    values.push_back(0.);
    values.push_back(1.);

    auto mm = Utils::minMax(values);
    CHECK(mm.at(0) == 0.);
    CHECK(mm.at(1) == 1.);
  }
}