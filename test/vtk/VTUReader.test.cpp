#include <catch2/catch.hpp>

#include "../../src/vtk/VTUReader.hpp"

TEST_CASE("VTUReader") {
  SECTION("Constructor 1") { auto reader = VTUReader(); }

  SECTION("Constructor 2") { auto reader = VTUReader("filename"); }

  SECTION("getArrays") {
    auto reader = VTUReader();

    std::vector<VTUData> arrays = reader.getArrays();
    CHECK(arrays.size() == 0);
  }

  SECTION("read") {
    auto reader = VTUReader("../test/assets/Result.vtu");
    reader.read();

    std::vector<VTUData> arrays = reader.getArrays();
    CHECK(arrays.size() == 2);
  }

  SECTION("read 2 pieces") {
    auto reader = VTUReader("../test/assets/Result2Pieces.vtu");
    reader.read();

    std::vector<VTUData> arrays = reader.getArrays();
    CHECK(arrays.size() == 1);
  }
}