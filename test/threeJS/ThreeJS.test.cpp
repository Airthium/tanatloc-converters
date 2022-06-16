#include <catch2/catch.hpp>

#include "../../src/threeJS/ThreeJS.hpp"

TEST_CASE("ThreeJS") {
  SECTION("Constructor 1") { ThreeJS threejs = ThreeJS(); }

  SECTION("Constructor 2") {
    uint size = 3;
    auto vertices = std::vector<float>();
    for (uint i = 0; i < size; ++i)
      vertices.push_back((float)i);

    ThreeJS threejs = ThreeJS(vertices);
  }

  SECTION("Constructor 3") {
    uint vsize = 3;
    auto vertices = std::vector<float>();
    for (uint i = 0; i < vsize; ++i)
      vertices.push_back((float)i);

    uint nsize = 3;
    auto normals = std::vector<float>();
    for (uint i = 0; i < nsize; ++i)
      normals.push_back((float)i);

    ThreeJS threejs = ThreeJS(vertices, normals);
  }

  SECTION("Constructor 4") {
    uint vsize = 3;
    auto vertices = std::vector<float>();
    for (uint i = 0; i < vsize; ++i)
      vertices.push_back((float)i);

    uint nsize = 3;
    auto normals = std::vector<float>();
    for (uint i = 0; i < nsize; ++i)
      normals.push_back((float)i);

    uint isize = 3;
    auto indices = std::vector<uint>();
    for (uint i = 0; i < nsize; ++i)
      indices.push_back(i);

    ThreeJS threejs = ThreeJS(vertices, normals, indices);
  }

  SECTION("setName") {
    ThreeJS threejs = ThreeJS();
    threejs.setName("name");
  }

  SECTION("setLabel") {
    ThreeJS threejs = ThreeJS();
    threejs.setLabel(0);
  }

  SECTION("setVertices") {
    ThreeJS threejs = ThreeJS();

    uint vsize = 3;
    auto vertices = std::vector<float>();
    for (uint i = 0; i < vsize; ++i)
      vertices.push_back((float)i);
    threejs.setVertices(vertices);

    threejs.setVertices(vertices);
  }

  SECTION("setNormals") {
    ThreeJS threejs = ThreeJS();

    uint nsize = 3;
    auto normals = std::vector<float>();
    for (uint i = 0; i < nsize; ++i)
      normals.push_back((float)i);
    threejs.setNormals(normals);

    threejs.setNormals(normals);
  }

  SECTION("setIndices") {
    ThreeJS threejs = ThreeJS();

    uint isize = 3;
    auto indices = std::vector<uint>();
    for (uint i = 0; i < isize; ++i)
      indices.push_back(i);
    threejs.setIndices(indices);

    threejs.setIndices(indices);
  }

  SECTION("setColors") {
    ThreeJS threejs = ThreeJS();

    uint csize = 3;
    auto colors = std::vector<Color>();
    for (uint i = 0; i < csize; ++i) {
      Color c;
      colors.push_back(c);
    }
    threejs.setColors(colors);

    threejs.setColors(colors);
  }

  SECTION("setData") {
    ThreeJS threejs = ThreeJS();

    uint dsize = 3;
    auto data = std::vector<float>();
    for (uint i = 0; i < dsize; ++i) {
      data.push_back((float)i);
    }
    threejs.setData(data);

    threejs.setData(data);
  }

  SECTION("save") {
    ThreeJS threejs = ThreeJS();

    // No file
    threejs.save("");

    // File
    threejs.save("threejstest.json");

    // With label
    threejs.setLabel(1);
    threejs.save("mythreejstest.json");

    // + With vertices
    uint vsize = 3;
    auto vertices = std::vector<float>();
    for (uint i = 0; i < vsize; ++i)
      vertices.push_back((float)i);
    threejs.setVertices(vertices);
    threejs.save("mythreejstest.json");

    // + With normals
    uint nsize = 3;
    auto normals = std::vector<float>();
    for (uint i = 0; i < nsize; ++i)
      normals.push_back((float)i);
    threejs.setNormals(normals);
    threejs.save("mythreejstest.json");

    // + With colors
    uint csize = 3;
    auto colors = std::vector<Color>();
    for (uint i = 0; i < csize; ++i) {
      Color c;
      colors.push_back(c);
    }
    threejs.setColors(colors);
    threejs.save("mythreejstest.json");

    // + With data
    uint dsize = 1;
    auto data = std::vector<float>();
    for (uint i = 0; i < dsize; ++i) {
      data.push_back((float)i);
    }
    threejs.setData(data);
    threejs.save("mythreejstest.json");

    // + With data (vertices size)
    uint dsize2 = 3;
    auto data2 = std::vector<float>();
    for (uint i = 0; i < dsize2; ++i) {
      data2.push_back((float)i);
    }
    threejs.setData(data2);
    threejs.save("mythreejstest.json");

    // + With indices
    uint isize = 3;
    auto indices = std::vector<uint>();
    for (uint i = 0; i < isize; ++i)
      indices.push_back(i);
    threejs.setIndices(indices);
    threejs.save("mythreejstest.json");
  }

  SECTION("writePartFile") {
    ThreeJS threejs = ThreeJS();

    // No file
    threejs.writePartFile("", "type", 0, 0, 0);

    // File
    threejs.writePartFile("threejstest.json", "type", 0, 0, 0);

    // With elements
    threejs.writePartFile("threejstest.json", "type", 2, 2, 2);
  }
}
