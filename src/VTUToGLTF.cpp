#include <algorithm>

#include "logger/Logger.hpp"
#include "occ/Triangulation.hpp"
#include "utils/utils.hpp"
#include "vtk/VTUReader.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

Result getMagnitude(const Result &);
// Surface getMagnitude(const Surface &);
// Line getMagnitude(const Line &);
Result getComponent(const Result &, const int);
// Surface getComponent(const Surface &, const int);
// Line getComponent(const Line &, const int);
bool writeOne(const Result &, const std::string &);
// bool writeOne(const Surface &, const std::string &);
// bool writeOne(const Line &, const std::string &);

/**
 * VTUToGLTF
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[]) {
  bool res;
  std::string vtuFile;
  std::string genericGltfFile;

  if (argc < 3) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("./VTUToGLTF vtuFile genericGltfFile");
    return EXIT_FAILURE;
  }
  vtuFile = argv[1];
  genericGltfFile = argv[2];

  // Read VTU file
  auto reader = VTUReader(vtuFile);
  res = reader.read();
  if (!res) {
    Logger::ERROR("Unable to read VTU file " + vtuFile);
    return EXIT_FAILURE;
  }

  std::vector<Result> results = reader.getResults();

  for (size_t i = 0; i < results.size(); i++) {
    Result result = results.at(i);
    // std::for_each(
    //     results.begin(), results.end(), [&genericGltfFile](const Result
    //     &result) {
    if (result.size == 1) { // Scalar
      bool status =
          writeOne(result, genericGltfFile + "_" + result.name + ".glb");
      if (!status)
        return EXIT_FAILURE;
    } else if (result.size == 3) { // Vector
      // Magnitude
      Result magnitude = getMagnitude(result);
      bool status = writeOne(magnitude, genericGltfFile + "_" + result.name +
                                            "_magnitude_line.glb");
      if (!status)
        return EXIT_FAILURE;

      // Component 1, 2 & 3
      for (int j = 0; j < 3; ++j) {
        Result component = getComponent(result, j);
        status = writeOne(component, genericGltfFile + "_" + result.name +
                                         "_component" + std::to_string(j + 1) +
                                         "_line.glb");
        if (!status)
          return EXIT_FAILURE;
      }
    }
    // });
  }

  // std::vector<Surface> surfaces = reader.getSurfaces();
  // std::vector<Line> lines = reader.getLines();

  // // Process lines
  // const auto numberOfLines = lines.size();
  // for (size_t i = 0; i < numberOfLines; ++i) {
  //   Line line = lines.at(i);

  //   if (line.size == 1) { // Scalar
  //     if (!writeOne(line, genericGltfFile + "_" + line.name + "_line.glb"))
  //       return EXIT_FAILURE;
  //   } else if (line.size == 3) { // Vector
  //     // Magnitude
  //     Line magnitude = getMagnitude(line);
  //     if (!writeOne(magnitude,
  //                   genericGltfFile + "_" + line.name +
  //                   "_magnitude_line.glb"))
  //       return EXIT_FAILURE;

  //     // Component 1, 2 & 3
  //     for (int j = 0; j < 3; ++j) {
  //       Line component = getComponent(line, j);
  //       if (!writeOne(component, genericGltfFile + "_" + line.name +
  //                                    "_component" + std::to_string(j + 1) +
  //                                    "_line.glb"))
  //         return EXIT_FAILURE;
  //     }
  //   }
  // }

  // // Process surfaces
  // const auto numberOfSurfaces = surfaces.size();
  // for (size_t i = 0; i < numberOfSurfaces; ++i) {
  //   Surface surface = surfaces.at(i);

  //   if (surface.size == 1) { // Scalar
  //     if (!writeOne(surface,
  //                   genericGltfFile + "_" + surface.name + "_surface.glb"))
  //       return EXIT_FAILURE;
  //   } else if (surface.size == 3) { // Vector
  //     // Magnitude
  //     Surface magnitude = getMagnitude(surface);
  //     if (!writeOne(magnitude, genericGltfFile + "_" + surface.name +
  //                                  "_magnitude_surface.glb"))
  //       return EXIT_FAILURE;

  //     // Component 1, 2 & 3
  //     for (int j = 0; j < 3; ++j) {
  //       Surface component = getComponent(surface, j);
  //       if (!writeOne(component, genericGltfFile + "_" + surface.name +
  //                                    "_component" + std::to_string(j + 1) +
  //                                    "_surface.glb"))
  //         return EXIT_FAILURE;
  //     }
  //   }
  // }

  return EXIT_SUCCESS;
}

/**
 * Get magnitude
 * @param result Result
 * @return Magnitude
 */
