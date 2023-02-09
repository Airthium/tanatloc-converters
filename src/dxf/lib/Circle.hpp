#ifndef _DXF_CIRCLE_
#define _DXF_CIRCLE_

#include <fstream>
#include <vector>

#include <TopoDS_Wire.hxx>

class DXFCircle {
public:
  float x = 0.;
  float y = 0.;
  float z = 0.;

  float r = 0.;

  // Process
  void process(std::ifstream &);

  // Is empty
  bool isEmpty() const;

  // Already exists
  bool alreadyExists(const std::vector<DXFCircle> &) const;

  // To wire
  TopoDS_Wire toWire() const;

  // Operator ==
  friend bool operator==(const DXFCircle &circle1, const DXFCircle &circle2);
};

#endif