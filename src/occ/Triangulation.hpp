#ifndef _TRIANGULATION_
#define _TRIANGULATION_

#include <vector>

#include <TopoDS_Compound.hxx>

#include "../geometry/Vertex.hpp"

constexpr double meshQuality = 1.e-3;

struct FaceMesh {
  uint label = 0;
  uint minIndex = 0;
  uint maxIndex = 0;
  Vertex minVertex;
  Vertex maxVertex;
  std::vector<uint> indices;
  std::vector<Vertex> vertices;
  std::vector<Vertex> normals;
};

class Triangulation {
private:
  TopoDS_Compound m_compound;
  double m_minBb = 0;
  double m_maxBb = 0;

  // Compute max bounding box
  void computeBb();

  // Is Valid
  bool isValid(const gp_Pnt &, const gp_Pnt &, const gp_Pnt &) const;

public:
  // Constructor
  Triangulation();
  // Constructor
  explicit Triangulation(const TopoDS_Compound &);

  // Triangulate face
  FaceMesh triangulateFace(const TopoDS_Shape &) const;

  // triangulate edge
  FaceMesh triangulateEdge(const TopoDS_Shape &) const;
};

#endif //_TRIANGULATION_
