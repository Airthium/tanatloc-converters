#ifndef _DXF_LINE_
#define _DXF_LINE_

#include <fstream>
#include <vector>

#include <BRepBuilderAPI_MakeWire.hxx>

class DXFLine {
public:
  float x1 = 0.;
  float y1 = 0.;
  float z1 = 0.;

  float x2 = 0.;
  float y2 = 0.;
  float z2 = 0.;

  // Process
  void process(std::ifstream &);

  // Is empty
  bool isEmpty() const;

  // In X-Y
  bool inXY() const;

  // Already exists
  bool alreadyExists(const std::vector<DXFLine> &) const;

  // Add to wire builder
  void addToWireBuilder(BRepBuilderAPI_MakeWire &) const;

  // Operator ==
  friend bool operator==(const DXFLine &line1, const DXFLine &line2);
};

#endif