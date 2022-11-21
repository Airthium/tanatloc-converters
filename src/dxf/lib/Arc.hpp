#ifndef _DXF_ARC_
#define _DXF_ARC_

#include "./Circle.hpp"

class DXFArc {
public:
  DXFCircle circle;
  float startAngle;
  float endAngle;

  // Operator ==
  friend bool operator==(const DXFArc &arc1, const DXFArc &arc2) {
    return (arc1.circle == arc2.circle) &&
           (arc1.startAngle == arc2.startAngle) &&
           (arc1.endAngle == arc2.endAngle);
  }
};

#endif