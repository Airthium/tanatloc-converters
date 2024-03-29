cmake_minimum_required(VERSION 3.5)

project(Tanatloc)

# # Debug
option(DEBUG "Print debug informations" OFF)

if(DEBUG)
  add_definitions(-DDEBUG_MODE)
else()
  remove_definitions(-DDEBUG_MODE)
endif()

# # Build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-O3")

# # External libraries
# VTK
find_package(VTK COMPONENTS
  IOXML
  REQUIRED)

if(VTK_FOUND)
  include_directories(${VTK_INCLUDE_DIRS})
  set(LIBS ${LIBS} ${VTK_LIBRARY_DIRS})
  set(VTK_LIBS ${VTK_LIBRARIES})
endif(VTK_FOUND)

# Catch2
find_package(Catch2 REQUIRED)

# OpenCASCADE
find_package(OpenCASCADE REQUIRED)

if(OpenCASCADE_FOUND)
  include_directories(${OpenCASCADE_INCLUDE_DIR})
  set(LIBS ${LIBS} ${OpenCASCADE_LIBRARY_DIR})
  set(OCC_LIBS ${OpenCASCADE_LIBRARIES})
endif(OpenCASCADE_FOUND)

# UUID
find_path(LIBUUID_INCLUDE_DIR uuid.h PATH_SUFFIXES uuid)
find_library(LIBUUID_LIBS libuuid.a)
include_directories(${LIBUUID_INCLUDE_DIR})

link_libraries(${VTK_LIBS} ${OCC_LIBS} ${LIBUUID_LIBS})

# TinyGLTF
find_package(TinyGLTF REQUIRED)

if(TinyGLTF_FOUND)
  include_directories(${TinyGLTF_INCLUDE_DIR})
endif(TinyGLTF_FOUND)

# # Libraries
# DXF
AUX_SOURCE_DIRECTORY(src/dxf DXF_SOURCE)
AUX_SOURCE_DIRECTORY(src/dxf/lib DXF_LIB_SOURCE)

# Geometry
AUX_SOURCE_DIRECTORY(src/geometry GEOMETRY_SOURCE)

# Gmsh
AUX_SOURCE_DIRECTORY(src/gmsh GMSH_SOURCE)

# Logger
AUX_SOURCE_DIRECTORY(src/logger LOGGER_SOURCE)

# Utils
AUX_SOURCE_DIRECTORY(src/utils UTILS_SOURCE)

# OCC
AUX_SOURCE_DIRECTORY(src/occ OCC_SOURCE)

# VTK
AUX_SOURCE_DIRECTORY(src/vtk VTK_SOURCE)

# # Sources
# DXFToGLTF
set(DXFToGLTFSOURCE
  src/DXFToGLTF.cpp
  ${DXF_SOURCE}
  ${DXF_LIB_SOURCE}
  ${GEOMETRY_SOURCE}
  ${LOGGER_SOURCE}
  ${OCC_SOURCE}
  ${UTILS_SOURCE}
)

# StepToGLTF
set(StepToGLTFSOURCE
  src/StepToGLTF.cpp
  ${GEOMETRY_SOURCE}
  ${LOGGER_SOURCE}
  ${OCC_SOURCE}
  ${UTILS_SOURCE}
)

# StepSplit
set(StepSplitSOURCE
  src/StepSplit.cpp
  ${GEOMETRY_SOURCE}
  ${LOGGER_SOURCE}
  ${OCC_SOURCE}
  ${UTILS_SOURCE}
)

# GmshToGLTF
set(GmshToGLTFSOURCE
  src/GmshToGLTF.cpp
  ${GEOMETRY_SOURCE}
  ${GMSH_SOURCE}
  ${LOGGER_SOURCE}
  ${UTILS_SOURCE}
)

# VTUToGLTF
set(VTUToGLTFSOURCE
  src/VTUToGLTF.cpp
  ${GEOMETRY_SOURCE}
  ${LOGGER_SOURCE}
  ${OCC_SOURCE}
  ${UTILS_SOURCE}
  ${VTK_SOURCE}
)

# # Executables
# DXFToGLTF
add_executable(DXFToGLTF ${DXFToGLTFSOURCE})

# StepToGLTF
add_executable(StepToGLTF ${StepToGLTFSOURCE})

# StepSplit
add_executable(StepSplit ${StepSplitSOURCE})

# GmshToGLTF
add_executable(GmshToGLTF ${GmshToGLTFSOURCE})

# VTUToGLTF
add_executable(VTUToGLTF ${VTUToGLTFSOURCE})

