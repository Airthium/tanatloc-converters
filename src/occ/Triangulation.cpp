#include "Triangulation.hpp"

#include <algorithm>

#include "../logger/Logger.hpp"
#include "makePipe.hpp"
#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_Box.hxx>
#include <Poly_Connect.hxx>
#include <Poly_Triangulation.hxx>
#include <StdPrs_ToolTriangulatedShape.hxx>
#include <TColgp_Array1OfDir.hxx>
#include <TopoDS.hxx>

/**
 * Constructor
 */
Triangulation::Triangulation() = default;

/**
 * Constructor
 * @param shape Shape
 */
Triangulation::Triangulation(const TopoDS_Compound &compound)
    : m_compound(compound) {
  this->computeBb();
}

/**
 * Compute bounding box and max dimensions
 */
void Triangulation::computeBb() {

  Bnd_Box boundingBox;
  double xMin;
  double yMin;
  double zMin;
  double xMax;
  double yMax;
  double zMax;

  BRepBndLib::Add(this->m_compound, boundingBox);
  boundingBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);

  double xDim = std::abs(xMax - xMin);
  double yDim = std::abs(yMax - yMin);
  double zDim = std::abs(zMax - zMin);

  this->m_minBb = std::min(xDim, std::min(yDim, zDim));
  this->m_maxBb = std::max(xDim, std::max(yDim, zDim));
}

/**
 * Triangulate face
 * @param face Face
 * @return FaceMesh
 */
FaceMesh Triangulation::triangulateFace(const TopoDS_Shape &face) const {
  BRepMesh_IncrementalMesh(face, this->m_maxBb * meshQuality);

  FaceMesh faceMesh;

  uint i;
  gp_Pnt p;
  gp_Dir d;

  TopLoc_Location location;
  Handle(Poly_Triangulation) triangulation =
      BRep_Tool::Triangulation(TopoDS::Face(face), location);

  if (triangulation.IsNull()) {
    Logger::ERROR("Null triangulation");
    return faceMesh;
  }
  Poly_Connect pc(triangulation);

  // Vertices
  const uint nbNodes = triangulation->NbNodes();
  TColgp_Array1OfPnt nodes(1, nbNodes);
  for (i = 1; i <= nbNodes; ++i) {
    const gp_Pnt &node = triangulation->Node(i);
    nodes.SetValue(i, node);
  }

  for (i = 1; i <= nbNodes; ++i) {
    const gp_Pnt node = nodes.Value(i);
    p = node.Transformed(location.Transformation());
    faceMesh.vertices.push_back(Vertex(p.X(), p.Y(), p.Z()));
  }

  // Normals
  TColgp_Array1OfDir normals(1, triangulation->NbNodes());
  StdPrs_ToolTriangulatedShape::Normal(TopoDS::Face(face), pc, normals);
  for (i = 1; i <= nbNodes; ++i) {
    d = normals(i).Transformed(location.Transformation());
    faceMesh.normals.push_back(Vertex(d.X(), d.Y(), d.Z()));
  }

  // Indices
  int n1;
  int n2;
  int n3;
  TopAbs_Orientation orient = face.Orientation();
  for (i = 1; i <= (uint)triangulation->NbTriangles(); ++i) {
    const Poly_Triangle &triangle = triangulation->Triangle(i);
    triangle.Get(n1, n2, n3);
    if (orient != TopAbs_FORWARD) {
      int tmp = n1;
      n1 = n2;
      n2 = tmp;
    }
    if (this->isValid(nodes.Value(n1), nodes.Value(n2), nodes.Value(n3))) {
      faceMesh.indices.push_back(--n1);
      faceMesh.indices.push_back(--n2);
      faceMesh.indices.push_back(--n3);
    }
  }

  // Min / max
  uint minIndex = 0; // Min is always 0
  uint maxIndex = 0;
  std::for_each(
      faceMesh.indices.begin(), faceMesh.indices.end(),
      [&maxIndex](const uint index) { maxIndex = std::max(maxIndex, index); });
  faceMesh.minIndex = minIndex;
  faceMesh.maxIndex = maxIndex;

  Vertex minVertex(faceMesh.vertices.size() ? faceMesh.vertices.at(0)
                                            : Vertex(0, 0, 0));
  Vertex maxVertex(faceMesh.vertices.size() ? faceMesh.vertices.at(0)
                                            : Vertex(0, 0, 0));
  std::for_each(faceMesh.vertices.begin(), faceMesh.vertices.end(),
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
  faceMesh.minVertex = minVertex;
  faceMesh.maxVertex = maxVertex;

  return faceMesh;
}

/**
 * Triangulate edge (convert to pipe)
 * @param edge Edge
 * @return FaceMesh
 */
FaceMesh Triangulation::triangulateEdge(const TopoDS_Shape &edge) const {
  TopoDS_Shape pipe = makePipe(this->m_maxBb / 250., TopoDS::Edge(edge));

  return triangulateFace(pipe);
}

/**
 * Check valid
 * @param p1 Point 1
 * @param p2 Point 2
 * @param p3 Point 3
 * @return Valid
 */
bool Triangulation::isValid(const gp_Pnt &p1, const gp_Pnt &p2,
                            const gp_Pnt &p3) const {
  gp_Vec v1(p1, p2);
  gp_Vec v2(p2, p3);
  gp_Vec v3(p3, p1);

  if ((v1.SquareMagnitude() > 1.e-10) && (v2.SquareMagnitude() > 1.e-10) &&
      (v3.SquareMagnitude() > 1.e-10)) {
    v1.Cross(v2);
    if (v1.SquareMagnitude() > 1.e-10)
      return true;
    else
      return false;
  }
  return false;
}
