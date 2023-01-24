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
  // Read
  IFSelect_ReturnStatus status;
  auto caf_reader = STEPCAFControl_Reader();
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

TopoDS_Compound StepReader::getCompound() const {
  return this->m_mainDocument.getCompound();
}

Quantity_Color StepReader::getShapeColor(const TopoDS_Shape &shape) const {
  return this->m_mainDocument.getShapeColor(shape);
}
