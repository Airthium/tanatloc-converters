#include "VTUReader.hpp"

#include <vtkCellArray.h>
#include <vtkIdList.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>

#include "../logger/Logger.hpp"
#include "../utils/utils.hpp"

/**
 * Constructor
 */
VTUReader::VTUReader() = default;

/**
 * Constructor
 */
VTUReader::VTUReader(const std::string &fileName) : m_fileName(fileName) {}

/**
 * Read
 * @return Status
 */
bool VTUReader::read() {
  // Check
  if (!this->m_reader->CanReadFile(m_fileName.c_str()))
    return false;

  // Read
  this->m_reader->SetFileName(m_fileName.c_str());
  this->m_reader->Update();

  vtkSmartPointer<vtkUnstructuredGrid> output = this->m_reader->GetOutput();

  if (output->GetNumberOfPieces() != 1) {
    Logger::ERROR("Number of pieces != 1");
    return false;
  }

  // Geometry
  VTUData baseData;
  baseData.name = "base";
  baseData.size = 0;

  // Vertices
  vtkSmartPointer<vtkPoints> points = output->GetPoints();
  const auto numberOfPoints = (int)points->GetNumberOfPoints();
  for (int i = 0; i < numberOfPoints; ++i) {
    const double *point = points->GetPoint(i);
    Vertex v(point[0], point[1], point[2]);
    baseData.vertices.push_back(v);
  }

  // Indices
  vtkSmartPointer<vtkCellArray> connectivity = output->GetCells();
  const auto numberOfCells = (int)connectivity->GetNumberOfCells();
  for (int i = 0; i < numberOfCells; ++i) {
    auto cellSize = connectivity->GetCellSize(i);

    vtkSmartPointer<vtkIdList> indices = vtkSmartPointer<vtkIdList>::New();
    connectivity->GetCellAtId(i, indices);

    if (cellSize == 3) { // Triangle
      Triangle triangle;

      const auto index1 = (int)indices->GetId(0);
      triangle.setI1(index1);
      const auto index2 = (int)indices->GetId(1);
      triangle.setI2(index2);
      const auto index3 = (int)indices->GetId(2);
      triangle.setI3(index3);

      baseData.triangles.push_back(triangle);
    } else if (cellSize == 4) { // Tetrahedron
      Tetrahedron tetrahedron;

      const auto index1 = (int)indices->GetId(0);
      tetrahedron.setI1(index1);
      const auto index2 = (int)indices->GetId(1);
      tetrahedron.setI2(index2);
      const auto index3 = (int)indices->GetId(2);
      tetrahedron.setI3(index3);
      const auto index4 = (int)indices->GetId(3);
      tetrahedron.setI4(index4);

      baseData.tetrahedra.push_back(tetrahedron);
    } else {
      Polygon polygon;

      for (int j = 0; j < cellSize; ++j) {
        polygon.addIndex((int)indices->GetId(j));
      }

      baseData.polygons.push_back(polygon);
    }
  }

  // Point data
  vtkSmartPointer<vtkPointData> pointData = output->GetPointData();
  const int numberOfPointData = pointData->GetNumberOfArrays();

  for (int i = 0; i < numberOfPointData; ++i) {
    VTUData data = baseData;
    data.name = pointData->GetArrayName(i);

    vtkSmartPointer<vtkDataArray> array = pointData->GetArray(i);
    const auto numberOfValues = (int)array->GetNumberOfValues();
    const int numberOfComponents = array->GetNumberOfComponents();
    data.size = numberOfComponents;

    const int numberOfTuples = numberOfValues / numberOfComponents;

    for (int j = 0; j < numberOfTuples; ++j) {
      const double *values = array->GetTuple(j);

      for (int k = 0; k < numberOfComponents; ++k)
        data.values.push_back(values[k]);
    }

    this->m_arrays.push_back(data);
  }

  return true;
}

/**
 * Get arrays
 * @return Arrays
 */
std::vector<VTUData> VTUReader::getArrays() const { return this->m_arrays; }

/**
 * Add vertex
 * @param index Index
 * @param data Data
 * @param vertices Vertices
 * @param values Values
 * @return New index
 */
uint addVertex(uint index, const VTUData &data, std::vector<Vertex> &vertices,
               std::vector<double> &values) {
  const auto newIndex = (uint)vertices.size();
  vertices.push_back(data.vertices.at(index));

  if (data.size == 1) {
    values.push_back(data.values.at(index));
  } else if (data.size == 3) {
    values.push_back(data.values.at(3 * index + 0));
    values.push_back(data.values.at(3 * index + 1));
    values.push_back(data.values.at(3 * index + 2));
  }

  return newIndex;
}

/**
 * Index job
 * @param index Index
 * @param data Data
 * @param vertices Vertices
 * @param values Values
 * @param indices Indices
 * @param set Set function
 */
template <typename FuncT>
void indexJob(const uint index, const VTUData &data,
              std::vector<Vertex> &vertices, std::vector<double> &values,
              std::vector<std::pair<uint, uint>> &indices, FuncT set) {
  const auto find = Utils::findIndex(index, indices);
  if (find == -1) {
    const auto newIndex = addVertex(index, data, vertices, values);
    set(newIndex);
    indices.push_back({index, newIndex});
  } else {
    set(find);
  }
}

