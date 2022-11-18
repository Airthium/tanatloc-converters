#ifndef _DXF_CONVERTER_
#define _DXF_CONVERTER_

#include <vector>

#include "../dxflib/dl_creationadapter.h"
#include <TopoDS_Compound.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>

class DXFConverter : public DL_CreationAdapter {
private:
  std::string m_input = "";
  std::string m_output = "";

  std::vector<DL_VertexData> m_vertices;
  std::vector<TopoDS_Wire> m_wires;
  std::vector<TopoDS_Shape> m_faces;

  TopoDS_Compound m_compound;

  void processCodeValuePair(unsigned int, const std::string &) override;
  void addLine(const DL_LineData &) override;

  void addArc(const DL_ArcData &) override;
  void addCircle(const DL_CircleData &) override;

  void addPolyline(const DL_PolylineData &) override;
  void addVertex(const DL_VertexData &) override;

  void endEntity() override;

  void buildWire();
  void buildFace();
  void buildCompound();

public:
  // Contructor
  DXFConverter();

  // Set input
  void setInput(const std::string &);

  // Convert
  bool convert();

  // Get shape
  TopoDS_Compound getCompound() const;
};

#endif //_DXF_CONVERTER_