# # Install
install(TARGETS DXFToGLTF StepToGLTF StepSplit GmshToGLTF VTUToGLTF)

# # Tests
include(CTest)
include(Catch)

set(LOGGER_TESTS
  test/logger/Logger.test.cpp
)

set(GEOMETRY_TESTS
  test/geometry/Polygon.test.cpp
  test/geometry/Tetrahedron.test.cpp
  test/geometry/Triangle.test.cpp
  test/geometry/Vertex.test.cpp
)

set(GMSH_TESTS
  test/gmsh/Gmsh.test.cpp
)

set(OCC_TESTS
  test/occ/StepReader.test.cpp
  test/occ/Triangulation.test.cpp
)

set(VTK_TESTS
  test/vtk/VTUReader.test.cpp
)

set(DXF_TESTS
  test/dxf/DXFConverter.test.cpp
  test/dxf/lib/Arc.test.cpp
  test/dxf/lib/Circle.test.cpp
  test/dxf/lib/Line.test.cpp
  test/dxf/lib/Polyline.test.cpp
  test/dxf/lib/Vertex.test.cpp
)

set(UTILS_TEST
  test/utils/utils.test.cpp
)

set(SOURCE_TESTS
  test/main.cpp
  ${DXF_TESTS}
  ${GEOMETRY_TESTS}
  ${GMSH_TESTS}
  ${LOGGER_TESTS}
  ${OCC_TESTS}
  ${VTK_TESTS}
  ${UTILS_TEST}
  ${DXF_SOURCE}
  ${DXF_LIB_SOURCE}
  ${GEOMETRY_SOURCE}
  ${GMSH_SOURCE}
  ${LOGGER_SOURCE}
  ${OCC_SOURCE}
  ${UTILS_SOURCE}
  ${VTK_SOURCE}
)

add_executable(tests ${SOURCE_TESTS})
target_link_libraries(tests Catch2::Catch2)
catch_discover_tests(tests)

# # Coverage
option(COVERAGE "Enable coverage flags" OFF)

if(COVERAGE)
  SET(CMAKE_CXX_FLAGS "-O0 --coverage -W")

  add_custom_target(coverage
    COMMAND make clean
    COMMAND make -j"$(nproc)"
    COMMAND make test
    COMMAND ./DXFToGLTF || true
    COMMAND ./DXFToGLTF ../test/assets/not_existing.dxf not_existing.glb not_existing.brep || true
    COMMAND ./DXFToGLTF ../test/assets/pipe.dxf pipe.glb pipe.brep
    COMMAND ./DXFToGLTF ../test/assets/circle.dxf circle.glb circle.brep
    COMMAND ./DXFToGLTF ../test/assets/curved.dxf curved.glb curved.brep
    COMMAND ./DXFToGLTF ../test/assets/curved_twice.dxf curved_twice.glb curved_twice.brep
    COMMAND ./DXFToGLTF ../test/assets/square.dxf square.glb square.brep
    COMMAND ./DXFToGLTF ../test/assets/square_hole.dxf square_hole.glb square_hole.brep
    COMMAND ./DXFToGLTF ../test/assets/spline.dxf spline.glb spline.brep
    COMMAND ./DXFToGLTF ../test/assets/spline_twice.dxf spline_twice.glb spline_twice.brep
    COMMAND ./GmshToGLTF || true
    COMMAND ./GmshToGLTF ../test/assets/not_existing.msh not_existing.glb || true
    COMMAND ./GmshToGLTF ../test/assets/Cube.msh Cube.glb
    COMMAND ./StepToGLTF || true
    COMMAND ./StepToGLTF ../test/assets/not_existing.step not_existing.glb not_existing.brep || true
    COMMAND ./StepToGLTF ../test/assets/cube.step cube.glb cube.brep
    COMMAND ./StepSplit || true
    COMMAND ./StepSplit ../test/assets/not_existing.step || true
    COMMAND ./StepSplit ../test/assets/cube.step
    COMMAND ./VTUToGLTF || true
    COMMAND ./VTUToGLTF ../test/assets/not_existing.vtu not_existing || true
    COMMAND ./VTUToGLTF ../test/assets/Result.vtu Result
    COMMAND ./VTUToGLTF ../test/assets/Result2Pieces.vtu Result
    COMMAND ./VTUToGLTF ../test/assets/Result_streamTracer.vtu Result
    COMMAND lcov --directory . -c -o report0.info
    COMMAND lcov --remove report0.info '/usr/include/*' '/usr/local/include/*' '/usr/lib/*' -o report.info
    COMMAND genhtml -o ../coverage -t "Converters coverage" report.info
  )
endif()
