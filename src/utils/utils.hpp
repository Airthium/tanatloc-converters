#ifndef _UTILS_
#define _UTILS_

#include <string>

#include "../geometry/Polygon.hpp"
#include "../geometry/Tetrahedron.hpp"
#include "../geometry/Triangle.hpp"
#include "../geometry/Vertex.hpp"

namespace Utils {

/**
 * UUID
 * @return UUID
 */
std::string uuid();

/**
 * Generate random
 * @return Random
 */
double generateRandom();

/**
 * Unsigned int to buffer
 * @param value Value
 * @param buffer Buffer
 */
void uintToBuffer(uint value, std::vector<unsigned char> &buffer);

/**
 * Float to buffer
 * @param value Value
 * @param buffer Buffer
 */
void floatToBuffer(float value, std::vector<unsigned char> &buffer);

/**
 * Find index
 * @param index Index
 * @param indices Indices
 * @return Find
 */
int findIndex(uint index, const std::vector<std::pair<uint, uint>> &indices);

/**
 * Min / Max (indices)
 * @param tetrahedra Tetrahedra
 * @return { min, max }
 */
std::vector<uint> minMax(const std::vector<Tetrahedron> &tetrahedra);

/**
 * Min / Max (indices)
 * @param triangles Triangles
 * @return { min, max }
 */
std::vector<uint> minMax(const std::vector<Triangle> &triangles);

/**
 * Min / Max (indices)
 * @param polygons Polygons
 * @return { min, max }
 */
std::vector<uint> minMax(const std::vector<Polygon> &polygons);

/**
 * Min / Max (vertices)
 * @param vertices Vertices
 * @return { min, max }
 */
std::vector<Vertex> minMax(const std::vector<Vertex> &vertices);

/**
 * Min / Max (values)
 * @param values Values
 * @return { min, max }
 */
std::vector<double> minMax(const std::vector<double> &values);

} // namespace Utils

#endif //_UTILS_
