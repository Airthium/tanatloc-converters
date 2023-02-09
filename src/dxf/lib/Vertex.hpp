#ifndef _DXF_VERTEX_
#define _DXF_VERTEX_

#include <fstream>
#include <vector>

class DXFVertex {
public:
  float x = 0.;
  float y = 0.;
  float z = 0.;

  // Process
  void process(std::ifstream &);

  // Already exists
  bool alreadyExists(const std::vector<DXFVertex> &) const;

  // Operator ==
  friend bool operator==(const DXFVertex &vertex1, const DXFVertex &vertex2);
};

#endif