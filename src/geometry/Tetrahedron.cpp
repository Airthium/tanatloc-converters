#include "Tetrahedron.hpp"

/**
 * Constructor
 */
Tetrahedron::Tetrahedron() = default;

/**
 * Constructor
 * @param index1 Index 1
 * @param index2 Index 2
 * @param index3 Index 3
 * @param index4 Index 4
 * @param label Label
 */
Tetrahedron::Tetrahedron(const uint index1, const uint index2,
                         const uint index3, const uint index4, const uint label)
    : m_index1(index1), m_index2(index2), m_index3(index3), m_index4(index4),
      m_label(label) {}

/**
 * Set index 1
 * @param index1 Index 1
 */
void Tetrahedron::setI1(const uint index1) { this->m_index1 = index1; }

/**
 * Set index 2
 * @param index2 Index 2
 */
void Tetrahedron::setI2(const uint index2) { this->m_index2 = index2; }

/**
 * Set index 3
 * @param index3 Index 3
 */
void Tetrahedron::setI3(const uint index3) { this->m_index3 = index3; }

/**
 * Set index 4
 * @param index4 Index 4
 */
void Tetrahedron::setI4(const uint index4) { this->m_index4 = index4; }

/**
 * Set label
 * @param label Label
 */
void Tetrahedron::setLabel(const uint label) { this->m_label = label; }

/**
 * Get index 1
 * @returns Index 1
 */
uint Tetrahedron::I1() const { return this->m_index1; }

/**
 * Get index 2
 * @returns Index 2
 */
uint Tetrahedron::I2() const { return this->m_index2; }

/**
 * Get index 3
 * @returns Index 3
 */
uint Tetrahedron::I3() const { return this->m_index3; }

/**
 * Get index 4
 * @returns Index 4
 */
uint Tetrahedron::I4() const { return this->m_index4; }

/**
 * Get indices
 * @returns Indices
 */
std::vector<uint> Tetrahedron::getIndices() const {
  auto indices = std::vector<uint>();
  indices.push_back(this->m_index1);
  indices.push_back(this->m_index2);
  indices.push_back(this->m_index3);
  indices.push_back(this->m_index4);
  return indices;
}

/**
 * Get label
 * @returns Label
 */
uint Tetrahedron::Label() const { return this->m_label; }