Result getMagnitude(const Result &result) {
  Result magnitude = result;
  magnitude.size = 1;
  magnitude.name = result.name + " (magnitude)";

  // Polygons values
  magnitude.polygonsValues.clear();
  for (uint i = 0; i < result.polygonsValues.size() / 3; ++i) {
    auto v = sqrt(pow(result.polygonsValues[3 * i + 0], 2) +
                  pow(result.polygonsValues[3 * i + 1], 2) +
                  pow(result.polygonsValues[3 * i + 2], 2));
    magnitude.polygonsValues.push_back(v);
  }

  double polygonsMinValue =
      magnitude.polygonsValues.size() ? magnitude.polygonsValues.at(0) : 0;
  double polygonsMaxValue =
      magnitude.polygonsValues.size() ? magnitude.polygonsValues.at(0) : 0;
  std::for_each(magnitude.polygonsValues.begin(),
                magnitude.polygonsValues.end(),
                [&polygonsMinValue, &polygonsMaxValue](const double value) {
                  polygonsMinValue = std::min(polygonsMinValue, value);
                  polygonsMaxValue = std::max(polygonsMaxValue, value);
                });

  magnitude.polygonsMinValue = polygonsMinValue;
  magnitude.polygonsMaxValue = polygonsMaxValue;

  // Triangles values
  magnitude.trianglesValues.clear();
  for (uint i = 0; i < result.trianglesValues.size() / 3; ++i) {
    auto v = sqrt(pow(result.trianglesValues[3 * i + 0], 2) +
                  pow(result.trianglesValues[3 * i + 1], 2) +
                  pow(result.trianglesValues[3 * i + 2], 2));
    magnitude.trianglesValues.push_back(v);
  }

  double trianglesMinValue =
      magnitude.trianglesValues.size() ? magnitude.trianglesValues.at(0) : 0;
  double trianglesMaxValue =
      magnitude.trianglesValues.size() ? magnitude.trianglesValues.at(0) : 0;
  std::for_each(magnitude.trianglesValues.begin(),
                magnitude.trianglesValues.end(),
                [&trianglesMinValue, &trianglesMaxValue](const double value) {
                  trianglesMinValue = std::min(trianglesMinValue, value);
                  trianglesMaxValue = std::max(trianglesMaxValue, value);
                });

  magnitude.trianglesMinValue = trianglesMinValue;
  magnitude.trianglesMaxValue = trianglesMaxValue;

  return magnitude;
}

// /**
//  * Get magnitude
//  * @param result Result
//  * @return Magnitude
//  */
// Surface getMagnitude(const Surface &result) {
//   Surface magnitude = result;
//   magnitude.size = 1;
//   magnitude.name = result.name + " (magnitude)";

//   magnitude.values.clear();
//   for (uint i = 0; i < result.values.size() / 3; ++i) {
//     auto v = sqrt(pow(result.values[3 * i + 0], 2) +
//                   pow(result.values[3 * i + 1], 2) +
//                   pow(result.values[3 * i + 2], 2));
//     magnitude.values.push_back(v);
//   }

//   double minValue = magnitude.values.size() ? magnitude.values.at(0) : 0;
//   double maxValue = magnitude.values.size() ? magnitude.values.at(0) : 0;
//   std::for_each(magnitude.values.begin(), magnitude.values.end(),
//                 [&minValue, &maxValue](const double value) {
//                   minValue = std::min(minValue, value);
//                   maxValue = std::max(maxValue, value);
//                 });

//   magnitude.minValue = minValue;
//   magnitude.maxValue = maxValue;

//   return magnitude;
// }

// /**
//  * Get magnitude
//  * @param result Result
//  * @return Magnitude
//  */
// Line getMagnitude(const Line &result) {
//   Line magnitude = result;
//   magnitude.size = 1;
//   magnitude.name = result.name + " (magnitude)";

//   magnitude.values.clear();
//   for (uint i = 0; i < result.values.size() / 3; ++i) {
//     auto v = sqrt(pow(result.values[3 * i + 0], 2) +
//                   pow(result.values[3 * i + 1], 2) +
//                   pow(result.values[3 * i + 2], 2));
//     magnitude.values.push_back(v);
//   }

//   double minValue = magnitude.values.size() ? magnitude.values.at(0) : 0;
//   double maxValue = magnitude.values.size() ? magnitude.values.at(0) : 0;
//   std::for_each(magnitude.values.begin(), magnitude.values.end(),
//                 [&minValue, &maxValue](const double value) {
//                   minValue = std::min(minValue, value);
//                   maxValue = std::max(maxValue, value);
//                 });

//   magnitude.minValue = minValue;
//   magnitude.maxValue = maxValue;

//   return magnitude;
// }

/**
 * Get component
 * @param result Result
 * @param index Index
 * @return Component
 */
Result getComponent(const Result &result, const int index) {
  Result component = result;
  component.size = 1;
  component.name =
      result.name + " (component " + std::to_string(index + 1) + ")";

  // Polygons values
  component.polygonsValues.clear();
  for (uint i = 0; i < result.polygonsValues.size() / 3; ++i) {
    double v = result.polygonsValues[3 * i + index];
    component.polygonsValues.push_back(v);
  }

  double polygonsMinValue =
      component.polygonsValues.size() ? component.polygonsValues.at(0) : 0;
  double polygonsMaxValue =
      component.polygonsValues.size() ? component.polygonsValues.at(0) : 0;
  std::for_each(component.polygonsValues.begin(),
                component.polygonsValues.end(),
                [&polygonsMinValue, &polygonsMaxValue](const double value) {
                  polygonsMinValue = std::min(polygonsMinValue, value);
                  polygonsMaxValue = std::max(polygonsMaxValue, value);
                });

  component.polygonsMinValue = polygonsMinValue;
  component.polygonsMaxValue = polygonsMaxValue;

  // Triangles values
  component.trianglesValues.clear();
  for (uint i = 0; i < result.trianglesValues.size() / 3; ++i) {
    double v = result.trianglesValues[3 * i + index];
    component.trianglesValues.push_back(v);
  }

  double trianglesMinValue =
      component.trianglesValues.size() ? component.trianglesValues.at(0) : 0;
  double trianglesMaxValue =
      component.trianglesValues.size() ? component.trianglesValues.at(0) : 0;
  std::for_each(component.trianglesValues.begin(),
                component.trianglesValues.end(),
                [&trianglesMinValue, &trianglesMaxValue](const double value) {
                  trianglesMinValue = std::min(trianglesMinValue, value);
                  trianglesMaxValue = std::max(trianglesMaxValue, value);
                });

  component.trianglesMinValue = trianglesMinValue;
  component.trianglesMaxValue = trianglesMaxValue;

  return component;
}

