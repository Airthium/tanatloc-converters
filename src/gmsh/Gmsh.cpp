#include "Gmsh.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
Gmsh::Gmsh() = default;

/**
 * Loader
 * @param {string} fileName File name
 * @returns {boolean} Loading status
 */
bool Gmsh::load(const std::string &fileName) {
  uint i;
  uint numberOfVertices;
  uint numberOfElements;
  std::string buffer;
  std::ifstream file;

  // Read file
  file.open(fileName, std::ios::in);
  if (!file) {
    Logger::ERROR("Unable to open " + fileName);
    return false;
  }

  // Vertices
  file >> buffer;
  while (buffer != "$Nodes" && !file.eof())
    file >> buffer;

  file >> numberOfVertices;
  if (!numberOfVertices) {
    Logger::ERROR("No vertices");
    return false;
  }

  for (i = 0; i < numberOfVertices; ++i) {
    double x;
    double y;
    double z;
    file >> buffer >> x >> y >> z;
    Vertex vertex = Vertex(x, y, z);
    this->m_vertices.push_back(vertex);
  }

  // Elements
  file >> buffer;
  while (buffer != "$Elements" && !file.eof())
    file >> buffer;

  file >> numberOfElements;
  if (!numberOfElements) {
    Logger::ERROR("No elements");
    return false;
  }

  for (i = 0; i < numberOfElements; ++i) {
    uint type;
    uint numberOfLabels;
    uint label;
    file >> buffer >> type >> numberOfLabels;

    file >> label >> buffer;
    for (uint j = 2; j < numberOfLabels; ++j)
      file >> buffer;

    if (type == 2) { // 3-nodes triangle
      uint index1;
      uint index2;
      uint index3;
      file >> index1 >> index2 >> index3;
      Triangle triangle = Triangle(--index1, --index2, --index3, label);
      this->m_triangles.push_back(triangle);
      if (std::find(this->m_triangleLabels.begin(),
                    this->m_triangleLabels.end(),
                    label) == this->m_triangleLabels.end())
        this->m_triangleLabels.push_back(label);
    } else if (type == 4) { // 4-nodes tetrahedron
      uint index1;
      uint index2;
      uint index3;
      uint index4;
      file >> index1 >> index2 >> index3 >> index4;
      Tetrahedron tetrahedron =
          Tetrahedron(--index1, --index2, --index3, --index4, label);
      this->m_tetrahedra.push_back(tetrahedron);
      if (std::find(this->m_tetrahedronLabels.begin(),
                    this->m_tetrahedronLabels.end(),
                    label) == this->m_tetrahedronLabels.end())
        this->m_tetrahedronLabels.push_back(label);
    } else if (type == 15) { // 1-node point
      uint index1;
      file >> index1;
    } else if (type == 1) { // 2-node line
      uint index1;
      uint index2;
      file >> index1 >> index2;
    } else {
      Logger::ERROR("Unsupported type: " + std::to_string(type));
      continue;
    }
  }

  return true;
}

/**
 * Get volume labels
 */
std::vector<uint> Gmsh::getVolumeLabels() const {
  return this->m_tetrahedronLabels;
}

/**
 * Get surface labels
 */
std::vector<uint> Gmsh::getSurfaceLabels() const {
  return this->m_triangleLabels;
}

/**
 * Get surface
 * @param label Label
 * @return Surface
 */
