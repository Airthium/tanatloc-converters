# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build

# Include any dependencies generated for this target.
include CMakeFiles/VTUToGLTF.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/VTUToGLTF.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/VTUToGLTF.dir/flags.make

CMakeFiles/VTUToGLTF.dir/src/VTUToGLTF.cpp.o: CMakeFiles/VTUToGLTF.dir/flags.make
CMakeFiles/VTUToGLTF.dir/src/VTUToGLTF.cpp.o: ../src/VTUToGLTF.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/VTUToGLTF.dir/src/VTUToGLTF.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VTUToGLTF.dir/src/VTUToGLTF.cpp.o -c /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/VTUToGLTF.cpp

CMakeFiles/VTUToGLTF.dir/src/VTUToGLTF.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VTUToGLTF.dir/src/VTUToGLTF.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/VTUToGLTF.cpp > CMakeFiles/VTUToGLTF.dir/src/VTUToGLTF.cpp.i

CMakeFiles/VTUToGLTF.dir/src/VTUToGLTF.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VTUToGLTF.dir/src/VTUToGLTF.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/VTUToGLTF.cpp -o CMakeFiles/VTUToGLTF.dir/src/VTUToGLTF.cpp.s

CMakeFiles/VTUToGLTF.dir/src/geometry/Polygon.cpp.o: CMakeFiles/VTUToGLTF.dir/flags.make
CMakeFiles/VTUToGLTF.dir/src/geometry/Polygon.cpp.o: ../src/geometry/Polygon.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/VTUToGLTF.dir/src/geometry/Polygon.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VTUToGLTF.dir/src/geometry/Polygon.cpp.o -c /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/geometry/Polygon.cpp

CMakeFiles/VTUToGLTF.dir/src/geometry/Polygon.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VTUToGLTF.dir/src/geometry/Polygon.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/geometry/Polygon.cpp > CMakeFiles/VTUToGLTF.dir/src/geometry/Polygon.cpp.i

CMakeFiles/VTUToGLTF.dir/src/geometry/Polygon.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VTUToGLTF.dir/src/geometry/Polygon.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/geometry/Polygon.cpp -o CMakeFiles/VTUToGLTF.dir/src/geometry/Polygon.cpp.s

CMakeFiles/VTUToGLTF.dir/src/geometry/Tetrahedron.cpp.o: CMakeFiles/VTUToGLTF.dir/flags.make
CMakeFiles/VTUToGLTF.dir/src/geometry/Tetrahedron.cpp.o: ../src/geometry/Tetrahedron.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/VTUToGLTF.dir/src/geometry/Tetrahedron.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VTUToGLTF.dir/src/geometry/Tetrahedron.cpp.o -c /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/geometry/Tetrahedron.cpp

CMakeFiles/VTUToGLTF.dir/src/geometry/Tetrahedron.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VTUToGLTF.dir/src/geometry/Tetrahedron.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/geometry/Tetrahedron.cpp > CMakeFiles/VTUToGLTF.dir/src/geometry/Tetrahedron.cpp.i

CMakeFiles/VTUToGLTF.dir/src/geometry/Tetrahedron.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VTUToGLTF.dir/src/geometry/Tetrahedron.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/geometry/Tetrahedron.cpp -o CMakeFiles/VTUToGLTF.dir/src/geometry/Tetrahedron.cpp.s

CMakeFiles/VTUToGLTF.dir/src/geometry/Triangle.cpp.o: CMakeFiles/VTUToGLTF.dir/flags.make
CMakeFiles/VTUToGLTF.dir/src/geometry/Triangle.cpp.o: ../src/geometry/Triangle.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/VTUToGLTF.dir/src/geometry/Triangle.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VTUToGLTF.dir/src/geometry/Triangle.cpp.o -c /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/geometry/Triangle.cpp

CMakeFiles/VTUToGLTF.dir/src/geometry/Triangle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VTUToGLTF.dir/src/geometry/Triangle.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/geometry/Triangle.cpp > CMakeFiles/VTUToGLTF.dir/src/geometry/Triangle.cpp.i

CMakeFiles/VTUToGLTF.dir/src/geometry/Triangle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VTUToGLTF.dir/src/geometry/Triangle.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/geometry/Triangle.cpp -o CMakeFiles/VTUToGLTF.dir/src/geometry/Triangle.cpp.s

