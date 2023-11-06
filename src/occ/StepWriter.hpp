#ifndef _STEP_WRITER_
#define _STEP_WRITER_

#include "MainDocument.hpp"

class StepWriter {
private:
  std::string m_fileName = "";
  Handle(TDocStd_Document) m_mainDocument;

public:
  // Constructor
  StepWriter();
  // Constructor
  explicit StepWriter(const std::string &, const Handle(TDocStd_Document) &);

  // Write
  bool write() const;
};

#endif //_STEP_WRITER_