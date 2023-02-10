#ifndef _DXF_ARC_
#define _DXF_ARC_

#include <fstream>
#include <vector>

#include <BRepBuilderAPI_MakeWire.hxx>

#include "./Circle.hpp"

class DXFArc {
public:
  DXFCircle circle;
  float startAngle = 0.;
  float endAngle = 0.;

  // Process
  void process(std::ifstream &);

  // Is empty
  bool isEmpty() const;

  // Already exists
  bool alreadyExists(const std::vector<DXFArc> &) const;

  // Add to wire builder
  bool addToWireBuilder(BRepBuilderAPI_MakeWire &) const;

  // Operator ==
  friend bool operator==(const DXFArc &arc1, const DXFArc &arc2);
};

#endif