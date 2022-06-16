#include <catch2/catch.hpp>

#include "../../src/logger/Logger.hpp"

TEST_CASE("Logger") {
  SECTION("DEBUG") { Logger::DEBUG("debug"); }

  SECTION("LOG") { Logger::LOG("log"); }

  SECTION("DISP") { Logger::DISP("info"); }

  SECTION("WARNING") { Logger::WARNING("warning"); }

  SECTION("ERROR") { Logger::ERROR("error"); }
}
