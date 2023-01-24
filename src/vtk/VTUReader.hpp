#ifndef _VTU_READER_
#define _VTU_READER_

#include <string>
#include <vector>

#include <vtkSmartPointer.h>
#include <vtkXMLUnstructuredGridReader.h>

#include "../geometry/Polygon.hpp"
#include "../geometry/Tetrahedron.hpp"
#include "../geometry/Triangle.hpp"
#include "../geometry/Vertex.hpp"

struct VTUData {
  int size;
  std::string name;
  std::vector<Vertex> vertices;
  std::vector<Polygon> polygons;
  std::vector<Triangle> triangles;
  std::vector<Tetrahedron> tetrahedra;
  std::vector<double> values;
};

struct Result {
  uint size;
  std::string name;

  uint polygonsMinIndex;
  uint polygonsMaxIndex;
  Vertex polygonsMinVertex;
  Vertex polygonsMaxVertex;
  double polygonsMinValue;
  double polygonsMaxValue;
  std::vector<Polygon> polygons;
  std::vector<Vertex> polygonsVertices;
  std::vector<double> polygonsValues;

  uint trianglesMinIndex;
  uint trianglesMaxIndex;
  Vertex trianglesMinVertex;
  Vertex trianglesMaxVertex;
  double trianglesMinValue;
  double trianglesMaxValue;
  std::vector<Triangle> triangles;
  std::vector<Vertex> trianglesVertices;
  std::vector<double> trianglesValues;
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

  // Get results
  std::vector<Result> getResults() const;
};

#endif // VTU_READER_
