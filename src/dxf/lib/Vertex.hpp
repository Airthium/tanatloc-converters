#ifndef _DXF_VERTEX_
#define _DXF_VERTEX_

class DXFVertex {
public:
  float x;
  float y;
  float z;

  // Operator ==
  friend bool operator==(const DXFVertex &vertex1, const DXFVertex &vertex2) {
    return (vertex1.x == vertex2.x) && (vertex1.y == vertex2.y) &&
           (vertex1.z == vertex2.z);
  }
};

#endif