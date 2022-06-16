#include "Triangle.hpp"

/**
 * Constructor
 */
Triangle::Triangle() = default;

/**
 * Constructor
 * @param index1 Index 1
 * @param index2 Index 2
 * @param index3 Index 3
 * @param label Label
 */
Triangle::Triangle(const uint index1, const uint index2, const uint index3,
                   const uint label)
    : m_index1(index1), m_index2(index2), m_index3(index3), m_label(label) {}

/**
 * Set index 1
 * @param index1 Index 1
 */
void Triangle::setI1(const uint index1) { this->m_index1 = index1; }

/**
 * Set index 2
 * @param index2 Index 2
 */
void Triangle::setI2(const uint index2) { this->m_index2 = index2; }

/**
 * Set index 3
 * @param index3 Index 3
 */
void Triangle::setI3(const uint index3) { this->m_index3 = index3; }

/**
 * Set label
 * @param label Label
 */
void Triangle::setLabel(const uint label) { this->m_label = label; }

/**
 * Get index 1
 * @returns Index 1
 */
uint Triangle::I1() const { return this->m_index1; }

/**
 * Get index 2
 * @returns Index 2
 */
uint Triangle::I2() const { return this->m_index2; }

/**
 * Get index 3
 * @returns Index 3
 */
uint Triangle::I3() const { return this->m_index3; }

/**
 * Get indices
 * @returns Indices
 */
std::vector<uint> Triangle::getIndices() const {
  auto indices = std::vector<uint>();
  indices.push_back(this->m_index1);
  indices.push_back(this->m_index2);
  indices.push_back(this->m_index3);
  return indices;
}

/**
 * Get label
 * @returns Label
 */
uint Triangle::Label() const { return this->m_label; }