// /**
//  * Get component
//  * @param result Result
//  * @param index Index
//  * @return Component
//  */
// Surface getComponent(const Surface &result, const int index) {
//   Surface component = result;
//   component.size = 1;
//   component.name =
//       result.name + " (component " + std::to_string(index + 1) + ")";

//   component.values.clear();
//   for (uint i = 0; i < result.values.size() / 3; ++i) {
//     double v = result.values[3 * i + index];
//     component.values.push_back(v);
//   }

//   double minValue = component.values.size() ? component.values.at(0) : 0;
//   double maxValue = component.values.size() ? component.values.at(0) : 0;
//   std::for_each(component.values.begin(), component.values.end(),
//                 [&minValue, &maxValue](const double value) {
//                   minValue = std::min(minValue, value);
//                   maxValue = std::max(maxValue, value);
//                 });

//   component.minValue = minValue;
//   component.maxValue = maxValue;

//   return component;
// }

// /**
//  * Get component
//  * @param result Result
//  * @param index Index
//  * @return Component
//  */
// Line getComponent(const Line &result, const int index) {
//   Line component = result;
//   component.size = 1;
//   component.name =
//       result.name + " (component " + std::to_string(index + 1) + ")";

//   component.values.clear();
//   for (uint i = 0; i < result.values.size() / 3; ++i) {
//     double v = result.values[3 * i + index];
//     component.values.push_back(v);
//   }

//   double minValue = component.values.size() ? component.values.at(0) : 0;
//   double maxValue = component.values.size() ? component.values.at(0) : 0;
//   std::for_each(component.values.begin(), component.values.end(),
//                 [&minValue, &maxValue](const double value) {
//                   minValue = std::min(minValue, value);
//                   maxValue = std::max(maxValue, value);
//                 });

//   component.minValue = minValue;
//   component.maxValue = maxValue;

//   return component;
// }

/**
 * Write one
 * @param result Result
 * @param gltfFile GLTF file
 * @return Status
 */
