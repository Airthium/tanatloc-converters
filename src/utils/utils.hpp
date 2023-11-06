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
void uintToBuffer(uint, std::vector<unsigned char> &);

/**
 * Float to buffer
 * @param value Value
 * @param buffer Buffer
 */
void floatToBuffer(float, std::vector<unsigned char> &);

/**
 * Find index
 * @param index Index
 * @param indices Indices
 * @return Find
 */
int findIndex(uint, const std::vector<std::pair<uint, uint>> &);

/**
 * Min / Max (indices)
 * @param tetrahedra Tetrahedra
 * @return { min, max }
 */
std::vector<uint> minMax(const std::vector<Tetrahedron> &);

/**
 * Min / Max (indices)
 * @param triangles Triangles
 * @return { min, max }
 */
std::vector<uint> minMax(const std::vector<Triangle> &);

/**
 * Min / Max (indices)
 * @param polygons Polygons
 * @return { min, max }
 */
std::vector<uint> minMax(const std::vector<Polygon> &);

/**
 * Min / Max (vertices)
 * @param vertices Vertices
 * @return { min, max }
 */
std::vector<Vertex> minMax(const std::vector<Vertex> &);

/**
 * Min / Max (values)
 * @param values Values
 * @return { min, max }
 */
std::vector<double> minMax(const std::vector<double> &);

/**
 * Remove extensions
 * @param str String
 * @return std::string String without extension
 */
std::string removeExtension(const std::string &);

} // namespace Utils

#endif //_UTILS_
