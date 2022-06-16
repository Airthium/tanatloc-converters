#ifndef _STEP_READER_
#define _STEP_READER_

#include "MainDocument.hpp"

class StepReader {
private:
  std::string m_fileName = "";
  MainDocument m_mainDocument;

public:
  // Constructor
  StepReader();
  // Constructor
  explicit StepReader(const std::string &);

  // Read
  bool read();

  TopoDS_Compound getCompound() const;
  Quantity_Color getShapeColor(const TopoDS_Shape &shape) const;
};

#endif //_STEP_READER_