bool writeOne(const Result &result, const std::string &gltfFile) {
  tinygltf::Model model;
  tinygltf::Scene scene;
  tinygltf::Asset asset;

  tinygltf::Material material;

  tinygltf::Buffer polygonsBuffer;
  tinygltf::BufferView polygonsBufferViewIndices;
  tinygltf::BufferView polygonsBufferViewVertices;
  tinygltf::BufferView polygonsBufferViewColors;
  tinygltf::Accessor polygonsAccessorIndices;
  tinygltf::Accessor polygonsAccessorVertices;
  tinygltf::Accessor polygonsAccessorColors;
  tinygltf::Primitive polygonsPrimitive;
  tinygltf::Mesh polygonsMesh;
  tinygltf::Node polygonsNode;

  tinygltf::Buffer trianglesBuffer;
  tinygltf::BufferView trianglesBufferViewIndices;
  tinygltf::BufferView trianglesBufferViewVertices;
  tinygltf::BufferView trianglesBufferViewColors;
  tinygltf::Accessor trianglesAccessorIndices;
  tinygltf::Accessor trianglesAccessorVertices;
  tinygltf::Accessor trianglesAccessorColors;
  tinygltf::Primitive trianglesPrimitive;
  tinygltf::Mesh trianglesMesh;
  tinygltf::Node trianglesNode;

  // Material
  material.pbrMetallicRoughness.metallicFactor = 0.25;
  material.doubleSided = true;
  model.materials.push_back(material);

  // Indices (polygons)
  uint sizeOfPolygons = 0;
  std::for_each(result.polygons.begin(), result.polygons.end(),
                [&polygonsBuffer, &sizeOfPolygons](const Polygon polygon) {
                  std::vector<uint> indices = polygon.getIndices();

                  std::for_each(
                      indices.begin(), indices.end(),
                      [&polygonsBuffer, &sizeOfPolygons](const uint index) {
                        sizeOfPolygons++;
                        Utils::uintToBuffer(index, polygonsBuffer.data);
                      });
                });

  // Padding (polygons)
  size_t polygonsPaddingLength = polygonsBuffer.data.size() % 4;
  for (size_t padding = 0; padding < polygonsPaddingLength; ++padding) {
    polygonsBuffer.data.push_back(0x00);
  }

  // Vertices (polygons)
  std::for_each(result.polygonsVertices.begin(), result.polygonsVertices.end(),
                [&polygonsBuffer](const Vertex vertex) {
                  double x = vertex.X();
                  double y = vertex.Y();
                  double z = vertex.Z();

                  // To buffer
                  Utils::floatToBuffer((float)x, polygonsBuffer.data);
                  Utils::floatToBuffer((float)y, polygonsBuffer.data);
                  Utils::floatToBuffer((float)z, polygonsBuffer.data);
                });

  // Padding again (polygons)
  size_t polygonsPaddingLength2 = polygonsBuffer.data.size() % 4;
  for (size_t padding = 0; padding < polygonsPaddingLength2; ++padding) {
    polygonsBuffer.data.push_back(0x00);
  }

  // Colors (polygons)
  std::for_each(result.polygonsValues.begin(), result.polygonsValues.end(),
                [&polygonsBuffer](const double value) {
                  // To buffer
                  Utils::floatToBuffer((float)value, polygonsBuffer.data);
                });

  // Indices (triangles)
  std::for_each(result.triangles.begin(), result.triangles.end(),
                [&trianglesBuffer](const Triangle triangle) {
                  uint index1 = triangle.I1();
                  uint index2 = triangle.I2();
                  uint index3 = triangle.I3();

                  // To buffer
                  Utils::uintToBuffer(index1, trianglesBuffer.data);
                  Utils::uintToBuffer(index2, trianglesBuffer.data);
                  Utils::uintToBuffer(index3, trianglesBuffer.data);
                });

  // Padding (triangles)
  size_t trianglesPaddingLength = trianglesBuffer.data.size() % 4;
  for (size_t padding = 0; padding < trianglesPaddingLength; ++padding) {
    trianglesBuffer.data.push_back(0x00);
  }

  // Vertices (triangles)
  std::for_each(result.trianglesVertices.begin(),
                result.trianglesVertices.end(),
                [&trianglesBuffer](const Vertex vertex) {
                  double x = vertex.X();
                  double y = vertex.Y();
                  double z = vertex.Z();

                  // To buffer
                  Utils::floatToBuffer((float)x, trianglesBuffer.data);
                  Utils::floatToBuffer((float)y, trianglesBuffer.data);
                  Utils::floatToBuffer((float)z, trianglesBuffer.data);
                });

  // Padding again (triangles)
  size_t trianglesPaddingLength2 = trianglesBuffer.data.size() % 4;
  for (size_t padding = 0; padding < trianglesPaddingLength2; ++padding) {
    trianglesBuffer.data.push_back(0x00);
  }

  // Colors (rtiangles)
  std::for_each(result.trianglesValues.begin(), result.trianglesValues.end(),
                [&trianglesBuffer](const double value) {
                  // To buffer
                  Utils::floatToBuffer((float)value, trianglesBuffer.data);
                });

  // // Model (polygons)
  // model.buffers.push_back(polygonsBuffer);

  // // Buffer views (polygons)
  // polygonsBufferViewIndices.buffer = (int)model.buffers.size() - 1;
  // polygonsBufferViewIndices.byteOffset = 0;
  // polygonsBufferViewIndices.byteLength = sizeOfPolygons * __SIZEOF_INT__;
  // polygonsBufferViewIndices.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
  // model.bufferViews.push_back(polygonsBufferViewIndices);

  // polygonsBufferViewVertices.buffer = (int)model.buffers.size() - 1;
  // polygonsBufferViewVertices.byteOffset =
  //     sizeOfPolygons * __SIZEOF_INT__ + polygonsPaddingLength;
  // polygonsBufferViewVertices.byteLength =
  //     result.polygonsVertices.size() * 3 * __SIZEOF_FLOAT__;
  // polygonsBufferViewVertices.target = TINYGLTF_TARGET_ARRAY_BUFFER;
  // model.bufferViews.push_back(polygonsBufferViewVertices);

  // polygonsBufferViewColors.buffer = (int)model.buffers.size() - 1;
  // polygonsBufferViewColors.byteOffset =
  //     sizeOfPolygons * __SIZEOF_INT__ + polygonsPaddingLength +
  //     result.polygonsVertices.size() * 3 * __SIZEOF_FLOAT__ +
  //     polygonsPaddingLength2;
  // polygonsBufferViewColors.byteLength =
  //     result.polygonsValues.size() * __SIZEOF_FLOAT__;
  // polygonsBufferViewColors.target = TINYGLTF_TARGET_ARRAY_BUFFER;
  // model.bufferViews.push_back(polygonsBufferViewColors);

  // // Accessors (polygons)
  // polygonsAccessorIndices.bufferView = (int)model.bufferViews.size() - 3;
  // polygonsAccessorIndices.byteOffset = 0;
  // polygonsAccessorIndices.componentType =
  // TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT; polygonsAccessorIndices.count =
  // sizeOfPolygons; polygonsAccessorIndices.type = TINYGLTF_TYPE_SCALAR;
  // polygonsAccessorIndices.minValues.push_back(result.polygonsMinIndex);
  // polygonsAccessorIndices.maxValues.push_back(result.polygonsMaxIndex);
  // model.accessors.push_back(polygonsAccessorIndices);

  // polygonsAccessorVertices.bufferView = (int)model.bufferViews.size() - 2;
  // polygonsAccessorVertices.byteOffset = 0;
  // polygonsAccessorVertices.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
  // polygonsAccessorVertices.count = result.polygonsVertices.size();
  // polygonsAccessorVertices.type = TINYGLTF_TYPE_VEC3;
  // polygonsAccessorVertices.minValues = {result.polygonsMinVertex.X(),
  //                                       result.polygonsMinVertex.Y(),
  //                                       result.polygonsMinVertex.Z()};
  // polygonsAccessorVertices.maxValues = {result.polygonsMaxVertex.X(),
  //                                       result.polygonsMaxVertex.Y(),
  //                                       result.polygonsMaxVertex.Z()};
  // model.accessors.push_back(polygonsAccessorVertices);

  // polygonsAccessorColors.bufferView = (int)model.bufferViews.size() - 1;
  // polygonsAccessorColors.byteOffset = 0;
  // polygonsAccessorColors.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
  // polygonsAccessorColors.count = result.polygonsValues.size();
  // polygonsAccessorColors.type = TINYGLTF_TYPE_SCALAR;
  // polygonsAccessorColors.minValues.push_back(result.polygonsMinValue);
  // polygonsAccessorColors.maxValues.push_back(result.polygonsMaxValue);
  // model.accessors.push_back(polygonsAccessorColors);

  // // Primitive (polygons)
  // polygonsPrimitive.indices = (int)model.accessors.size() - 3;
  // polygonsPrimitive.attributes["POSITION"] = (int)model.accessors.size() -
  // 2; polygonsPrimitive.attributes["DATA"] = (int)model.accessors.size() -
  // 1; polygonsPrimitive.material = (int)model.materials.size() - 1;
  // polygonsPrimitive.mode = TINYGLTF_MODE_LINE_STRIP;

  // // Mesh (polygons)
  // polygonsMesh.name = "Line";
  // std::string polygonsUuid = Utils::uuid();
  // polygonsMesh.extras = tinygltf::Value(
  //     {{"uuid", tinygltf::Value(polygonsUuid)}, {"label",
  //     tinygltf::Value(1)}});
  // polygonsMesh.primitives.push_back(polygonsPrimitive);
  // model.meshes.push_back(polygonsMesh);

  // // Node (polygons)
  // polygonsNode.mesh = (int)model.meshes.size() - 1;
  // model.nodes.push_back(polygonsNode);

  // Model (triangles)
  model.buffers.push_back(trianglesBuffer);

  // Buffer views (triangles)
  trianglesBufferViewIndices.buffer = (int)model.buffers.size() - 1;
  trianglesBufferViewIndices.byteOffset = 0;
  trianglesBufferViewIndices.byteLength =
      result.triangles.size() * 3 * __SIZEOF_INT__;
  trianglesBufferViewIndices.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
  model.bufferViews.push_back(trianglesBufferViewIndices);

  trianglesBufferViewVertices.buffer = (int)model.buffers.size() - 1;
  trianglesBufferViewVertices.byteOffset =
      result.triangles.size() * 3 * __SIZEOF_INT__ + trianglesPaddingLength;
  trianglesBufferViewVertices.byteLength =
      result.trianglesVertices.size() * 3 * __SIZEOF_FLOAT__;
  trianglesBufferViewVertices.target = TINYGLTF_TARGET_ARRAY_BUFFER;
  model.bufferViews.push_back(trianglesBufferViewVertices);

  trianglesBufferViewColors.buffer = (int)model.buffers.size() - 1;
  trianglesBufferViewColors.byteOffset =
      result.triangles.size() * 3 * __SIZEOF_INT__ + trianglesPaddingLength +
      result.trianglesVertices.size() * 3 * __SIZEOF_FLOAT__ +
      trianglesPaddingLength2;
  trianglesBufferViewColors.byteLength =
      result.trianglesValues.size() * __SIZEOF_FLOAT__;
  trianglesBufferViewColors.target = TINYGLTF_TARGET_ARRAY_BUFFER;
  model.bufferViews.push_back(trianglesBufferViewColors);

  // Accessors (triangle)
  trianglesAccessorIndices.bufferView = (int)model.bufferViews.size() - 3;
  trianglesAccessorIndices.byteOffset = 0;
  trianglesAccessorIndices.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
  trianglesAccessorIndices.count = result.triangles.size() * 3;
  trianglesAccessorIndices.type = TINYGLTF_TYPE_SCALAR;
  trianglesAccessorIndices.minValues.push_back(result.trianglesMinIndex);
  trianglesAccessorIndices.maxValues.push_back(result.trianglesMaxIndex);
  model.accessors.push_back(trianglesAccessorIndices);

  trianglesAccessorVertices.bufferView = (int)model.bufferViews.size() - 2;
  trianglesAccessorVertices.byteOffset = 0;
  trianglesAccessorVertices.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
  trianglesAccessorVertices.count = result.trianglesVertices.size();
  trianglesAccessorVertices.type = TINYGLTF_TYPE_VEC3;
  trianglesAccessorVertices.minValues = {result.trianglesMinVertex.X(),
                                         result.trianglesMinVertex.Y(),
                                         result.trianglesMinVertex.Z()};
  trianglesAccessorVertices.maxValues = {result.trianglesMaxVertex.X(),
                                         result.trianglesMaxVertex.Y(),
                                         result.trianglesMaxVertex.Z()};
  model.accessors.push_back(trianglesAccessorVertices);

  trianglesAccessorColors.bufferView = (int)model.bufferViews.size() - 1;
  trianglesAccessorColors.byteOffset = 0;
  trianglesAccessorColors.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
  trianglesAccessorColors.count = result.trianglesValues.size();
  trianglesAccessorColors.type = TINYGLTF_TYPE_SCALAR;
  trianglesAccessorColors.minValues.push_back(result.trianglesMinValue);
  trianglesAccessorColors.maxValues.push_back(result.trianglesMaxValue);
  model.accessors.push_back(trianglesAccessorColors);

  // Primitive (triangles)
  trianglesPrimitive.indices = (int)model.accessors.size() - 3;
  trianglesPrimitive.attributes["POSITION"] = (int)model.accessors.size() - 2;
  trianglesPrimitive.attributes["DATA"] = (int)model.accessors.size() - 1;
  trianglesPrimitive.material = (int)model.materials.size() - 1;
  trianglesPrimitive.mode = TINYGLTF_MODE_TRIANGLES;

  // Mesh (triangle)
  trianglesMesh.name = "Face";
  std::string trianglesUuid = Utils::uuid();
  trianglesMesh.extras =
      tinygltf::Value({{"uuid", tinygltf::Value(trianglesUuid)},
                       {"label", tinygltf::Value(1)}});
  trianglesMesh.primitives.push_back(trianglesPrimitive);
  model.meshes.push_back(trianglesMesh);

  // Node (triangle)
  trianglesNode.mesh = (int)model.meshes.size() - 1;
  model.nodes.push_back(trianglesNode);

  // Scene
  // scene.nodes.push_back((int)model.nodes.size() - 2);
  scene.nodes.push_back((int)model.nodes.size() - 1);

  scene.name = "master";
  scene.extras = tinygltf::Value(
      {{"type", tinygltf::Value(std::string("result"))},
       {"uuid", tinygltf::Value(Utils::uuid())},
       {"dimension", tinygltf::Value(3)},
       {"faces", tinygltf::Value({{"name", tinygltf::Value(trianglesMesh.name)},
                                  {"uuid", tinygltf::Value(trianglesUuid)},
                                  {"label", tinygltf::Value(1)}})},
       /*{"edges", tinygltf::Value({{"name", tinygltf::Value(polygonsMesh.name)},
                                  {"uuid", tinygltf::Value(polygonsUuid)},
                                  {"label", tinygltf::Value(1)}})}*/});

  // Scenes
  model.scenes.push_back(scene);

  // Asset
  asset.version = "2.0";
  asset.generator = "Tanatloc-VTUToGLTF";
  model.asset = asset;

  tinygltf::TinyGLTF gltf;
  bool res = gltf.WriteGltfSceneToFile(&model, gltfFile,
                                       true,  // embedImages
                                       true,  // embedBuffers
                                       true,  // pretty print
                                       true); // write binary
  if (!res) {
    Logger::ERROR("Unable to write glft file " + gltfFile);
    return false;
  }

  Logger::DISP("{ \"glb\": \"" + gltfFile + "\", \"name\": \"" + result.name +
               "\"}");

  return true;
}

