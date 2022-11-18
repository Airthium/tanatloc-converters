#ifndef _DXF_CIRCLE_
#define _DXF_CIRCLE_

class DXFCircle {
public:
  float x = 0.;
  float y = 0.;
  float z = 0.;

  float r = 0.;

  // Operator ==
  friend bool operator==(const DXFCircle &circle1, const DXFCircle &circle2) {
    return (circle1.x == circle2.x) && (circle1.y == circle2.y) &&
           (circle1.z == circle2.z) && (circle1.r == circle2.r);
  }
};

#endif