CMakeFiles/VTUToGLTF.dir/src/geometry/Vertex.cpp.o: CMakeFiles/VTUToGLTF.dir/flags.make
CMakeFiles/VTUToGLTF.dir/src/geometry/Vertex.cpp.o: ../src/geometry/Vertex.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/VTUToGLTF.dir/src/geometry/Vertex.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VTUToGLTF.dir/src/geometry/Vertex.cpp.o -c /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/geometry/Vertex.cpp

CMakeFiles/VTUToGLTF.dir/src/geometry/Vertex.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VTUToGLTF.dir/src/geometry/Vertex.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/geometry/Vertex.cpp > CMakeFiles/VTUToGLTF.dir/src/geometry/Vertex.cpp.i

CMakeFiles/VTUToGLTF.dir/src/geometry/Vertex.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VTUToGLTF.dir/src/geometry/Vertex.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/geometry/Vertex.cpp -o CMakeFiles/VTUToGLTF.dir/src/geometry/Vertex.cpp.s

CMakeFiles/VTUToGLTF.dir/src/gmsh/Gmsh.cpp.o: CMakeFiles/VTUToGLTF.dir/flags.make
CMakeFiles/VTUToGLTF.dir/src/gmsh/Gmsh.cpp.o: ../src/gmsh/Gmsh.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/VTUToGLTF.dir/src/gmsh/Gmsh.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VTUToGLTF.dir/src/gmsh/Gmsh.cpp.o -c /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/gmsh/Gmsh.cpp

CMakeFiles/VTUToGLTF.dir/src/gmsh/Gmsh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VTUToGLTF.dir/src/gmsh/Gmsh.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/gmsh/Gmsh.cpp > CMakeFiles/VTUToGLTF.dir/src/gmsh/Gmsh.cpp.i

CMakeFiles/VTUToGLTF.dir/src/gmsh/Gmsh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VTUToGLTF.dir/src/gmsh/Gmsh.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/gmsh/Gmsh.cpp -o CMakeFiles/VTUToGLTF.dir/src/gmsh/Gmsh.cpp.s

CMakeFiles/VTUToGLTF.dir/src/logger/Logger.cpp.o: CMakeFiles/VTUToGLTF.dir/flags.make
CMakeFiles/VTUToGLTF.dir/src/logger/Logger.cpp.o: ../src/logger/Logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/VTUToGLTF.dir/src/logger/Logger.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VTUToGLTF.dir/src/logger/Logger.cpp.o -c /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/logger/Logger.cpp

CMakeFiles/VTUToGLTF.dir/src/logger/Logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VTUToGLTF.dir/src/logger/Logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/logger/Logger.cpp > CMakeFiles/VTUToGLTF.dir/src/logger/Logger.cpp.i

CMakeFiles/VTUToGLTF.dir/src/logger/Logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VTUToGLTF.dir/src/logger/Logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/logger/Logger.cpp -o CMakeFiles/VTUToGLTF.dir/src/logger/Logger.cpp.s

CMakeFiles/VTUToGLTF.dir/src/occ/MainDocument.cpp.o: CMakeFiles/VTUToGLTF.dir/flags.make
CMakeFiles/VTUToGLTF.dir/src/occ/MainDocument.cpp.o: ../src/occ/MainDocument.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/VTUToGLTF.dir/src/occ/MainDocument.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VTUToGLTF.dir/src/occ/MainDocument.cpp.o -c /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/occ/MainDocument.cpp

CMakeFiles/VTUToGLTF.dir/src/occ/MainDocument.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VTUToGLTF.dir/src/occ/MainDocument.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/occ/MainDocument.cpp > CMakeFiles/VTUToGLTF.dir/src/occ/MainDocument.cpp.i

CMakeFiles/VTUToGLTF.dir/src/occ/MainDocument.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VTUToGLTF.dir/src/occ/MainDocument.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/occ/MainDocument.cpp -o CMakeFiles/VTUToGLTF.dir/src/occ/MainDocument.cpp.s

CMakeFiles/VTUToGLTF.dir/src/occ/StepReader.cpp.o: CMakeFiles/VTUToGLTF.dir/flags.make
CMakeFiles/VTUToGLTF.dir/src/occ/StepReader.cpp.o: ../src/occ/StepReader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/VTUToGLTF.dir/src/occ/StepReader.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VTUToGLTF.dir/src/occ/StepReader.cpp.o -c /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/occ/StepReader.cpp

