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
#include "./lib/Spline.hpp"

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
  std::vector<DXFSpline> m_splines;
  std::vector<Index> m_index;

  bool needReverse;

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

  // Process polyline
  void processPolyline(std::ifstream &);

  // Process spline
  void processSpline(std::ifstream &);

  // Available entities
  bool availableEntities() const;

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