/**
 * Triangle job
 * @param data Data
 * @param triangle Triangle
 * @param triangles Triangles
 * @param vertices Vertices
 * @param values Values
 * @param indices Indices
 */
void triangleJob(const VTUData &data, const Triangle triangle,
                 std::vector<Triangle> &triangles,
                 std::vector<Vertex> &vertices, std::vector<double> &values,
                 std::vector<std::pair<uint, uint>> &indices) {
  const uint index1 = triangle.I1();
  const uint index2 = triangle.I2();
  const uint index3 = triangle.I3();

  Triangle newTriangle;

  indexJob(index1, data, vertices, values, indices,
           [&newTriangle](uint index) { newTriangle.setI1(index); });

  indexJob(index2, data, vertices, values, indices,
           [&newTriangle](uint index) { newTriangle.setI2(index); });

  indexJob(index3, data, vertices, values, indices,
           [&newTriangle](uint index) { newTriangle.setI3(index); });

  triangles.push_back(newTriangle);
}

/**
 * Polygon job
 * @param data Data
 * @param polygon Polygon
 * @param polygons Polygons
 * @param vertices Vertices
 * @param values Values
 * @param indices Indices
 */
void polygonJob(const VTUData &data, const Polygon &polygon,
                std::vector<Polygon> &polygons, std::vector<Vertex> &vertices,
                std::vector<double> &values,
                std::vector<std::pair<uint, uint>> &indices) {
  const std::vector<uint> polygonIndices = polygon.getIndices();

  Polygon newPolygon;

  std::for_each(polygonIndices.begin(), polygonIndices.end(),
                [&data, &vertices, &values, &indices, &newPolygon](uint index) {
                  indexJob(index, data, vertices, values, indices,
                           [&newPolygon](uint newIndex) {
                             newPolygon.addIndex(newIndex);
                           });
                });

  polygons.push_back(newPolygon);
}

/**
 * Get result
 * @param data Data
 * @return Result
 */
Result getResult(const VTUData &data) {
  // Lines polygons, surface triangles & vertices
  std::vector<Polygon> tempPloygons = data.polygons;
  std::vector<Polygon> polygons;
  std::vector<Triangle> tempTriangles = data.triangles;
  std::vector<Triangle> triangles;
  std::vector<Vertex> polygonsVertices;
  std::vector<Vertex> trianglesVertices;
  std::vector<double> polygonsValues;
  std::vector<double> trianglesValues;

  // Polygons
  std::vector<std::pair<uint, uint>> polygonsIndices;
  std::for_each(tempPloygons.begin(), tempPloygons.end(),
                [&data, &polygons, &polygonsVertices, &polygonsValues,
                 &polygonsIndices](const Polygon &polygon) {
                  polygonJob(data, polygon, polygons, polygonsVertices,
                             polygonsValues, polygonsIndices);
                });

  // Triangles
  std::vector<std::pair<uint, uint>> trianglesIndices;
  std::for_each(tempTriangles.begin(), tempTriangles.end(),
                [&data, &triangles, &trianglesVertices, &trianglesValues,
                 &trianglesIndices](const Triangle &triangle) {
                  triangleJob(data, triangle, triangles, trianglesVertices,
                              trianglesValues, trianglesIndices);
                });

  // min / max
  std::vector<uint> polygonsMinMaxIndex = Utils::minMax(polygons);
  std::vector<Vertex> polygonsMinMaxVertex = Utils::minMax(polygonsVertices);
  std::vector<double> polygonsMinMaxValue = Utils::minMax(polygonsValues);

  std::vector<uint> trianglesMinMaxIndex = Utils::minMax(triangles);
  std::vector<Vertex> trianglesMinMaxVertex = Utils::minMax(trianglesVertices);
  std::vector<double> trianglesMinMaxValue = Utils::minMax(trianglesValues);

  // Result
  Result result;
  result.size = data.size;
  result.name = data.name;

  result.polygonsMinIndex = polygonsMinMaxIndex.at(0);
  result.polygonsMaxIndex = polygonsMinMaxIndex.at(1);
  result.polygonsMinVertex = polygonsMinMaxVertex.at(0);
  result.polygonsMaxVertex = polygonsMinMaxVertex.at(1);
  result.polygonsMinValue = polygonsMinMaxValue.at(0);
  result.polygonsMaxValue = polygonsMinMaxValue.at(1);
  result.polygons = polygons;
  result.polygonsVertices = polygonsVertices;
  result.polygonsValues = polygonsValues;

  result.trianglesMinIndex = trianglesMinMaxIndex.at(0);
  result.trianglesMaxIndex = trianglesMinMaxIndex.at(1);
  result.trianglesMinVertex = trianglesMinMaxVertex.at(0);
  result.trianglesMaxVertex = trianglesMinMaxVertex.at(1);
  result.trianglesMinValue = trianglesMinMaxValue.at(0);
  result.trianglesMaxValue = trianglesMinMaxValue.at(1);
  result.triangles = triangles;
  result.trianglesVertices = trianglesVertices;
  result.trianglesValues = trianglesValues;

  return result;
}

/**
 * Get results
 * @return Results
 */
std::vector<Result> VTUReader::getResults() const {
  std::vector<VTUData> arrays = this->m_arrays;
  std::vector<Result> results;

  std::for_each(arrays.begin(), arrays.end(), [&results](const VTUData &data) {
    Result result = getResult(data);

    results.push_back(result);
  });

  return results;
}
