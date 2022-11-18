#ifndef _DXF_LINE_
#define _DXF_LINE_

class DXFLine {
public:
  float x1 = 0.;
  float y1 = 0.;
  float z1 = 0.;

  float x2 = 0.;
  float y2 = 0.;
  float z2 = 0.;

  // In X-Y
  bool inXY() const { return this->z1 == this->z2; };

  // Operator ==
  friend bool operator==(const DXFLine &line1, const DXFLine &line2) {
    return (line1.x1 == line2.x1) && (line1.y1 == line2.y1) &&
           (line1.z1 == line2.z1) && (line1.x2 == line2.x2) &&
           (line1.y2 == line2.y2) && (line1.z2 == line2.z2);
  }
};

#endif