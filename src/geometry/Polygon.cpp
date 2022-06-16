#include "Polygon.hpp"

/**
 * Constructor
 */
Polygon::Polygon() = default;

/**
 * Add index
 * @param index Index
 */
void Polygon::addIndex(const uint index) { this->m_indices.push_back(index); }

/**
 * Get indices
 * @return Indices
 */
std::vector<uint> Polygon::getIndices() const { return this->m_indices; }
