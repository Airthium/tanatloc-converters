#ifndef _VERTEX_
#define _VERTEX_

using uint = unsigned int;

/**
 * Vertex class
 */
class Vertex {
private:
  // Coordinate x
  double m_x = 0;
  // Coordinate y
  double m_y = 0;
  // Coordinate z
  double m_z = 0;

public:
  // Constructor
  Vertex();
  Vertex(const double, const double, const double);

  // Set x
  void setX(const double);

  // Set y
  void setY(const double);

  // Set z
  void setZ(const double);

  // X
  double X() const;

  // Y
  double Y() const;

  // Z
  double Z() const;
};

#endif //_VERTEX_
