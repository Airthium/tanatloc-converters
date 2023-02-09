#ifndef _DXF_SPLINE_
#define _DXF_SPLINE_

#include <algorithm>
#include <vector>

#include "./Vertex.hpp"

#include <BRepBuilderAPI_MakeWire.hxx>

class DXFSpline {
public:
  int degree = 0;
  int numberOfKnots = 0;
  int numberOfControlPoints = 0;
  int numberOfFitPoints = 0;

  DXFVertex normal;

  std::vector<float> knotsValues;
  std::vector<DXFVertex> controlPoints;
  std::vector<DXFVertex> fitPoints;

  // Process
  void process(std::ifstream &);

  // Is empty
  bool isEmpty() const;

  // In X-Y
  bool inXY() const;

  // Already exists
  bool alreadyExists(const std::vector<DXFSpline> &) const;

  // Add to wire builder
  void addToWireBuilder(BRepBuilderAPI_MakeWire &) const;

  // Operator ==
  friend bool operator==(const DXFSpline &spline1, const DXFSpline &spline2);
};

#endif