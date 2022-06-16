#include <catch2/catch.hpp>

#include <TDocStd_Document.hxx>
#include <TopoDS_Shape.hxx>
#include <vector>

#include "../../src/occ/StepReader.hpp"

TEST_CASE("StepReader") {
  SECTION("Constructor 1") { StepReader stepReader = StepReader(); }

  SECTION("Constructor 2") {
    std::string fileName = "fileName";
    auto stepReader = StepReader("fileName");
  }

  SECTION("read - no file") {
    std::string fileName = "non_existing_file";
    auto stepReader = StepReader(fileName);

    bool res = stepReader.read();

    CHECK(!res);
  }

  SECTION("read - empty") {
    std::string fileName = "../test/assets/empty.step";
    auto stepReader = StepReader(fileName);

    bool res = stepReader.read();

    CHECK(!res);
  }

  SECTION("read - file") {
    std::string fileName = "../test/assets/cube.step";
    auto stepReader = StepReader(fileName);

    bool res = stepReader.read();

    CHECK(res);
  }

  // SECTION("getDocument") {
  //   auto stepReader = StepReader();

  //   stepReader.getMainDocument();
  // }
}