// /**
//  * Write one
//  * @param result Result
//  * @param gltfFile GLTF file
//  * @return Status
//  */
// bool writeOne(const Surface &result, const std::string &gltfFile) {
//   tinygltf::Model model;
//   tinygltf::Scene scene;
//   tinygltf::Asset asset;

//   tinygltf::Node node;
//   tinygltf::Mesh mesh;
//   tinygltf::Buffer buffer;
//   tinygltf::BufferView bufferViewIndices;
//   tinygltf::BufferView bufferViewVertices;
//   tinygltf::BufferView bufferViewColors;
//   tinygltf::Accessor accessorIndices;
//   tinygltf::Accessor accessorVertices;
//   tinygltf::Accessor accessorColors;
//   tinygltf::Primitive primitive;
//   tinygltf::Material material;

//   // Indices
//   std::for_each(result.triangles.begin(), result.triangles.end(),
//                 [&buffer](const Triangle triangle) {
//                   uint index1 = triangle.I1();
//                   uint index2 = triangle.I2();
//                   uint index3 = triangle.I3();

//                   // To buffer
//                   Utils::uintToBuffer(index1, buffer.data);
//                   Utils::uintToBuffer(index2, buffer.data);
//                   Utils::uintToBuffer(index3, buffer.data);
//                 });

