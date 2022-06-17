#include "VTUReader.hpp"
#include "../logger/Logger.hpp"
#include <vtkCellArray.h>
#include <vtkIdList.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>

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

  const int numberOfPieces = output->GetNumberOfPieces();
  if (numberOfPieces != 1) {
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
 * Find index
 * @param index Index
 * @param indices Indices
 * @return Find
 */
int findIndex(uint index, std::vector<std::pair<uint, uint>> &indices) {
  auto find = std::find_if(indices.begin(), indices.end(),
                           [index](const std::pair<uint, uint> indexPair) {
                             return indexPair.first == index;
                           });

  if (find == indices.end())
    return -1;
  else
    return (*find).second;
}

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

void indexJob(const uint index, const VTUData &data,
              std::vector<Vertex> &vertices, std::vector<double> &values,
              std::vector<std::pair<uint, uint>> &indices,
              const std::function<void(uint)> &set) {
  const auto find = findIndex(index, indices);
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
void polygonJob(const VTUData &data, const Polygon polygon,
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
      polygons.begin(), polygons.end(), [&max](const Polygon polygon) {
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
                [&min, &max](const Vertex vertex) {
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

/**
 * Get results
 * @return Results
 */
std::vector<Result> VTUReader::getResults() const {
  std::vector<VTUData> arrays = this->m_arrays;
  std::vector<Result> results;

  std::for_each(arrays.begin(), arrays.end(), [&results](const VTUData &data) {
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
                  [data, &polygons, &polygonsVertices, &polygonsValues,
                   &polygonsIndices](const Polygon polygon) {
                    polygonJob(data, polygon, polygons, polygonsVertices,
                               polygonsValues, polygonsIndices);
                  });

    // Triangles
    std::vector<std::pair<uint, uint>> trianglesIndices;
    std::for_each(tempTriangles.begin(), tempTriangles.end(),
                  [data, &triangles, &trianglesVertices, &trianglesValues,
                   &trianglesIndices](const Triangle triangle) {
                    triangleJob(data, triangle, triangles, trianglesVertices,
                                trianglesValues, trianglesIndices);
                  });

    // min / max
    std::vector<uint> polygonsMinMaxIndex = minMax(polygons);
    std::vector<Vertex> polygonsMinMaxVertex = minMax(polygonsVertices);
    std::vector<double> polygonsMinMaxValue = minMax(polygonsValues);

    std::vector<uint> trianglesMinMaxIndex = minMax(triangles);
    std::vector<Vertex> trianglesMinMaxVertex = minMax(trianglesVertices);
    std::vector<double> trianglesMinMaxValue = minMax(trianglesValues);

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

    results.push_back(result);
  });

  return results;
}

// std::vector<Surface> VTUReader::getSurfaces() const {
//   std::vector<VTUData> arrays = this->m_arrays;
//   std::vector<Surface> surfaces;

//   std::for_each(arrays.begin(), arrays.end(), [&surfaces](VTUData &data) {
//     // Surface triangles & vertices
//     std::vector<Triangle> tempSurfaceTriangles = data.triangles;
//     std::vector<Triangle> surfaceTriangles;
//     std::vector<Vertex> surfaceVertices;
//     std::vector<double> surfaceValues;

//     // Surface vertices
//     std::vector<std::pair<uint, uint>> newIndices;
//     std::for_each(
//         tempSurfaceTriangles.begin(), tempSurfaceTriangles.end(),
//         [data, &surfaceVertices, &newIndices, &surfaceTriangles,
//          &surfaceValues](const Triangle triangle) {
//           const uint index1 = triangle.I1();
//           const uint index2 = triangle.I2();
//           const uint index3 = triangle.I3();

//           Triangle newTriangle;

//           auto find1 =
//               std::find_if(newIndices.begin(), newIndices.end(),
//                            [index1](const std::pair<uint, uint> newIndex) {
//                              return newIndex.first == index1;
//                            });
//           if (find1 == newIndices.end()) {
//             const uint newIndex1 = surfaceVertices.size();
//             surfaceVertices.push_back(data.vertices.at(index1));
//             if (data.size == 1) {
//               surfaceValues.push_back(data.values.at(index1));
//             } else if (data.size == 3) {
//               surfaceValues.push_back(data.values.at(3 * index1 + 0));
//               surfaceValues.push_back(data.values.at(3 * index1 + 1));
//               surfaceValues.push_back(data.values.at(3 * index1 + 2));
//             }
//             newTriangle.setI1(newIndex1);
//             newIndices.push_back({index1, newIndex1});
//           } else {
//             newTriangle.setI1((*find1).second);
//           }

//           auto find2 =
//               std::find_if(newIndices.begin(), newIndices.end(),
//                            [index2](const std::pair<uint, uint> oldIndex) {
//                              return oldIndex.first == index2;
//                            });
//           if (find2 == newIndices.end()) {
//             const uint newIndex2 = surfaceVertices.size();
//             surfaceVertices.push_back(data.vertices.at(index2));
//             if (data.size == 1) {
//               surfaceValues.push_back(data.values.at(index2));
//             } else if (data.size == 3) {
//               surfaceValues.push_back(data.values.at(3 * index2 + 0));
//               surfaceValues.push_back(data.values.at(3 * index2 + 1));
//               surfaceValues.push_back(data.values.at(3 * index2 + 2));
//             }
//             newTriangle.setI2(newIndex2);
//             newIndices.push_back({index2, newIndex2});
//           } else {
//             newTriangle.setI2((*find2).second);
//           }

//           auto find3 =
//               std::find_if(newIndices.begin(), newIndices.end(),
//                            [index3](const std::pair<uint, uint> oldIndex) {
//                              return oldIndex.first == index3;
//                            });
//           if (find3 == newIndices.end()) {
//             const uint newIndex3 = surfaceVertices.size();
//             surfaceVertices.push_back(data.vertices.at(index3));
//             if (data.size == 1) {
//               surfaceValues.push_back(data.values.at(index3));
//             } else if (data.size == 3) {
//               surfaceValues.push_back(data.values.at(3 * index3 + 0));
//               surfaceValues.push_back(data.values.at(3 * index3 + 1));
//               surfaceValues.push_back(data.values.at(3 * index3 + 2));
//             }
//             newTriangle.setI3(newIndex3);
//             newIndices.push_back({index3, newIndex3});
//           } else {
//             newTriangle.setI3((*find3).second);
//           }

//           surfaceTriangles.push_back(newTriangle);
//         });

//     // min / max
//     uint minIndex = 0; // Min is always 0
//     uint maxIndex = 0;
//     std::for_each(surfaceTriangles.begin(), surfaceTriangles.end(),
//                   [&maxIndex](const Triangle triangle) {
//                     const uint index1 = triangle.I1();
//                     const uint index2 = triangle.I2();
//                     const uint index3 = triangle.I3();

//                     maxIndex = std::max(
//                         maxIndex, std::max(index1, std::max(index2,
//                         index3)));
//                   });

//     Vertex minVertex(surfaceVertices.size() ? surfaceVertices.at(0)
//                                             : Vertex(0, 0, 0));
//     Vertex maxVertex(surfaceVertices.size() ? surfaceVertices.at(0)
//                                             : Vertex(0, 0, 0));
//     std::for_each(surfaceVertices.begin(), surfaceVertices.end(),
//                   [&minVertex, &maxVertex](const Vertex vertex) {
//                     const double x = vertex.X();
//                     const double y = vertex.Y();
//                     const double z = vertex.Z();

//                     minVertex.setX(std::min(minVertex.X(), x));
//                     minVertex.setY(std::min(minVertex.Y(), y));
//                     minVertex.setZ(std::min(minVertex.Z(), z));

//                     maxVertex.setX(std::max(maxVertex.X(), x));
//                     maxVertex.setY(std::max(maxVertex.Y(), y));
//                     maxVertex.setZ(std::max(maxVertex.Z(), z));
//                   });

//     double minValue = data.values.size() ? data.values.at(0) : 0;
//     double maxValue = data.values.size() ? data.values.at(0) : 0;
//     std::for_each(data.values.begin(), data.values.end(),
//                   [&minValue, &maxValue](const double value) {
//                     minValue = std::min(minValue, value);
//                     maxValue = std::max(maxValue, value);
//                   });

//     Surface surface;
//     surface.size = data.size;
//     surface.name = data.name;
//     surface.minIndex = minIndex;
//     surface.maxIndex = maxIndex;
//     surface.minVertex = minVertex;
//     surface.maxVertex = maxVertex;
//     surface.minValue = minValue;
//     surface.maxValue = maxValue;
//     surface.triangles = surfaceTriangles;
//     surface.vertices = surfaceVertices;
//     surface.values = surfaceValues;

//     surfaces.push_back(surface);
//   });

//   return surfaces;
// }

// std::vector<Line> VTUReader::getLines() const {
//   std::vector<VTUData> arrays = this->m_arrays;
//   std::vector<Line> lines;

//   std::for_each(arrays.begin(), arrays.end(), [&lines](VTUData &data) {
//     // Lines polygons & vertices
//     std::vector<Polygon> tempLinePolygons = data.polygons;
//     std::vector<Polygon> linePolygons;
//     std::vector<Vertex> lineVertices;
//     std::vector<double> lineValues;

//     // Line vertices
//     std::vector<std::pair<uint, uint>> newIndices;
//     std::for_each(tempLinePolygons.begin(), tempLinePolygons.end(),
//                   [data, &lineVertices, &newIndices, &linePolygons,
//                    &lineValues](const Polygon polygon) {
//                     std::vector<uint> indices = polygon.getIndices();

//                     Polygon newPolygon;

//                     for (size_t i = 0; i < indices.size(); ++i) {
//                       uint index = indices.at(i);
//                       auto find = std::find_if(
//                           newIndices.begin(), newIndices.end(),
//                           [index](const std::pair<uint, uint> newIndex) {
//                             return newIndex.first == index;
//                           });
//                       if (find == newIndices.end()) {
//                         const uint newIndex = lineVertices.size();
//                         lineVertices.push_back(data.vertices.at(index));
//                         if (data.size == 1) {
//                           lineValues.push_back(data.values.at(index));
//                         } else if (data.size == 3) {
//                           lineValues.push_back(data.values.at(3 * index +
//                           0)); lineValues.push_back(data.values.at(3 * index
//                           + 1)); lineValues.push_back(data.values.at(3 *
//                           index + 2));
//                         }
//                         newPolygon.addIndex(newIndex);
//                         newIndices.push_back({index, newIndex});
//                       } else {
//                         newPolygon.addIndex((*find).second);
//                       }
//                     }

//                     linePolygons.push_back(newPolygon);
//                   });

//     if (!linePolygons.size())
//       return;

//     // min / max
//     uint minIndex = 0; // Min is always 0
//     uint maxIndex = 0;
//     std::for_each(linePolygons.begin(), linePolygons.end(),
//                   [&maxIndex](const Polygon Polygon) {
//                     std::vector<uint> indices = Polygon.getIndices();

//                     std::vector<uint>::iterator result =
//                         std::max_element(indices.begin(), indices.end());
//                     maxIndex = *result;
//                   });

//     Vertex minVertex(lineVertices.size() ? lineVertices.at(0)
//                                          : Vertex(0, 0, 0));
//     Vertex maxVertex(lineVertices.size() ? lineVertices.at(0)
//                                          : Vertex(0, 0, 0));
//     std::for_each(lineVertices.begin(), lineVertices.end(),
//                   [&minVertex, &maxVertex](const Vertex vertex) {
//                     const double x = vertex.X();
//                     const double y = vertex.Y();
//                     const double z = vertex.Z();

//                     minVertex.setX(std::min(minVertex.X(), x));
//                     minVertex.setY(std::min(minVertex.Y(), y));
//                     minVertex.setZ(std::min(minVertex.Z(), z));

//                     maxVertex.setX(std::max(maxVertex.X(), x));
//                     maxVertex.setY(std::max(maxVertex.Y(), y));
//                     maxVertex.setZ(std::max(maxVertex.Z(), z));
//                   });

//     double minValue = data.values.size() ? data.values.at(0) : 0;
//     double maxValue = data.values.size() ? data.values.at(0) : 0;
//     std::for_each(data.values.begin(), data.values.end(),
//                   [&minValue, &maxValue](const double value) {
//                     minValue = std::min(minValue, value);
//                     maxValue = std::max(maxValue, value);
//                   });

//     Line line;
//     line.size = data.size;
//     line.name = data.name;
//     line.minIndex = minIndex;
//     line.maxIndex = maxIndex;
//     line.minVertex = minVertex;
//     line.maxVertex = maxVertex;
//     line.minValue = minValue;
//     line.maxValue = maxValue;
//     line.polygons = linePolygons;
//     line.vertices = lineVertices;
//     line.values = lineValues;

//     lines.push_back(line);
//   });

//   return lines;
// }
