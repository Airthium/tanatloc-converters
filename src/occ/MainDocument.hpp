#ifndef _MAIN_DOCUMENT_
#define _MAIN_DOCUMENT_

#include <Quantity_Color.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDocStd_Document.hxx>
#include <TopoDS_Compound.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>

const Quantity_Color OCCDefaultColor(1., 1., 0., Quantity_TOC_RGB);
const Quantity_Color TanatlocDefaultColor(0.75, 0.75, 0.75, Quantity_TOC_RGB);

class MainDocument {
private:
  Handle(XCAFApp_Application) m_app;
  Handle(XCAFDoc_ShapeTool) m_shapeTool;
  Handle(XCAFDoc_ColorTool) m_colorTool;

  // Get labels
  TDF_LabelSequence getLabels() const;

public:
  Handle(TDocStd_Document) document;

  // Constructor
  MainDocument();

  // Get compound
  TopoDS_Compound getCompound() const;

  // Get shape color
  Quantity_Color getShapeColor(const TopoDS_Shape &shape) const;
};

#endif //_MAIN_DOCUMENT_
