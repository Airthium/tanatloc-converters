#ifndef _DXF_CONVERTER_
#define _DXF_CONVERTER_

#include <string>
#include <vector>

#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Shape.hxx>

#include "./lib/Arc.hpp"
#include "./lib/Circle.hpp"
#include "./lib/Line.hpp"
#include "./lib/Polyline.hpp"

const std::string TAG0 = "  0";
const std::string TAG10 = " 10";
const std::string TAG11 = " 11";
const std::string TAG20 = " 20";
const std::string TAG21 = " 21";
const std::string TAG30 = " 30";
const std::string TAG31 = " 31";
const std::string TAG40 = " 40";
const std::string TAG50 = " 50";
const std::string TAG51 = " 51";
const std::string VERTEX = "VERTEX";
const std::string SEQEND = "SEQEND";

struct Index {
  std::string type;
  unsigned long index;
};

class DXFConverter {
private:
  std::string m_input;

  std::vector<DXFLine> m_lines;
  std::vector<DXFCircle> m_circles;
  std::vector<DXFArc> m_arcs;
  std::vector<DXFPolyline> m_polylines;
  std::vector<Index> m_index;

  std::vector<TopoDS_Shape> m_faces;

  TopoDS_Compound m_compound;

  // Clear
  void clear();

  // Process line
  void processLine(std::ifstream &);

  // Process circle
  void processCircle(std::ifstream &);

  // Process arc
  void processArc(std::ifstream &);

  // Process vertex
  DXFVertex processVertex(std::ifstream &) const;

  // Process polyline
  void processPolyline(std::ifstream &);

  // Available entities
  bool availableEntities() const;

  // Remove doubles
  void removeDoubles();

  // Add line to wire builder
  void addLineToWireBuilder(const DXFLine &, BRepBuilderAPI_MakeWire &) const;

  // Add arc to wire builder
  void addArcToWireBuilder(const DXFArc &, BRepBuilderAPI_MakeWire &) const;

  // Add polyline to wire builder
  void addPolylineToWireBuilder(const DXFPolyline &,
                                BRepBuilderAPI_MakeWire &) const;

  // Process
  void process();

  // Finish
  void finish();

public:
  // Constructor
  DXFConverter();

  // Set input
  void setInput(const std::string &);

  // Convert
  bool convert();

  // Get shape
  TopoDS_Compound getCompound() const;
};

#endif