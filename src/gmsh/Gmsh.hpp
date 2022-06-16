#ifndef _GMSH_
#define _GMSH_

#include <string>
#include <vector>

#include "../geometry/Tetrahedron.hpp"
#include "../geometry/Triangle.hpp"
#include "../geometry/Vertex.hpp"

using uint = unsigned int;

struct Surface {
  uint label;
  uint minIndex;
  uint maxIndex;
  Vertex minVertex;
  Vertex maxVertex;
  std::vector<Triangle> triangles;
  std::vector<Vertex> vertices;
};

struct Volume {
  uint label;
  uint minIndex;
  uint maxIndex;
  Vertex minVertex;
  Vertex maxVertex;
  std::vector<Tetrahedron> tetrahedra;
  std::vector<Vertex> vertices;
};

/**
 * Gmsh class
 */
class Gmsh {
private:
  // Vertices
  std::vector<Vertex> m_vertices = std::vector<Vertex>();
  // Triangles
  std::vector<Triangle> m_triangles = std::vector<Triangle>();
  // Triangle labels
  std::vector<uint> m_triangleLabels = std::vector<uint>();
  // Tetrahedra
  std::vector<Tetrahedron> m_tetrahedra = std::vector<Tetrahedron>();
  // Tetrahedron labels
  std::vector<uint> m_tetrahedronLabels = std::vector<uint>();

public:
  // Constructor
  Gmsh();

  // Loader
  bool load(const std::string &);

  // Get volume labels
  std::vector<uint> getVolumeLabels() const;

  // Get surface labels
  std::vector<uint> getSurfaceLabels() const;

  // Get volume
  Volume getVolume(const uint) const;

  // Get surface
  Surface getSurface(const uint) const;
};

#endif //_GMSH_
