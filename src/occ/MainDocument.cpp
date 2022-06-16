#include "MainDocument.hpp"

#include <uuid/uuid.h>

#include "../logger/Logger.hpp"
#include <TopoDS_Builder.hxx>
#include <XCAFDoc_DocumentTool.hxx>

std::string uuid() {
  uuid_t uuid;
  char uuidChar[UUID_STR_LEN];
  uuid_generate_random(uuid);
  uuid_unparse_upper(uuid, uuidChar);

  return std::string(uuidChar);
}

/**
 * Constructor
 */
MainDocument::MainDocument() {
  this->m_app = XCAFApp_Application::GetApplication();
  this->m_app->NewDocument("TANATLOC_CONVERTERS", this->document);

  this->m_shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->document->Main());
  this->m_colorTool = XCAFDoc_DocumentTool::ColorTool(this->document->Main());
}

/**
 * Get labels
 * @return Labels
 */
TDF_LabelSequence MainDocument::getLabels() const {
  TDF_LabelSequence labels;
  this->m_shapeTool->GetFreeShapes(labels);
  return labels;
}

/**
 * Get compound
 * @return Compound
 */
TopoDS_Compound MainDocument::getCompound() const {
  TDF_LabelSequence labels = this->getLabels();

  TopoDS_Compound compound;
  TopoDS_Builder builder;
  builder.MakeCompound(compound);
  for (uint i = 1; i <= labels.Size(); ++i) {
    const TDF_Label &label = labels.Value(i);
    TopoDS_Shape shape;
    if (XCAFDoc_ShapeTool::GetShape(label, shape)) {
      builder.Add(compound, shape);
    }
  }

  return compound;
}

/**
 * Get shape color
 * @param shape Shap
 * @return Color
 */
Quantity_Color MainDocument::getShapeColor(const TopoDS_Shape &shape) const {

  Quantity_Color color;
  this->m_colorTool->GetColor(shape, XCAFDoc_ColorSurf, color);

  if (color == OCCDefaultColor)
    color = TanatlocDefaultColor;

  return color;
}
