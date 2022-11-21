#include "utils.hpp"

namespace Utils {

/**
 * UUID
 * @return UUID
 */
std::string uuid() {
  uuid_t uuid;
  char uuidChar[UUID_STR_LEN];
  uuid_generate_random(uuid);
  uuid_unparse_upper(uuid, uuidChar);

  return std::string(uuidChar);
}

/**
 * Generate random
 * @return Random
 */
double generateRandom() {
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_real_distribution<> dist(0., 1.);
  return dist(generator);
}

/**
 * Unsigned int to buffer
 * @param value Value
 * @param buffer Buffer
 */
void uintToBuffer(uint value, std::vector<unsigned char> &buffer) {
  unsigned char buf[__SIZEOF_INT__];
  std::memcpy(buf, &value, __SIZEOF_INT__);

  for (size_t i = 0; i < __SIZEOF_INT__; ++i) {
    buffer.push_back(buf[i]);
  }
}

/**
 * Float to buffer
 * @param value Value
 * @param buffer Buffer
 */
void floatToBuffer(float value, std::vector<unsigned char> &buffer) {
  unsigned char buf[__SIZEOF_FLOAT__];
  std::memcpy(buf, &value, __SIZEOF_FLOAT__);

  for (size_t i = 0; i < __SIZEOF_FLOAT__; ++i) {
    buffer.push_back(buf[i]);
  }
}

/**
 * Find index
 * @param index Index
 * @param indices Indices
 * @return Find
 */
int findIndex(uint index, const std::vector<std::pair<uint, uint>> &indices) {
  auto find = std::find_if(indices.begin(), indices.end(),
                           [index](const std::pair<uint, uint> indexPair) {
                             return indexPair.first == index;
                           });

  if (find == indices.end()) {
    return -1;
  } else {
    return (*find).second;
  }
}

/**
 * Min / Max (indices)
 * @param tetrahedra Tetrahedra
 * @return { min, max }
 */
std::vector<uint> minMax(const std::vector<Tetrahedron> &tetrahedra) {
  uint min = 0; // Min is always 0
  uint max = 0;

  std::for_each(
      tetrahedra.begin(), tetrahedra.end(),
      [&max](const Tetrahedron &tetrahedron) {
        const uint index1 = tetrahedron.I1();
        const uint index2 = tetrahedron.I2();
        const uint index3 = tetrahedron.I3();
        const uint index4 = tetrahedron.I4();

        max = std::max(
            max, std::max(index1, std::max(index2, std::max(index3, index4))));
      });

  return {min, max};
}

/**
 * Min / Max (indices)
 * @param triangles Triangles
 * @return { min, max }
 */
std::vector<uint> minMax(const std::vector<Triangle> &triangles) {
  uint min = 0; // Min is always 0
  uint max = 0;

  std::for_each(
      triangles.begin(), triangles.end(), [&max](const Triangle triangle) {
        const uint index1 = triangle.I1();
        const uint index2 = triangle.I2();
        const uint index3 = triangle.I3();

        max = std::max(max, std::max(index1, std::max(index2, index3)));
      });

  return {min, max};
}

/**
 * Min / Max (indices)
 * @param polygons Polygons
 * @return { min, max }
 */
std::vector<uint> minMax(const std::vector<Polygon> &polygons) {
  uint min = 0; // Min is always 0
  uint max = 0;

  // Polygons
  std::for_each(
      polygons.begin(), polygons.end(), [&max](const Polygon &polygon) {
        const std::vector<uint> indices = polygon.getIndices();
        max = std::max(max, *std::max_element(indices.begin(), indices.end()));
      });

  return {min, max};
}

/**
 * Min / Max (vertices)
 * @param vertices Vertices
 * @return { min, max }
 */
std::vector<Vertex> minMax(const std::vector<Vertex> &vertices) {
  Vertex min(vertices.size() ? vertices.at(0) : Vertex(0, 0, 0));
  Vertex max(vertices.size() ? vertices.at(0) : Vertex(0, 0, 0));

  std::for_each(vertices.begin(), vertices.end(),
                [&min, &max](const Vertex &vertex) {
                  const double x = vertex.X();
                  const double y = vertex.Y();
                  const double z = vertex.Z();

                  min.setX(std::min(min.X(), x));
                  min.setY(std::min(min.Y(), y));
                  min.setZ(std::min(min.Z(), z));

                  max.setX(std::max(max.X(), x));
                  max.setY(std::max(max.Y(), y));
                  max.setZ(std::max(max.Z(), z));
                });

  return {min, max};
}

/**
 * Min / Max (values)
 * @param values Values
 * @return { min, max }
 */
std::vector<double> minMax(const std::vector<double> &values) {
  double min = values.size() ? values.at(0) : 0;
  double max = values.size() ? values.at(0) : 0;

  std::for_each(values.begin(), values.end(), [&min, &max](const double value) {
    min = std::min(min, value);
    max = std::max(max, value);
  });

  return {min, max};
}

} // namespace Utils