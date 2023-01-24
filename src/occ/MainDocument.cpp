#include "MainDocument.hpp"

#include <TopoDS_Builder.hxx>
#include <XCAFDoc_DocumentTool.hxx>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
MainDocument::MainDocument() {
  this->m_app = XCAFApp_Application::GetApplication();
  this->m_app->NewDocument("TANATLOC_CONVERTERS", this->m_document);

  this->m_shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->m_document->Main());
  this->m_colorTool = XCAFDoc_DocumentTool::ColorTool(this->m_document->Main());
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
 * Get document
 * @return Document
 */
Handle(TDocStd_Document) MainDocument::getDocument() const {
  return this->m_document;
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
  for (int i = 1; i <= labels.Size(); ++i) {
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
