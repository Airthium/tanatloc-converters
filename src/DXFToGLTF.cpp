#include <algorithm>

#include "dxf/DXFConverter.hpp"
#include "logger/Logger.hpp"
#include "occ/MainDocument.hpp"
#include "occ/Triangulation.hpp"
#include "utils/utils.hpp"
#include <BRepTools.hxx>
#include <TopExp_Explorer.hxx>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

/**
 * DXFToGLTF
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv) {
  bool res;
  std::string dxfFile;
  std::string gltfFile;
  std::string brepFile;

  // Input arguments
  if (argc < 4) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("DXFToGLTF dxfFile glftFile brepFile");
    return EXIT_FAILURE;
  }
  dxfFile = argv[1];
  gltfFile = argv[2];
  brepFile = argv[3];

  // Converter
  auto converter = std::make_unique<DXFConverter>();
  converter->setInput(dxfFile);
  res = converter->convert();
  if (!res) {
    Logger::ERROR("Unable to convert " + dxfFile);
    return EXIT_FAILURE;
  }
  TopoDS_Compound compound = converter->getCompound();

  // Triangulate (prepare)
  Triangulation triangulation(compound);

  // GLTF
  tinygltf::Model model;
  tinygltf::Scene scene;
  tinygltf::Asset asset;

  uint nFaces = 0;
  uint nEdges = 0;
  std::vector<tinygltf::Value> facesExtras;
  std::vector<tinygltf::Value> edgesExtras;
  TopExp_Explorer faceExplorer;
  for (faceExplorer.Init(compound, TopAbs_FACE); faceExplorer.More();
       faceExplorer.Next()) {
    nFaces++;

    // Face
    TopoDS_Shape face = faceExplorer.Current();

    // Triangulate
    FaceMesh faceMesh = triangulation.triangulateFace(face);

    tinygltf::Node faceNode;
    tinygltf::Mesh mesh;
    tinygltf::Buffer buffer;
    tinygltf::BufferView bufferViewIndices;
    tinygltf::BufferView bufferViewVertices;
    tinygltf::Accessor accessorIndices;
    tinygltf::Accessor accessorVertices;
    tinygltf::Primitive primitive;
    tinygltf::Material material;

    // Indices
    std::for_each(faceMesh.indices.begin(), faceMesh.indices.end(),
                  [&buffer](const uint index) {
                    // To buffer
                    Utils::uintToBuffer(index, buffer.data);
                  });

    // Padding
    size_t paddingLength = buffer.data.size() % 4;
    for (size_t padding; padding < paddingLength; ++padding) {
      buffer.data.push_back(0x00);
    }

    // Vertices
    std::for_each(faceMesh.vertices.begin(), faceMesh.vertices.end(),
                  [&buffer](const Vertex vertex) {
                    double x = vertex.X();
                    double y = vertex.Y();
                    double z = vertex.Z();

                    // To buffer
                    Utils::floatToBuffer(x, buffer.data);
                    Utils::floatToBuffer(y, buffer.data);
                    Utils::floatToBuffer(z, buffer.data);
                  });
    model.buffers.push_back(buffer);

    // Buffer views
    bufferViewIndices.buffer = model.buffers.size() - 1;
    bufferViewIndices.byteOffset = 0;
    bufferViewIndices.byteLength = faceMesh.indices.size() * __SIZEOF_INT__;
    bufferViewIndices.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
    model.bufferViews.push_back(bufferViewIndices);

    bufferViewVertices.buffer = model.buffers.size() - 1;
    bufferViewVertices.byteOffset =
        faceMesh.indices.size() * __SIZEOF_INT__ + paddingLength;
    bufferViewVertices.byteLength =
        faceMesh.vertices.size() * 3 * __SIZEOF_FLOAT__;
    bufferViewVertices.target = TINYGLTF_TARGET_ARRAY_BUFFER;
    model.bufferViews.push_back(bufferViewVertices);

    // Accessors
    accessorIndices.bufferView = model.bufferViews.size() - 2;
    accessorIndices.byteOffset = 0;
    accessorIndices.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
    accessorIndices.count = faceMesh.indices.size();
    accessorIndices.type = TINYGLTF_TYPE_SCALAR;
    accessorIndices.minValues.push_back(faceMesh.minIndex);
    accessorIndices.maxValues.push_back(faceMesh.maxIndex);
    model.accessors.push_back(accessorIndices);

    accessorVertices.bufferView = model.bufferViews.size() - 1;
    accessorVertices.byteOffset = 0;
    accessorVertices.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessorVertices.count = faceMesh.vertices.size();
    accessorVertices.type = TINYGLTF_TYPE_VEC3;
    accessorVertices.minValues = {
        faceMesh.minVertex.X(), faceMesh.minVertex.Y(), faceMesh.minVertex.Z()};
    accessorVertices.maxValues = {
        faceMesh.maxVertex.X(), faceMesh.maxVertex.Y(), faceMesh.maxVertex.Z()};
    model.accessors.push_back(accessorVertices);

    // Material
    material.doubleSided = true;
    material.pbrMetallicRoughness.roughnessFactor = 0.5;
    material.pbrMetallicRoughness.metallicFactor = 0.5;
    model.materials.push_back(material);

    // Primitive
    primitive.indices = model.accessors.size() - 2;
    primitive.attributes["POSITION"] = model.accessors.size() - 1;
    primitive.material = model.materials.size() - 1;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;

    // Mesh
    mesh.name = "Face " + std::to_string(nFaces);
    std::string uuid = Utils::uuid();
    mesh.extras = tinygltf::Value({{"uuid", tinygltf::Value(uuid)},
                                   {"label", tinygltf::Value((int)nFaces)}});
    mesh.primitives.push_back(primitive);
    model.meshes.push_back(mesh);

    // Extras
    facesExtras.push_back(
        tinygltf::Value({{"name", tinygltf::Value(mesh.name)},
                         {"uuid", tinygltf::Value(uuid)},
                         {"label", tinygltf::Value((int)nFaces)}}));

    // Node
    faceNode.mesh = model.meshes.size() - 1;

    TopExp_Explorer edgeExplorer;
    for (edgeExplorer.Init(face, TopAbs_EDGE); edgeExplorer.More();
         edgeExplorer.Next()) {
      nEdges++;

      // Edge
      TopoDS_Shape edge = edgeExplorer.Current();

      // Triangulate
      FaceMesh edgeMesh = triangulation.triangulateEdge(edge);
      edgeMesh.label = nEdges;

      tinygltf::Node node;
      tinygltf::Mesh mesh;
      tinygltf::Buffer buffer;
      tinygltf::BufferView bufferViewIndices;
      tinygltf::BufferView bufferViewVertices;
      tinygltf::Accessor accessorIndices;
      tinygltf::Accessor accessorVertices;
      tinygltf::Primitive primitive;
      tinygltf::Material material;

      // Indices
      std::for_each(edgeMesh.indices.begin(), edgeMesh.indices.end(),
                    [&buffer](const uint index) {
                      // To buffer
                      Utils::uintToBuffer(index, buffer.data);
                    });

      // Padding
      size_t paddingLength = buffer.data.size() % 4;
      for (size_t padding; padding < paddingLength; ++padding) {
        buffer.data.push_back(0x00);
      }

      // Vertices
      std::for_each(edgeMesh.vertices.begin(), edgeMesh.vertices.end(),
                    [&buffer](const Vertex vertex) {
                      double x = vertex.X();
                      double y = vertex.Y();
                      double z = vertex.Z();

                      // To buffer
                      Utils::floatToBuffer(x, buffer.data);
                      Utils::floatToBuffer(y, buffer.data);
                      Utils::floatToBuffer(z, buffer.data);
                    });
      model.buffers.push_back(buffer);

      // Buffer views
      bufferViewIndices.buffer = model.buffers.size() - 1;
      bufferViewIndices.byteOffset = 0;
      bufferViewIndices.byteLength = edgeMesh.indices.size() * __SIZEOF_INT__;
      bufferViewIndices.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
      model.bufferViews.push_back(bufferViewIndices);

      bufferViewVertices.buffer = model.buffers.size() - 1;
      bufferViewVertices.byteOffset =
          edgeMesh.indices.size() * __SIZEOF_INT__ + paddingLength;
      bufferViewVertices.byteLength =
          edgeMesh.vertices.size() * 3 * __SIZEOF_FLOAT__;
      bufferViewVertices.target = TINYGLTF_TARGET_ARRAY_BUFFER;
      model.bufferViews.push_back(bufferViewVertices);

      // Accessors
      accessorIndices.bufferView = model.bufferViews.size() - 2;
      accessorIndices.byteOffset = 0;
      accessorIndices.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
      accessorIndices.count = edgeMesh.indices.size();
      accessorIndices.type = TINYGLTF_TYPE_SCALAR;
      accessorIndices.minValues.push_back(edgeMesh.minIndex);
      accessorIndices.maxValues.push_back(edgeMesh.maxIndex);
      model.accessors.push_back(accessorIndices);

      accessorVertices.bufferView = model.bufferViews.size() - 1;
      accessorVertices.byteOffset = 0;
      accessorVertices.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
      accessorVertices.count = edgeMesh.vertices.size();
      accessorVertices.type = TINYGLTF_TYPE_VEC3;
      accessorVertices.minValues = {edgeMesh.minVertex.X(),
                                    edgeMesh.minVertex.Y(),
                                    edgeMesh.minVertex.Z()};
      accessorVertices.maxValues = {edgeMesh.maxVertex.X(),
                                    edgeMesh.maxVertex.Y(),
                                    edgeMesh.maxVertex.Z()};
      model.accessors.push_back(accessorVertices);

      // Material
      material.doubleSided = true;
      material.pbrMetallicRoughness.roughnessFactor = 0.5;
      material.pbrMetallicRoughness.metallicFactor = 0.5;
      model.materials.push_back(material);

      // Primitive
      primitive.indices = model.accessors.size() - 2;
      primitive.attributes["POSITION"] = model.accessors.size() - 1;
      primitive.material = model.materials.size() - 1;
      primitive.mode = TINYGLTF_MODE_TRIANGLES;

      // Mesh
      mesh.name = "Edge " + std::to_string(nEdges);
      std::string uuid = Utils::uuid();
      mesh.extras = tinygltf::Value({{"uuid", tinygltf::Value(uuid)},
                                     {"label", tinygltf::Value((int)nEdges)}});
      mesh.primitives.push_back(primitive);
      model.meshes.push_back(mesh);

      // Extras
      edgesExtras.push_back(
          tinygltf::Value({{"name", tinygltf::Value(mesh.name)},
                           {"uuid", tinygltf::Value(uuid)},
                           {"label", tinygltf::Value((int)nEdges)}}));

      // Node
      node.mesh = model.meshes.size() - 1;

      // Nodes
      model.nodes.push_back(node);

      // Inside solid
      faceNode.children.push_back(model.nodes.size() - 1);

      // Scene
      scene.nodes.push_back(model.nodes.size() - 1);
    }

    // Nodes
    model.nodes.push_back(faceNode);

    // Scene
    scene.nodes.push_back(model.nodes.size() - 1);
  }

  // Scene
  scene.name = "master";
  scene.extras =
      tinygltf::Value({{"type", tinygltf::Value(std::string("geometry2D"))},
                       {"uuid", tinygltf::Value(Utils::uuid())},
                       {"dimension", tinygltf::Value(2)},
                       {"faces", tinygltf::Value(facesExtras)},
                       {"edges", tinygltf::Value(edgesExtras)}});

  // Scenes
  model.scenes.push_back(scene);

  // Asset
  asset.version = "2.0";
  asset.generator = "Tanatloc-DXFToGLTF";
  model.asset = asset;

  // Save
  tinygltf::TinyGLTF gltf;
  res = gltf.WriteGltfSceneToFile(&model, gltfFile,
                                  true,  // embedImages
                                  true,  // embedBuffers
                                  true,  // pretty print
                                  true); // write binary
  if (!res) {
    Logger::ERROR("Unable to write glft file " + gltfFile);
    return EXIT_FAILURE;
  }

  // BRep
  BRepTools::Write(compound, brepFile.c_str());

  return EXIT_SUCCESS;
}
