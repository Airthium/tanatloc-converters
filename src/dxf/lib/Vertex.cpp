#include "Vertex.hpp"

#include "./Defs.hpp"
#include "./Utils.hpp"

#include "../../logger/Logger.hpp"

/**
 * Process
 * @param file File
 */
void DXFVertex::process(std::ifstream &file) {
  Logger::DEBUG("VERTEX::Process");

  std::string line;
  while (std::getline(file, line)) {
    rtrim(line);

    if (line == TAG10) {
      file >> this->x;
    } else if (line == TAG20) {
      file >> this->y;
    } else if (line == TAG30) {
      file >> this->z;
    } else if (line == TAG0)
      break;
  }

  // Force z to 0.
  this->z = 0.;
}

/**
 * Already exists?
 * @param vertices Vertices
 * @return true
 * @return false
 */
bool DXFVertex::alreadyExists(const std::vector<DXFVertex> &vertices) const {
  bool res = false;
  std::for_each(vertices.begin(), vertices.end(),
                [this, &res](const DXFVertex &vertex) {
                  if (*this == vertex)
                    res = true;
                });

  return res;
}

/**
 * Operator ==
 * @param vertex1 Vertex 1
 * @param vertex2 Vertex 2
 * @return true
 * @return false
 */
bool operator==(const DXFVertex &vertex1, const DXFVertex &vertex2) {
  return (vertex1.x == vertex2.x) && (vertex1.y == vertex2.y) &&
         (vertex1.z == vertex2.z);
}
