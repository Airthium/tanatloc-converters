#ifndef _DXF_POLYLINE_
#define _DXF_POLYLINE_

#include <algorithm>
#include <fstream>
#include <vector>

#include <BRepBuilderAPI_MakeWire.hxx>

#include "./Vertex.hpp"

class DXFPolyline {
public:
  std::vector<DXFVertex> vertices;

  // Process
  void process(std::ifstream &);

  // Is empty
  bool isEmpty() const;

  // Already exists
  bool alreadyExists(const std::vector<DXFPolyline> &) const;

  // Add to wire builder
  void addToWireBuilder(BRepBuilderAPI_MakeWire &) const;

  // Operator ==
  friend bool operator==(const DXFPolyline &polyline1,
                         const DXFPolyline &polyline2);
};

#endif