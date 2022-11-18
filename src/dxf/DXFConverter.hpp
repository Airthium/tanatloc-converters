#ifndef _DXF_CONVERTER_
#define _DXF_CONVERTER_

#include <vector>

#include <TopoDS_Compound.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>

#include "./lib/Circle.hpp"
#include "./lib/Line.hpp"

const std::string TAG0 = "  0";
const std::string TAG10 = " 10";
const std::string TAG11 = " 11";
const std::string TAG20 = " 20";
const std::string TAG21 = " 21";
const std::string TAG30 = " 30";
const std::string TAG31 = " 31";
const std::string TAG40 = " 40";

class DXFConverter {
private:
  std::string m_input;

  std::vector<Line> m_lines;
  std::vector<Circle> m_circles;

  std::vector<TopoDS_Wire> m_wires;
  std::vector<TopoDS_Shape> m_faces;

  TopoDS_Compound m_compound;

  // Clear
  void clear();

  // Process line
  void processLine(std::ifstream &);

  // Process circle
  void processCircle(std::ifstream &);

  // Process
  void process();

public:
  // Constructor
  DXFConverter();

  // Set input
  void setInput(const std::string &);

  // Convert
  bool convert();

  // Get shape
  TopoDS_Compound getCompound() const;
};

#endif