//   // Padding
//   size_t paddingLength = buffer.data.size() % 4;
//   for (size_t padding; padding < paddingLength; ++padding) {
//     buffer.data.push_back(0x00);
//   }

//   // Vertices
//   std::for_each(result.vertices.begin(), result.vertices.end(),
//                 [&buffer](const Vertex vertex) {
//                   double x = vertex.X();
//                   double y = vertex.Y();
//                   double z = vertex.Z();

//                   // To buffer
//                   Utils::floatToBuffer(x, buffer.data);
//                   Utils::floatToBuffer(y, buffer.data);
//                   Utils::floatToBuffer(z, buffer.data);
//                 });

//   // Padding
//   size_t paddingLength2 = buffer.data.size() % 4;
//   for (size_t padding; padding < paddingLength2; ++padding) {
//     buffer.data.push_back(0x00);
//   }

//   // Colors
//   std::for_each(result.values.begin(), result.values.end(),
//                 [&result, &buffer](const double value) {
//                   // To buffer
//                   Utils::floatToBuffer(value, buffer.data);
//                 });
//   model.buffers.push_back(buffer);

//   // Buffer views
//   bufferViewIndices.buffer = model.buffers.size() - 1;
//   bufferViewIndices.byteOffset = 0;
//   bufferViewIndices.byteLength = result.triangles.size() * 3 *
//   __SIZEOF_INT__; bufferViewIndices.target =
//   TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
//   model.bufferViews.push_back(bufferViewIndices);

//   bufferViewVertices.buffer = model.buffers.size() - 1;
//   bufferViewVertices.byteOffset =
//       result.triangles.size() * 3 * __SIZEOF_INT__ + paddingLength;
//   bufferViewVertices.byteLength = result.vertices.size() * 3 *
//   __SIZEOF_FLOAT__; bufferViewVertices.target =
//   TINYGLTF_TARGET_ARRAY_BUFFER;
//   model.bufferViews.push_back(bufferViewVertices);

//   bufferViewColors.buffer = model.buffers.size() - 1;
//   bufferViewColors.byteOffset =
//       result.triangles.size() * 3 * __SIZEOF_INT__ + paddingLength +
//       result.vertices.size() * 3 * __SIZEOF_FLOAT__ + paddingLength2;
//   bufferViewColors.byteLength = result.values.size() * __SIZEOF_FLOAT__;
//   bufferViewColors.target = TINYGLTF_TARGET_ARRAY_BUFFER;
//   model.bufferViews.push_back(bufferViewColors);

//   // Accessors
//   accessorIndices.bufferView = model.bufferViews.size() - 3;
//   accessorIndices.byteOffset = 0;
//   accessorIndices.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
//   accessorIndices.count = result.triangles.size() * 3;
//   accessorIndices.type = TINYGLTF_TYPE_SCALAR;
//   accessorIndices.minValues.push_back(result.minIndex);
//   accessorIndices.maxValues.push_back(result.maxIndex);
//   model.accessors.push_back(accessorIndices);

