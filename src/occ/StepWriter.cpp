#include "StepWriter.hpp"

#include <IFSelect_ReturnStatus.hxx>
#include <Message.hxx>
#include <STEPCAFControl_Writer.hxx>

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
StepWriter::StepWriter() = default;

/**
 * Constructor
 * @param fileName File name
 * @param document Document
 */
StepWriter::StepWriter(const std::string &fileName,
                       const Handle(TDocStd_Document) & document)
    : m_fileName(fileName), m_mainDocument(document) {}

/**
 * Write
 * @returns Status
 */
bool StepWriter::write() const {
  // Disable statistics output
  for (Message_SequenceOfPrinters::Iterator printer(
           Message::DefaultMessenger()->Printers());
       printer.More(); printer.Next()) {
    printer.Value()->SetTraceLevel(Message_Alarm);
  }

  // Prepare
  IFSelect_ReturnStatus status;
  auto caf_writer = STEPCAFControl_Writer();

  // Transfer
  if (!caf_writer.Transfer(this->m_mainDocument)) {
    Logger::ERROR("Unable to transfer root");
    return false;
  }

  // Write
  status = caf_writer.Write(this->m_fileName.c_str());
  if (status != IFSelect_RetDone) {
    Logger::ERROR("Unable to write " + this->m_fileName);
    return false;
  }

  return true;
}
