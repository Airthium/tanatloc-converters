#ifndef _TRIANGLE_
#define _TRIANGLE_

#include <vector>

using uint = unsigned int;

/**
 * Triangle class
 */
class Triangle {
private:
  // Index 1
  uint m_index1 = 0;
  // Index 2
  uint m_index2 = 0;
  // Index 3
  uint m_index3 = 0;
  // Label
  uint m_label = 0;

public:
  // Constructor
  Triangle();
  // Constructor
  Triangle(const uint, const uint, const uint, const uint);

  // Set indices
  void setI1(const uint);
  void setI2(const uint);
  void setI3(const uint);
  // Set label
  void setLabel(const uint);

  // Get index
  uint I1() const;
  uint I2() const;
  uint I3() const;

  // Get indices
  std::vector<uint> getIndices() const;

  // Get label
  uint Label() const;
};

#endif //_TRIANGLE_
