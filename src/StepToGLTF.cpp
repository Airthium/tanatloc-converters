#include <algorithm>

#include "logger/Logger.hpp"
#include "occ/StepReader.hpp"
#include "occ/Triangulation.hpp"
#include "utils/utils.hpp"
#include <BRepTools.hxx>
#include <TopExp_Explorer.hxx>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

/**
 * StepToGLTF
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[]) {
  bool res;
  std::string stepFile;
  std::string gltfFile;
  std::string brepFile;

  // Arguments
  if (argc < 4) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("StepToGLTF stepFile gltfFile brepFile");
    return EXIT_FAILURE;
  }
  stepFile = argv[1];
  gltfFile = argv[2];
  brepFile = argv[3];

  // Read step file
  auto reader = StepReader(stepFile);
  res = reader.read();
  if (!res) {
    Logger::ERROR("Unable to read step file " + stepFile);
    return EXIT_FAILURE;
  }
  TopoDS_Compound compound = reader.getCompound();

  // Triangulation (prepare)
  Triangulation triangulation(compound);

  // GLTF
  tinygltf::Model model;
  tinygltf::Scene scene;
  tinygltf::Asset asset;

  uint nSolids = 0;
  uint nFaces = 0;
  std::vector<tinygltf::Value> solidsExtras;
  std::vector<tinygltf::Value> facesExtras;
  TopExp_Explorer solidExplorer;
  for (solidExplorer.Init(compound, TopAbs_SOLID); solidExplorer.More();
       solidExplorer.Next()) {
    nSolids++;

    // Solid
    TopoDS_Shape solid = solidExplorer.Current();
    Quantity_Color solidColor = reader.getShapeColor(solid);

    tinygltf::Node solidNode;
    solidNode.name = "Solid " + std::to_string(nSolids);
    std::string uuid = Utils::uuid();
    solidNode.extras =
        tinygltf::Value({{"uuid", tinygltf::Value(uuid)},
                         {"label", tinygltf::Value((int)nSolids)}});

    // Extras
    solidsExtras.push_back(tinygltf::Value(
        {{"name", tinygltf::Value(solidNode.name)},
         {"uuid", tinygltf::Value(uuid)},
         {"label", tinygltf::Value((int)nSolids)},
         {"color",
          tinygltf::Value({{"r", tinygltf::Value(solidColor.Red())},
                           {"g", tinygltf::Value(solidColor.Green())},
                           {"b", tinygltf::Value(solidColor.Blue())}})}}));

    TopExp_Explorer faceExplorer;
    for (faceExplorer.Init(solid, TopAbs_FACE); faceExplorer.More();
         faceExplorer.Next()) {
      nFaces++;

      // Face
      TopoDS_Shape face = faceExplorer.Current();
      Quantity_Color faceColor = reader.getShapeColor(face);

      // Triangulate
      FaceMesh faceMesh = triangulation.triangulateFace(face);

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
                      double x = vertex.X() * 1.e-3; // mm to m
                      double y = vertex.Y() * 1.e-3; // mm to m
                      double z = vertex.Z() * 1.e-3; // mm to m

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
      accessorVertices.minValues = {faceMesh.minVertex.X() * 1.e-3,  // mm to m
                                    faceMesh.minVertex.Y() * 1.e-3,  // mm to m
                                    faceMesh.minVertex.Z() * 1.e-3}; // mm to m
      accessorVertices.maxValues = {faceMesh.maxVertex.X() * 1.e-3,  // mm to m
                                    faceMesh.maxVertex.Y() * 1.e-3,  // mm to m
                                    faceMesh.maxVertex.Z() * 1.e-3}; // mm to m
      model.accessors.push_back(accessorVertices);

      // Material
      material.pbrMetallicRoughness.baseColorFactor = {
          faceColor.Red(), faceColor.Green(), faceColor.Blue(), 1.0f};
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
      facesExtras.push_back(tinygltf::Value(
          {{"name", tinygltf::Value(mesh.name)},
           {"uuid", tinygltf::Value(uuid)},
           {"label", tinygltf::Value((int)nFaces)},
           {"color",
            tinygltf::Value({{"r", tinygltf::Value(faceColor.Red())},
                             {"g", tinygltf::Value(faceColor.Green())},
                             {"b", tinygltf::Value(faceColor.Blue())}})}}));

      // Node
      node.mesh = model.meshes.size() - 1;

      // Nodes
      model.nodes.push_back(node);

      // Inside solid
      solidNode.children.push_back(model.nodes.size() - 1);

      // Scene
      scene.nodes.push_back(model.nodes.size() - 1);
    }

    // Nodes
    model.nodes.push_back(solidNode);

    // Scene
    scene.nodes.push_back(model.nodes.size() - 1);
  }

  // Scene
  scene.name = "master";
  scene.extras =
      tinygltf::Value({{"type", tinygltf::Value(std::string("geometry3D"))},
                       {"uuid", tinygltf::Value(Utils::uuid())},
                       {"dimension", tinygltf::Value(3)},
                       {"solids", tinygltf::Value(solidsExtras)},
                       {"faces", tinygltf::Value(facesExtras)}});

  // Scenes
  model.scenes.push_back(scene);

  // Asset
  asset.version = "2.0";
  asset.generator = "Tanatloc-StepToGLTF";
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
