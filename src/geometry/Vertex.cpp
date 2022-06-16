#include "Vertex.hpp"

/**
 * Constructor
 */
Vertex::Vertex() = default;

/**
 * Constructor
 * @param {double} X
 * @param {double} Y
 * @param {double} Z
 */
Vertex::Vertex(const double x, const double y, const double z)
    : m_x(x), m_y(y), m_z(z) {}

/**
 * Set x
 * @param x X
 */
void Vertex::setX(const double x) { this->m_x = x; }

/**
 * Set y
 * @param y Y
 */
void Vertex::setY(const double y) { this->m_y = y; }

/**
 * Set z
 * @param z Z
 */
void Vertex::setZ(const double z) { this->m_z = z; }

/**
 * Get x
 * @return double X
 */
double Vertex::X() const { return this->m_x; }

/**
 * Get y
 * @return double Y
 */
double Vertex::Y() const { return this->m_y; }

/**
 * Get z
 *
 * @return double Z
 */
double Vertex::Z() const { return this->m_z; }
