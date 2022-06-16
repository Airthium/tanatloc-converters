#ifndef _POLYGON_
#define _POLYGON_

#include <vector>

using uint = unsigned int;

class Polygon {
private:
  std::vector<uint> m_indices;

public:
  // Constructor
  Polygon();

  // Add index
  void addIndex(const uint);

  // Get indices
  std::vector<uint> getIndices() const;
};

#endif //_POLYGON_