CMakeFiles/VTUToGLTF.dir/src/occ/StepReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VTUToGLTF.dir/src/occ/StepReader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/occ/StepReader.cpp > CMakeFiles/VTUToGLTF.dir/src/occ/StepReader.cpp.i

CMakeFiles/VTUToGLTF.dir/src/occ/StepReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VTUToGLTF.dir/src/occ/StepReader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/occ/StepReader.cpp -o CMakeFiles/VTUToGLTF.dir/src/occ/StepReader.cpp.s

CMakeFiles/VTUToGLTF.dir/src/occ/Triangulation.cpp.o: CMakeFiles/VTUToGLTF.dir/flags.make
CMakeFiles/VTUToGLTF.dir/src/occ/Triangulation.cpp.o: ../src/occ/Triangulation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/VTUToGLTF.dir/src/occ/Triangulation.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VTUToGLTF.dir/src/occ/Triangulation.cpp.o -c /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/occ/Triangulation.cpp

CMakeFiles/VTUToGLTF.dir/src/occ/Triangulation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VTUToGLTF.dir/src/occ/Triangulation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/occ/Triangulation.cpp > CMakeFiles/VTUToGLTF.dir/src/occ/Triangulation.cpp.i

CMakeFiles/VTUToGLTF.dir/src/occ/Triangulation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VTUToGLTF.dir/src/occ/Triangulation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/occ/Triangulation.cpp -o CMakeFiles/VTUToGLTF.dir/src/occ/Triangulation.cpp.s

CMakeFiles/VTUToGLTF.dir/src/vtk/VTUReader.cpp.o: CMakeFiles/VTUToGLTF.dir/flags.make
CMakeFiles/VTUToGLTF.dir/src/vtk/VTUReader.cpp.o: ../src/vtk/VTUReader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/VTUToGLTF.dir/src/vtk/VTUReader.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VTUToGLTF.dir/src/vtk/VTUReader.cpp.o -c /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/vtk/VTUReader.cpp

CMakeFiles/VTUToGLTF.dir/src/vtk/VTUReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VTUToGLTF.dir/src/vtk/VTUReader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/vtk/VTUReader.cpp > CMakeFiles/VTUToGLTF.dir/src/vtk/VTUReader.cpp.i

CMakeFiles/VTUToGLTF.dir/src/vtk/VTUReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VTUToGLTF.dir/src/vtk/VTUReader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/src/vtk/VTUReader.cpp -o CMakeFiles/VTUToGLTF.dir/src/vtk/VTUReader.cpp.s

# Object files for target VTUToGLTF
VTUToGLTF_OBJECTS = \
"CMakeFiles/VTUToGLTF.dir/src/VTUToGLTF.cpp.o" \
"CMakeFiles/VTUToGLTF.dir/src/geometry/Polygon.cpp.o" \
"CMakeFiles/VTUToGLTF.dir/src/geometry/Tetrahedron.cpp.o" \
"CMakeFiles/VTUToGLTF.dir/src/geometry/Triangle.cpp.o" \
"CMakeFiles/VTUToGLTF.dir/src/geometry/Vertex.cpp.o" \
"CMakeFiles/VTUToGLTF.dir/src/gmsh/Gmsh.cpp.o" \
"CMakeFiles/VTUToGLTF.dir/src/logger/Logger.cpp.o" \
"CMakeFiles/VTUToGLTF.dir/src/occ/MainDocument.cpp.o" \
"CMakeFiles/VTUToGLTF.dir/src/occ/StepReader.cpp.o" \
"CMakeFiles/VTUToGLTF.dir/src/occ/Triangulation.cpp.o" \
"CMakeFiles/VTUToGLTF.dir/src/vtk/VTUReader.cpp.o"

# External object files for target VTUToGLTF
VTUToGLTF_EXTERNAL_OBJECTS =

