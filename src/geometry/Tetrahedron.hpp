#ifndef _TETRAHEDRON_
#define _TETRAHEDRON_

#include <vector>

using uint = unsigned int;

/**
 * Tetrahedron class
 */
class Tetrahedron {
private:
  // Index 1
  uint m_index1 = 0;
  // Index 2
  uint m_index2 = 0;
  // Index 3
  uint m_index3 = 0;
  // Index 4
  uint m_index4 = 0;
  // Label
  uint m_label = 0;

public:
  // Constructor
  Tetrahedron();
  // Constructor
  Tetrahedron(const uint, const uint, const uint, const uint, const uint);

  // Set index
  void setI1(const uint);
  void setI2(const uint);
  void setI3(const uint);
  void setI4(const uint);

  // Set label
  void setLabel(const uint);

  // Get index
  uint I1() const;
  uint I2() const;
  uint I3() const;
  uint I4() const;

  // Get indices
  std::vector<uint> getIndices() const;

  // Get label
  uint Label() const;
};

#endif //_TETRAHEDRON_