Surface Gmsh::getSurface(const uint label) const {
  // Surface triangles & vertices
  std::vector<Triangle> tempSurfaceTriangles;
  std::vector<Triangle> surfaceTriangles;
  std::vector<Vertex> surfaceVertices;

  // Surface triangles
  std::for_each(this->m_triangles.begin(), this->m_triangles.end(),
                [label, &tempSurfaceTriangles](const Triangle triangle) {
                  if (triangle.Label() == label) {
                    tempSurfaceTriangles.push_back(triangle);
                  }
                });

  // Surface vertices
  std::vector<std::pair<uint, uint>> newIndices;
  std::for_each(
      tempSurfaceTriangles.begin(), tempSurfaceTriangles.end(),
      [this, &surfaceVertices, &newIndices,
       &surfaceTriangles](const Triangle triangle) {
        const uint index1 = triangle.I1();
        const uint index2 = triangle.I2();
        const uint index3 = triangle.I3();

        Triangle newTriangle;

        auto find1 =
            std::find_if(newIndices.begin(), newIndices.end(),
                         [index1](const std::pair<uint, uint> newIndex) {
                           return newIndex.first == index1;
                         });
        if (find1 == newIndices.end()) {
          const uint newIndex1 = surfaceVertices.size();
          surfaceVertices.push_back(this->m_vertices.at(index1));
          newTriangle.setI1(newIndex1);
          newIndices.push_back({index1, newIndex1});
        } else {
          newTriangle.setI1((*find1).second);
        }

        auto find2 =
            std::find_if(newIndices.begin(), newIndices.end(),
                         [index2](const std::pair<uint, uint> oldIndex) {
                           return oldIndex.first == index2;
                         });
        if (find2 == newIndices.end()) {
          const uint newIndex2 = surfaceVertices.size();
          surfaceVertices.push_back(this->m_vertices.at(index2));
          newTriangle.setI2(newIndex2);
          newIndices.push_back({index2, newIndex2});
        } else {
          newTriangle.setI2((*find2).second);
        }

        auto find3 =
            std::find_if(newIndices.begin(), newIndices.end(),
                         [index3](const std::pair<uint, uint> oldIndex) {
                           return oldIndex.first == index3;
                         });
        if (find3 == newIndices.end()) {
          const uint newIndex3 = surfaceVertices.size();
          surfaceVertices.push_back(this->m_vertices.at(index3));
          newTriangle.setI3(newIndex3);
          newIndices.push_back({index3, newIndex3});
        } else {
          newTriangle.setI3((*find3).second);
        }

        surfaceTriangles.push_back(newTriangle);
      });

  // min / max
  uint minIndex = 0; // Min is always 0
  uint maxIndex = 0;
  std::for_each(surfaceTriangles.begin(), surfaceTriangles.end(),
                [&maxIndex](const Triangle triangle) {
                  const uint index1 = triangle.I1();
                  const uint index2 = triangle.I2();
                  const uint index3 = triangle.I3();

                  maxIndex = std::max(
                      maxIndex, std::max(index1, std::max(index2, index3)));
                });

  Vertex minVertex(surfaceVertices.size() ? surfaceVertices.at(0)
                                          : Vertex(0, 0, 0));
  Vertex maxVertex(surfaceVertices.size() ? surfaceVertices.at(0)
                                          : Vertex(0, 0, 0));
  std::for_each(surfaceVertices.begin(), surfaceVertices.end(),
                [&minVertex, &maxVertex](const Vertex vertex) {
                  const double x = vertex.X();
                  const double y = vertex.Y();
                  const double z = vertex.Z();

                  minVertex.setX(std::min(minVertex.X(), x));
                  minVertex.setY(std::min(minVertex.Y(), y));
                  minVertex.setZ(std::min(minVertex.Z(), z));

                  maxVertex.setX(std::max(maxVertex.X(), x));
                  maxVertex.setY(std::max(maxVertex.Y(), y));
                  maxVertex.setZ(std::max(maxVertex.Z(), z));
                });

  Surface surface;
  surface.label = label;
  surface.minIndex = minIndex;
  surface.maxIndex = maxIndex;
  surface.minVertex = minVertex;
  surface.maxVertex = maxVertex;
  surface.triangles = surfaceTriangles;
  surface.vertices = surfaceVertices;

  return surface;
}

/**
 * Get volume
 * @param label Label
 * @return Volume
 */
