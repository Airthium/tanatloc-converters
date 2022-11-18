#ifndef _DXF_POLYLINE_
#define _DXF_POLYLINE_

#include <algorithm>
#include <vector>

#include "./Vertex.hpp"

class DXFPolyline {
public:
  std::vector<DXFVertex> vertices;

  // In X-Y
  bool inXY() const {
    bool res = true;

    if (!vertices.size())
      return res;

    float lastZ = vertices.at(0).z;
    std::for_each(vertices.begin(), vertices.end(),
                  [&lastZ, &res](const DXFVertex vertex) {
                    if (vertex.z != lastZ)
                      res = false;
                  });

    return res;
  }

  // Operator ==
  friend bool operator==(const DXFPolyline &polyline1,
                         const DXFPolyline &polyline2) {
    bool res = true;

    if (polyline1.vertices.size() != polyline2.vertices.size())
      return false;

    std::for_each(
        polyline1.vertices.begin(), polyline1.vertices.end(),
        [index = 0, &polyline2, &res](const DXFVertex vertex1) mutable {
          if (!(vertex1 == polyline2.vertices.at(index)))
            res = false;

          index++;
        });

    return res;
  }
};

#endif