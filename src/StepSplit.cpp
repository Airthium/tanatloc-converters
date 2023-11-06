#include <algorithm>
#include <vector>

#include <TopExp_Explorer.hxx>

#include <XCAFDoc_DocumentTool.hxx>

#include "logger/Logger.hpp"
#include "occ/StepReader.hpp"
#include "occ/StepWriter.hpp"
#include "utils/utils.hpp"

/**
 * StepToGLTF
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, const char *argv[]) {
  bool res;
  std::string stepFile;

  // Arguments
  if (argc < 2) {
    Logger::ERROR("USAGE:");
    Logger::ERROR("StepSplit stepFile");
    return EXIT_FAILURE;
  }
  stepFile = argv[1];

  // Read step file
  auto reader = StepReader(stepFile);
  res = reader.read();
  if (!res) {
    Logger::ERROR("Unable to read step file " + stepFile);
    return EXIT_FAILURE;
  }
  TopoDS_Compound compound = reader.getCompound();

  Handle(TDocStd_Document) document = reader.getDocument();

  // Prepare base file name
  const std::string base = Utils::removeExtension(stepFile);

  // Split volumes
  auto storageFormat = document->StorageFormat();
  TDF_LabelSequence labels;
  Handle(XCAFDoc_ShapeTool) shapeTool =
      XCAFDoc_DocumentTool::ShapeTool(document->Main());
  Handle(XCAFDoc_ColorTool) colorTool =
      XCAFDoc_DocumentTool::ColorTool(document->Main());
  shapeTool->GetFreeShapes(labels);

  // Loop volumes
  for (int i = 1; i <= labels.Size(); ++i) {
    const TDF_Label label = labels.Value(i);

    // New document
    Handle(TDocStd_Document) splittedDocument =
        new TDocStd_Document(storageFormat);
    Handle(XCAFDoc_ShapeTool) splittedShapeTool =
        XCAFDoc_DocumentTool::ShapeTool(splittedDocument->Main());
    Handle(XCAFDoc_ColorTool) splittedColorTool =
        XCAFDoc_DocumentTool::ColorTool(splittedDocument->Main());

    // Solid
    auto solid = shapeTool->GetShape(label);
    auto solidColor = Quantity_Color();
    colorTool->GetColor(solid, XCAFDoc_ColorSurf, solidColor);

    // Faces
    auto faceColors = std::vector<Quantity_Color>();
    TopExp_Explorer faceExplorer;
    for (faceExplorer.Init(solid, TopAbs_FACE); faceExplorer.More();
         faceExplorer.Next()) {
      // Face
      TopoDS_Shape face = faceExplorer.Current();

      // Color
      auto color = Quantity_Color();
      colorTool->GetColor(face, XCAFDoc_ColorSurf, color);
      faceColors.push_back(color);
    }

    // New solid
    splittedShapeTool->AddShape(solid);
    splittedColorTool->SetColor(solid, solidColor, XCAFDoc_ColorSurf);
    uint colorIndex = 0;
    for (faceExplorer.Init(solid, TopAbs_FACE); faceExplorer.More();
         faceExplorer.Next()) {
      // Face
      TopoDS_Shape face = faceExplorer.Current();

      // Color
      auto color = Quantity_Color();
      splittedColorTool->SetColor(face, faceColors[colorIndex],
                                  XCAFDoc_ColorSurf);

      colorIndex++;
    }

    // Write
    const std::string splittedStepFile =
        base + "_" + std::to_string(label.Tag()) + ".stp";
    StepWriter writer(splittedStepFile, splittedDocument);
    writer.write();

    Logger::DISP(splittedStepFile);
  }

  return EXIT_SUCCESS;
}
