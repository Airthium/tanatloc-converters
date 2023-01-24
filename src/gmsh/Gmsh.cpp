#include "Gmsh.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "../logger/Logger.hpp"
#include "../utils/utils.hpp"

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
    auto vertex = Vertex(x, y, z);
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
      auto triangle = Triangle(--index1, --index2, --index3, label);
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
      auto tetrahedron =
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
 * Get surface labels
 */
std::vector<uint> Gmsh::getSurfaceLabels() const {
  return this->m_triangleLabels;
}

/**
 * Index job
 * @param index Index
 * @param originalVertices Original vertices
 * @param vertices Vertices
 * @param indices Indices
 * @param set Set function
 */
template <typename FuncT>
void indexJob(const uint index, const std::vector<Vertex> &originalVertices,
              std::vector<Vertex> &vertices,
              std::vector<std::pair<uint, uint>> &indices, FuncT set) {
  auto find = Utils::findIndex(index, indices);
  if (find == -1) {
    const auto newIndex = (uint)vertices.size();
    vertices.push_back(originalVertices.at(index));
    set(newIndex);
    indices.push_back({index, newIndex});
  } else {
    set(find);
  }
}

/**
 * Get surface
 * @param label Label
 * @return Surface
 */
Surface Gmsh::getSurface(const uint label) const {
  // Surface triangles & vertices
  std::vector<Triangle> tempTriangles;
  std::vector<Triangle> triangles;
  std::vector<Vertex> vertices;

  // Surface triangles
  std::for_each(this->m_triangles.begin(), this->m_triangles.end(),
                [label, &tempTriangles](const Triangle triangle) {
                  if (triangle.Label() == label) {
                    tempTriangles.push_back(triangle);
                  }
                });

  // Surface vertices
  std::vector<std::pair<uint, uint>> newIndices;
  std::for_each(
      tempTriangles.begin(), tempTriangles.end(),
      [this, &vertices, &newIndices, &triangles](const Triangle triangle) {
        const uint index1 = triangle.I1();
        const uint index2 = triangle.I2();
        const uint index3 = triangle.I3();

        Triangle newTriangle;

        indexJob(
            index1, this->m_vertices, vertices, newIndices,
            [&newTriangle](const uint index) { newTriangle.setI1(index); });

        indexJob(
            index2, this->m_vertices, vertices, newIndices,
            [&newTriangle](const uint index) { newTriangle.setI2(index); });

        indexJob(
            index3, this->m_vertices, vertices, newIndices,
            [&newTriangle](const uint index) { newTriangle.setI3(index); });

        triangles.push_back(newTriangle);
      });

  // min / max
  std::vector<uint> minMaxIndex = Utils::minMax(triangles);
  std::vector<Vertex> minMaxVertex = Utils::minMax(vertices);

  Surface surface;
  surface.label = label;
  surface.minIndex = minMaxIndex.at(0);
  surface.maxIndex = minMaxIndex.at(1);
  surface.minVertex = minMaxVertex.at(0);
  surface.maxVertex = minMaxVertex.at(1);
  surface.triangles = triangles;
  surface.vertices = vertices;

  return surface;
}