Volume Gmsh::getVolume(const uint label) const {
  // Volume tethrahedra & vertices
  std::vector<Tetrahedron> tempVolumeTetrahedra;
  std::vector<Tetrahedron> volumeTetrahedra;
  std::vector<Vertex> volumeVertices;

  // Volume tetrahedra
  std::for_each(this->m_tetrahedra.begin(), this->m_tetrahedra.end(),
                [label, &tempVolumeTetrahedra](const Tetrahedron tetrahedron) {
                  if (tetrahedron.Label() == label) {
                    tempVolumeTetrahedra.push_back(tetrahedron);
                  }
                });

  // Volume vertices
  std::vector<std::pair<uint, uint>> newIndices;
  std::for_each(
      tempVolumeTetrahedra.begin(), tempVolumeTetrahedra.end(),
      [this, &volumeVertices, &newIndices,
       &volumeTetrahedra](const Tetrahedron tetrahedron) {
        const uint index1 = tetrahedron.I1();
        const uint index2 = tetrahedron.I2();
        const uint index3 = tetrahedron.I3();
        const uint index4 = tetrahedron.I4();

        Tetrahedron newTetrahedron;

        auto find1 =
            std::find_if(newIndices.begin(), newIndices.end(),
                         [index1](const std::pair<uint, uint> newIndex) {
                           return newIndex.first == index1;
                         });
        if (find1 == newIndices.end()) {
          const uint newIndex1 = volumeVertices.size();
          volumeVertices.push_back(this->m_vertices.at(index1));
          newTetrahedron.setI1(newIndex1);
          newIndices.push_back({index1, newIndex1});
        } else {
          newTetrahedron.setI1((*find1).second);
        }

        auto find2 =
            std::find_if(newIndices.begin(), newIndices.end(),
                         [index2](const std::pair<uint, uint> oldIndex) {
                           return oldIndex.first == index2;
                         });
        if (find2 == newIndices.end()) {
          const uint newIndex2 = volumeVertices.size();
          volumeVertices.push_back(this->m_vertices.at(index2));
          newTetrahedron.setI2(newIndex2);
          newIndices.push_back({index2, newIndex2});
        } else {
          newTetrahedron.setI2((*find2).second);
        }

        auto find3 =
            std::find_if(newIndices.begin(), newIndices.end(),
                         [index3](const std::pair<uint, uint> oldIndex) {
                           return oldIndex.first == index3;
                         });
        if (find3 == newIndices.end()) {
          const uint newIndex3 = volumeVertices.size();
          volumeVertices.push_back(this->m_vertices.at(index3));
          newTetrahedron.setI3(newIndex3);
          newIndices.push_back({index3, newIndex3});
        } else {
          newTetrahedron.setI3((*find3).second);
        }

        auto find4 =
            std::find_if(newIndices.begin(), newIndices.end(),
                         [index4](const std::pair<uint, uint> oldIndex) {
                           return oldIndex.first == index4;
                         });
        if (find4 == newIndices.end()) {
          const uint newIndex4 = volumeVertices.size();
          volumeVertices.push_back(this->m_vertices.at(index4));
          newTetrahedron.setI4(newIndex4);
          newIndices.push_back({index4, newIndex4});
        } else {
          newTetrahedron.setI4((*find4).second);
        }

        volumeTetrahedra.push_back(newTetrahedron);
      });

  // min / max
  uint minIndex = 0; // Min is always 0
  uint maxIndex = 0;
  std::for_each(
      volumeTetrahedra.begin(), volumeTetrahedra.end(),
      [&maxIndex](const Tetrahedron tetrahedron) {
        const uint index1 = tetrahedron.I1();
        const uint index2 = tetrahedron.I2();
        const uint index3 = tetrahedron.I3();
        const uint index4 = tetrahedron.I4();

        maxIndex = std::max(
            maxIndex,
            std::max(index1, std::max(index2, std::max(index3, index4))));
      });

  Vertex minVertex(volumeVertices.size() ? volumeVertices.at(0)
                                         : Vertex(0, 0, 0));
  Vertex maxVertex(volumeVertices.size() ? volumeVertices.at(0)
                                         : Vertex(0, 0, 0));
  std::for_each(volumeVertices.begin(), volumeVertices.end(),
                [&minVertex, &maxVertex](const Vertex vertex) {
                  const double x = vertex.X();
                  const double y = vertex.Y();
                  const double z = vertex.Z();

                  minVertex.setX(std::min(minVertex.X(), x));
                  minVertex.setY(std::min(minVertex.Y(), y));
                  minVertex.setZ(std::min(minVertex.Z(), z));

                  maxVertex.setX(std::max(maxVertex.X(), x));
                  maxVertex.setY(std::max(maxVertex.Y(), y));
                  maxVertex.setZ(std::max(maxVertex.Z(), z));
                });

  Volume volume;
  volume.label = label;
  volume.minIndex = minIndex;
  volume.maxIndex = maxIndex;
  volume.minVertex = minVertex;
  volume.maxVertex = maxVertex;
  volume.tetrahedra = volumeTetrahedra;
  volume.vertices = volumeVertices;

  return volume;
}