VTUToGLTF: CMakeFiles/VTUToGLTF.dir/src/VTUToGLTF.cpp.o
VTUToGLTF: CMakeFiles/VTUToGLTF.dir/src/geometry/Polygon.cpp.o
VTUToGLTF: CMakeFiles/VTUToGLTF.dir/src/geometry/Tetrahedron.cpp.o
VTUToGLTF: CMakeFiles/VTUToGLTF.dir/src/geometry/Triangle.cpp.o
VTUToGLTF: CMakeFiles/VTUToGLTF.dir/src/geometry/Vertex.cpp.o
VTUToGLTF: CMakeFiles/VTUToGLTF.dir/src/gmsh/Gmsh.cpp.o
VTUToGLTF: CMakeFiles/VTUToGLTF.dir/src/logger/Logger.cpp.o
VTUToGLTF: CMakeFiles/VTUToGLTF.dir/src/occ/MainDocument.cpp.o
VTUToGLTF: CMakeFiles/VTUToGLTF.dir/src/occ/StepReader.cpp.o
VTUToGLTF: CMakeFiles/VTUToGLTF.dir/src/occ/Triangulation.cpp.o
VTUToGLTF: CMakeFiles/VTUToGLTF.dir/src/vtk/VTUReader.cpp.o
VTUToGLTF: CMakeFiles/VTUToGLTF.dir/build.make
VTUToGLTF: /usr/local/lib/libvtkIOXML-9.1.so.9.1.0
VTUToGLTF: /usr/local/lib/libTKXMesh.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKOpenGlTest.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKXDEDRAW.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKTObjDRAW.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKQADraw.so.7.6.0
VTUToGLTF: /usr/lib/x86_64-linux-gnu/libuuid.a
VTUToGLTF: /usr/local/lib/libvtkIOXMLParser-9.1.so.9.1.0
VTUToGLTF: /usr/local/lib/libvtkCommonExecutionModel-9.1.so.9.1.0
VTUToGLTF: /usr/local/lib/libvtkCommonDataModel-9.1.so.9.1.0
VTUToGLTF: /usr/local/lib/libvtkCommonTransforms-9.1.so.9.1.0
VTUToGLTF: /usr/local/lib/libvtkCommonMath-9.1.so.9.1.0
VTUToGLTF: /usr/local/lib/libvtkCommonCore-9.1.so.9.1.0
VTUToGLTF: /usr/local/lib/libvtksys-9.1.so.9.1.0
VTUToGLTF: /usr/local/lib/libvtkkissfft-9.1.so.9.1.0
VTUToGLTF: /usr/local/lib/libTKOpenGl.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKXDEIGES.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKXmlXCAF.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKBinTObj.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKXmlTObj.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKTObj.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKXDESTEP.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKBinXCAF.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKXSDRAW.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKMeshVS.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKSTEP.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKSTEPAttr.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKSTEP209.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKSTEPBase.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKIGES.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKXSBase.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKSTL.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKVRML.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKRWMesh.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKXCAF.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKDCAF.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKBin.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKBinL.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKXml.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKXmlL.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKStd.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKStdL.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKVCAF.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKCAF.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKLCAF.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKCDF.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKViewerTest.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKTopTest.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKOffset.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKFillet.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKFeat.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKBool.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKBO.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKPrim.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKV3d.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKDraw.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKHLR.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKMesh.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKShHealing.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKTopAlgo.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKGeomAlgo.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKBRep.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKGeomBase.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKG3d.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKG2d.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKService.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKMath.so.7.6.0
VTUToGLTF: /usr/local/lib/libTKernel.so.7.6.0
VTUToGLTF: /usr/lib/x86_64-linux-gnu/libtk8.6.so
VTUToGLTF: /usr/lib/x86_64-linux-gnu/libfreetype.so
VTUToGLTF: /usr/lib/x86_64-linux-gnu/libtcl8.6.so
VTUToGLTF: CMakeFiles/VTUToGLTF.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable VTUToGLTF"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/VTUToGLTF.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/VTUToGLTF.dir/build: VTUToGLTF

.PHONY : CMakeFiles/VTUToGLTF.dir/build

CMakeFiles/VTUToGLTF.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/VTUToGLTF.dir/cmake_clean.cmake
.PHONY : CMakeFiles/VTUToGLTF.dir/clean

CMakeFiles/VTUToGLTF.dir/depend:
	cd /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build /home/simon/Documents/Git/Airthium/tanatloc-dockers/converters/build/CMakeFiles/VTUToGLTF.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/VTUToGLTF.dir/depend