//   accessorVertices.bufferView = model.bufferViews.size() - 2;
//   accessorVertices.byteOffset = 0;
//   accessorVertices.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
//   accessorVertices.count = result.vertices.size();
//   accessorVertices.type = TINYGLTF_TYPE_VEC3;
//   accessorVertices.minValues = {result.minVertex.X(), result.minVertex.Y(),
//                                 result.minVertex.Z()};
//   accessorVertices.maxValues = {result.maxVertex.X(), result.maxVertex.Y(),
//                                 result.maxVertex.Z()};
//   model.accessors.push_back(accessorVertices);

//   accessorColors.bufferView = model.bufferViews.size() - 1;
//   accessorColors.byteOffset = 0;
//   accessorColors.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
//   accessorColors.count = result.values.size();
//   accessorColors.type = TINYGLTF_TYPE_SCALAR;
//   accessorColors.minValues.push_back(result.minValue);
//   accessorColors.maxValues.push_back(result.maxValue);
//   model.accessors.push_back(accessorColors);

//   // Material
//   material.pbrMetallicRoughness.metallicFactor = 0.25;
//   material.doubleSided = true;
//   model.materials.push_back(material);

//   // Primitive
//   primitive.indices = model.accessors.size() - 3;
//   primitive.attributes["POSITION"] = model.accessors.size() - 2;
//   primitive.attributes["DATA"] = model.accessors.size() - 1;
//   primitive.material = model.materials.size() - 1;
//   primitive.mode = TINYGLTF_MODE_TRIANGLES;

//   // Mesh
//   mesh.name = "Face " + std::to_string(model.meshes.size() + 1);
//   std::string uuid = Utils::uuid();
//   mesh.extras = tinygltf::Value(
//       {{"uuid", tinygltf::Value(uuid)}, {"label", tinygltf::Value(1)}});
//   mesh.primitives.push_back(primitive);
//   model.meshes.push_back(mesh);

//   // Node
//   node.mesh = model.meshes.size() - 1;
//   model.nodes.push_back(node);

//   // Scene
//   scene.nodes.push_back(model.nodes.size() - 1);

//   // Scene
//   scene.name = "master";
//   scene.extras = tinygltf::Value(
//       {{"type", tinygltf::Value(std::string("result"))},
//        {"uuid", tinygltf::Value(Utils::uuid())},
//        {"dimension", tinygltf::Value(3)},
//        {"faces", tinygltf::Value({{"name", tinygltf::Value(mesh.name)},
//                                   {"uuid", tinygltf::Value(uuid)},
//                                   {"label", tinygltf::Value(1)}})}});

//   // Scenes
//   model.scenes.push_back(scene);

//   // Asset
//   asset.version = "2.0";
//   asset.generator = "Tanatloc-VTUToGLTF";
//   model.asset = asset;

//   tinygltf::TinyGLTF gltf;
//   bool res = gltf.WriteGltfSceneToFile(&model, gltfFile,
//                                        true,  // embedImages
//                                        true,  // embedBuffers
//                                        true,  // pretty print
//                                        true); // write binary
//   if (!res) {
//     Logger::ERROR("Unable to write glft file " + gltfFile);
//     return false;
//   }

//   Logger::DISP("{ \"glb\": \"" + gltfFile + "\", \"name\": \"" +
//   result.name
//   +
//                "\"}");

//   return true;
// }

// /**
//  * Write one
//  * @param result Result
//  * @param gltfFile GLTF file
//  * @return Status
//  */
// bool writeOne(const Line &result, const std::string &gltfFile) {
//   tinygltf::Model model;
//   tinygltf::Scene scene;
//   tinygltf::Asset asset;

//   tinygltf::Node node;
//   tinygltf::Mesh mesh;
//   tinygltf::Buffer buffer;
//   tinygltf::BufferView bufferViewIndices;
//   tinygltf::BufferView bufferViewVertices;
//   tinygltf::BufferView bufferViewColors;
//   tinygltf::Accessor accessorIndices;
//   tinygltf::Accessor accessorVertices;
//   tinygltf::Accessor accessorColors;
//   tinygltf::Primitive primitive;
//   tinygltf::Material material;

//   // Indices
//   uint sizeOfPolygons = 0;
//   std::for_each(result.polygons.begin(), result.polygons.end(),
//                 [&buffer, &sizeOfPolygons](const Polygon polygon) {
//                   std::vector<uint> indices = polygon.getIndices();

//                   std::for_each(indices.begin(), indices.end(),
//                                 [&buffer, &sizeOfPolygons](const uint
//                                 index)
//                                 {
//                                   sizeOfPolygons++;
//                                   Utils::uintToBuffer(index, buffer.data);
//                                 });
//                 });

//   // Padding
//   size_t paddingLength = buffer.data.size() % 4;
//   for (size_t padding; padding < paddingLength; ++padding) {
//     buffer.data.push_back(0x00);
//   }

//   // Vertices
//   std::for_each(result.vertices.begin(), result.vertices.end(),
//                 [&buffer](const Vertex vertex) {
//                   double x = vertex.X();
//                   double y = vertex.Y();
//                   double z = vertex.Z();

//                   // To buffer
//                   Utils::floatToBuffer(x, buffer.data);
//                   Utils::floatToBuffer(y, buffer.data);
//                   Utils::floatToBuffer(z, buffer.data);
//                 });

