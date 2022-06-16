#include <algorithm>

#include "gmsh/Gmsh.hpp"
#include "logger/Logger.hpp"
#include "utils/utils.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

std::vector<double> generateColor();

/**
 * GmshToGLTF
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[]) {
  bool res;
  uint i;
  std::string meshFile;
  std::string gltfFile;

  // Arguments
  if (argc < 3) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("GmshToGLTF meshFile gltfFile");
    return EXIT_FAILURE;
  }
  meshFile = argv[1];
  gltfFile = argv[2];

  // Read & process mesh
  auto gmsh = std::make_unique<Gmsh>();
  res = gmsh->load(meshFile);
  if (!res) {
    Logger::ERROR("Unable to load Gmsh file " + meshFile);
    return EXIT_FAILURE;
  }

  // GLTF
  tinygltf::Model model;
  tinygltf::Scene scene;
  tinygltf::Asset asset;

  // Surface
  std::vector<tinygltf::Value> facesExtras;
  std::vector<uint> surfaceLabels = gmsh->getSurfaceLabels();
  std::for_each(
      surfaceLabels.begin(), surfaceLabels.end(),
      [&gmsh, &model, &scene, &facesExtras](const uint label) {
        Surface surface = gmsh->getSurface(label);

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
        std::for_each(surface.triangles.begin(), surface.triangles.end(),
                      [&buffer](const Triangle triangle) {
                        uint index1 = triangle.I1();
                        uint index2 = triangle.I2();
                        uint index3 = triangle.I3();

                        // To buffer
                        Utils::uintToBuffer(index1, buffer.data);
                        Utils::uintToBuffer(index2, buffer.data);
                        Utils::uintToBuffer(index3, buffer.data);
                      });

        // Padding
        size_t paddingLength = buffer.data.size() % 4;
        for (size_t padding; padding < paddingLength; ++padding) {
          buffer.data.push_back(0x00);
        }

        // Vertices
        std::for_each(surface.vertices.begin(), surface.vertices.end(),
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
        bufferViewIndices.byteLength =
            surface.triangles.size() * 3 * __SIZEOF_INT__;
        bufferViewIndices.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
        model.bufferViews.push_back(bufferViewIndices);

        bufferViewVertices.buffer = model.buffers.size() - 1;
        bufferViewVertices.byteOffset =
            surface.triangles.size() * 3 * __SIZEOF_INT__ + paddingLength;
        bufferViewVertices.byteLength =
            surface.vertices.size() * 3 * __SIZEOF_FLOAT__;
        bufferViewVertices.target = TINYGLTF_TARGET_ARRAY_BUFFER;
        model.bufferViews.push_back(bufferViewVertices);

        // Accessors
        accessorIndices.bufferView = model.bufferViews.size() - 2;
        accessorIndices.byteOffset = 0;
        accessorIndices.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
        accessorIndices.count = surface.triangles.size() * 3;
        accessorIndices.type = TINYGLTF_TYPE_SCALAR;
        accessorIndices.minValues.push_back(surface.minIndex);
        accessorIndices.maxValues.push_back(surface.maxIndex);
        model.accessors.push_back(accessorIndices);

        accessorVertices.bufferView = model.bufferViews.size() - 1;
        accessorVertices.byteOffset = 0;
        accessorVertices.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        accessorVertices.count = surface.vertices.size();
        accessorVertices.type = TINYGLTF_TYPE_VEC3;
        accessorVertices.minValues = {surface.minVertex.X(),
                                      surface.minVertex.Y(),
                                      surface.minVertex.Z()};
        accessorVertices.maxValues = {surface.maxVertex.X(),
                                      surface.maxVertex.Y(),
                                      surface.maxVertex.Z()};
        model.accessors.push_back(accessorVertices);

        // Material
        std::vector<double> color = generateColor();
        material.pbrMetallicRoughness.baseColorFactor = color;
        material.doubleSided = true;
        model.materials.push_back(material);

        // Primitive
        primitive.indices = model.accessors.size() - 2;
        primitive.attributes["POSITION"] = model.accessors.size() - 1;
        primitive.material = model.materials.size() - 1;
        primitive.mode = TINYGLTF_MODE_TRIANGLES;

        // Mesh
        mesh.name = "Face " + std::to_string(model.meshes.size() + 1);
        std::string uuid = Utils::uuid();
        mesh.extras = tinygltf::Value({{"uuid", tinygltf::Value(uuid)},
                                       {"label", tinygltf::Value((int)label)}});
        mesh.primitives.push_back(primitive);
        model.meshes.push_back(mesh);

        // Extras
        facesExtras.push_back(tinygltf::Value(
            {{"name", tinygltf::Value(mesh.name)},
             {"uuid", tinygltf::Value(uuid)},
             {"label", tinygltf::Value((int)label)},
             {"color",
              tinygltf::Value({{"r", tinygltf::Value(color.at(0))},
                               {"g", tinygltf::Value(color.at(1))},
                               {"b", tinygltf::Value(color.at(2))}})}}));

        // Node
        node.mesh = model.meshes.size() - 1;
        model.nodes.push_back(node);

        // Scene
        scene.nodes.push_back(model.nodes.size() - 1);
      });

  // Scene
  scene.name = "master";
  scene.extras =
      tinygltf::Value({{"type", tinygltf::Value(std::string("mesh"))},
                       {"uuid", tinygltf::Value(Utils::uuid())},
                       {"dimension", tinygltf::Value(3)},
                       {"faces", tinygltf::Value(facesExtras)}});

  // Scenes
  model.scenes.push_back(scene);

  // Asset
  asset.version = "2.0";
  asset.generator = "Tanatloc-GmshToGLTF";
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

  return EXIT_SUCCESS;
}

/**
 * Generate random color
 */
std::vector<double> generateColor() {
  return {Utils::generateRandom(), Utils::generateRandom(),
          Utils::generateRandom(), 1.0f};
}
