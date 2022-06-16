#ifndef _VTU_READER_
#define _VTU_READER_

#include <string>
#include <vector>

#include "../geometry/Polygon.hpp"
#include "../geometry/Tetrahedron.hpp"
#include "../geometry/Triangle.hpp"
#include "../geometry/Vertex.hpp"
#include <vtkSmartPointer.h>
#include <vtkXMLUnstructuredGridReader.h>

struct VTUData {
  int size;
  std::string name;
  std::vector<Vertex> vertices;
  std::vector<Polygon> polygons;
  std::vector<Triangle> triangles;
  std::vector<Tetrahedron> tetrahedra;
  std::vector<double> values;
};

struct Surface {
  uint size;
  std::string name;
  uint minIndex;
  uint maxIndex;
  Vertex minVertex;
  Vertex maxVertex;
  double minValue;
  double maxValue;
  std::vector<Triangle> triangles;
  std::vector<Vertex> vertices;
  std::vector<double> values;
};

struct Line {
  uint size;
  std::string name;
  uint minIndex;
  uint maxIndex;
  Vertex minVertex;
  Vertex maxVertex;
  double minValue;
  double maxValue;
  std::vector<Polygon> polygons;
  std::vector<Vertex> vertices;
  std::vector<double> values;
};

class VTUReader {
private:
  std::string m_fileName = "";
  vtkSmartPointer<vtkXMLUnstructuredGridReader> m_reader =
      vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();

  std::vector<VTUData> m_arrays = std::vector<VTUData>();

public:
  // Constructor
  VTUReader();
  // Constructor
  explicit VTUReader(const std::string &);

  // Read
  bool read();

  // Get arrays
  std::vector<VTUData> getArrays() const;

  // Get surfaces
  std::vector<Surface> getSurfaces() const;

  // Get lines
  std::vector<Line> getLines() const;
};

#endif // VTU_READER_