//   // Padding
//   size_t paddingLength2 = buffer.data.size() % 4;
//   for (size_t padding; padding < paddingLength2; ++padding) {
//     buffer.data.push_back(0x00);
//   }

//   // Colors
//   std::for_each(result.values.begin(), result.values.end(),
//                 [&result, &buffer](const double value) {
//                   // To buffer
//                   Utils::floatToBuffer(value, buffer.data);
//                 });
//   model.buffers.push_back(buffer);

//   // Buffer views
//   bufferViewIndices.buffer = model.buffers.size() - 1;
//   bufferViewIndices.byteOffset = 0;
//   bufferViewIndices.byteLength = sizeOfPolygons * __SIZEOF_INT__;
//   bufferViewIndices.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
//   model.bufferViews.push_back(bufferViewIndices);

//   bufferViewVertices.buffer = model.buffers.size() - 1;
//   bufferViewVertices.byteOffset =
//       sizeOfPolygons * __SIZEOF_INT__ + paddingLength;
//   bufferViewVertices.byteLength = result.vertices.size() * 3 *
//   __SIZEOF_FLOAT__; bufferViewVertices.target =
//   TINYGLTF_TARGET_ARRAY_BUFFER;
//   model.bufferViews.push_back(bufferViewVertices);

//   bufferViewColors.buffer = model.buffers.size() - 1;
//   bufferViewColors.byteOffset =
//       sizeOfPolygons * __SIZEOF_INT__ + paddingLength +
//       result.vertices.size() * 3 * __SIZEOF_FLOAT__ + paddingLength2;
//   bufferViewColors.byteLength = result.values.size() * __SIZEOF_FLOAT__;
//   bufferViewColors.target = TINYGLTF_TARGET_ARRAY_BUFFER;
//   model.bufferViews.push_back(bufferViewColors);

//   // Accessors
//   accessorIndices.bufferView = model.bufferViews.size() - 3;
//   accessorIndices.byteOffset = 0;
//   accessorIndices.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
//   accessorIndices.count = sizeOfPolygons;
//   accessorIndices.type = TINYGLTF_TYPE_SCALAR;
//   accessorIndices.minValues.push_back(result.minIndex);
//   accessorIndices.maxValues.push_back(result.maxIndex);
//   model.accessors.push_back(accessorIndices);

//   accessorVertices.bufferView = model.bufferViews.size() - 2;
//   accessorVertices.byteOffset = 0;
//   accessorVertices.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
//   accessorVertices.count = result.vertices.size();
//   accessorVertices.type = TINYGLTF_TYPE_VEC3;
//   accessorVertices.minValues = {result.minVertex.X(), result.minVertex.Y(),
//                                 result.minVertex.Z()};
//   accessorVertices.maxValues = {result.maxVertex.X(), result.maxVertex.Y(),
//                                 result.maxVertex.Z()};
//   model.accessors.push_back(accessorVertices);

//   accessorColors.bufferView = model.bufferViews.size() - 1;
//   accessorColors.byteOffset = 0;
//   accessorColors.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
//   accessorColors.count = result.values.size();
//   accessorColors.type = TINYGLTF_TYPE_SCALAR;
//   accessorColors.minValues.push_back(result.minValue);
//   accessorColors.maxValues.push_back(result.maxValue);
//   model.accessors.push_back(accessorColors);

//   // Material
//   material.pbrMetallicRoughness.metallicFactor = 0.25;
//   material.doubleSided = true;
//   model.materials.push_back(material);

//   // Primitive
//   primitive.indices = model.accessors.size() - 3;
//   primitive.attributes["POSITION"] = model.accessors.size() - 2;
//   primitive.attributes["DATA"] = model.accessors.size() - 1;
//   primitive.material = model.materials.size() - 1;
//   primitive.mode = TINYGLTF_MODE_LINE_STRIP;

//   // Mesh
//   mesh.name = "Face " + std::to_string(model.meshes.size() + 1);
//   std::string uuid = Utils::uuid();
//   mesh.extras = tinygltf::Value(
//       {{"uuid", tinygltf::Value(uuid)}, {"label", tinygltf::Value(1)}});
//   mesh.primitives.push_back(primitive);
//   model.meshes.push_back(mesh);

//   // Node
//   node.mesh = model.meshes.size() - 1;
//   model.nodes.push_back(node);

//   // Scene
//   scene.nodes.push_back(model.nodes.size() - 1);

//   // Scene
//   scene.name = "master";
//   scene.extras = tinygltf::Value(
//       {{"type", tinygltf::Value(std::string("result"))},
//        {"uuid", tinygltf::Value(Utils::uuid())},
//        {"dimension", tinygltf::Value(3)},
//        {"faces", tinygltf::Value({{"name", tinygltf::Value(mesh.name)},
//                                   {"uuid", tinygltf::Value(uuid)},
//                                   {"label", tinygltf::Value(1)}})}});

//   // Scenes
//   model.scenes.push_back(scene);

//   // Asset
//   asset.version = "2.0";
//   asset.generator = "Tanatloc-VTUToGLTF";
//   model.asset = asset;

//   tinygltf::TinyGLTF gltf;
//   bool res = gltf.WriteGltfSceneToFile(&model, gltfFile,
//                                        true,  // embedImages
//                                        true,  // embedBuffers
//                                        true,  // pretty print
//                                        true); // write binary
//   if (!res) {
//     Logger::ERROR("Unable to write glft file " + gltfFile);
//     return false;
//   }

//   Logger::DISP("{ \"glb\": \"" + gltfFile + "\", \"name\": \"" +
//   result.name
//   +
//                "\"}");

//   return true;
// }
