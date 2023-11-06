#include "StepReader.hpp"

#include <IFSelect_ReturnStatus.hxx>
#include <STEPCAFControl_Reader.hxx>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
StepReader::StepReader() = default;

/**
 * Constructor
 * @param fileName File name
 */
StepReader::StepReader(const std::string &fileName) : m_fileName(fileName) {}

/**
 * Read
 * @returns Status
 */
bool StepReader::read() const {
  // Prepare
  IFSelect_ReturnStatus status;
  auto caf_reader = STEPCAFControl_Reader();

  // Read
  status = caf_reader.ReadFile(this->m_fileName.c_str());
  if (status != IFSelect_RetDone) {
    Logger::ERROR("Unable to read " + this->m_fileName);
    return false;
  }

  // Transfer
  Handle(TDocStd_Document) document = this->m_mainDocument.getDocument();
  if (!caf_reader.Transfer(document)) {
    Logger::ERROR("Unable to transfert root");
    return false;
  }

  return true;
}

/**
 * Get document
 * @returns TDocStd_Document Document
 */
Handle(TDocStd_Document) StepReader::getDocument() const {
  return this->m_mainDocument.getDocument();
}

/**
 * Get compound
 * @return TopoDS_Compound Compound
 */
TopoDS_Compound StepReader::getCompound() const {
  return this->m_mainDocument.getCompound();
}

/**
 * Get shape color
 * @param shape Shape
 * @return Quantity_Color Color
 */
Quantity_Color StepReader::getShapeColor(const TopoDS_Shape &shape) const {
  return this->m_mainDocument.getShapeColor(shape);
}
