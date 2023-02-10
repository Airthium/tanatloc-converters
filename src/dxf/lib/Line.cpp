#include "Line.hpp"

#include <algorithm>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <gp_Pnt.hxx>

#include "./Defs.hpp"
#include "./Utils.hpp"

#include "../../logger/Logger.hpp"

/**
 * Process
 * @param file File
 */
void DXFLine::process(std::ifstream &file) {
  Logger::DEBUG("LINE::Process");

  std::string line;
  bool start = false;
  while (std::getline(file, line)) {
    rtrim(line);
    if (line == "AcDbEntity")
      start = true;

    if (start) {
      if (line == TAG10) {
        file >> this->x1;
      } else if (line == TAG11) {
        file >> this->x2;
      } else if (line == TAG20) {
        file >> this->y1;
      } else if (line == TAG21) {
        file >> this->y2;
      } else if (line == TAG30) {
        file >> this->z1;
      } else if (line == TAG31) {
        file >> this->z2;
      } else if (line == TAG0)
        break;
    }
  }

  // Check XY
  if (!this->inXY()) {
    this->x1 = 0.;
    this->y1 = 0.;
    this->x2 = 0.;
    this->y2 = 0.;
  }

  // Force z to 0
  this->z1 = 0.;
  this->z2 = 0.;
}

/**
 * Is empty?
 * @return true
 * @return false
 */
bool DXFLine::isEmpty() const {
  return (this->x1 == this->x2) && (this->y1 == this->y2) &&
         (this->z1 == this->z2);
}

/**
 * In X-Y?
 * @return true
 * @return false
 */
bool DXFLine::inXY() const { return this->z1 == this->z2; }

/**
 * Already exists?
 * @param lines Lines
 * @return true
 * @return false
 */
bool DXFLine::alreadyExists(const std::vector<DXFLine> &lines) const {
  bool res = false;
  std::for_each(lines.begin(), lines.end(), [this, &res](const DXFLine &line) {
    if (*this == line)
      res = true;
  });

  return res;
}

/**
 * Add to wire builder
 * @param wireBuilder
 */
void DXFLine::addToWireBuilder(BRepBuilderAPI_MakeWire &wireBuilder) const {
  Logger::DEBUG("LINE::addToWireBuilder");
  gp_Pnt point1(this->x1, this->y1, this->z1);
  gp_Pnt point2(this->x2, this->y2, this->z2);

  BRepBuilderAPI_MakeVertex vertexBuilder1(point1);
  BRepBuilderAPI_MakeVertex vertexBuilder2(point2);

  TopoDS_Vertex vertex1 = vertexBuilder1.Vertex();
  TopoDS_Vertex vertex2 = vertexBuilder2.Vertex();

  auto edgeBuilder = BRepBuilderAPI_MakeEdge(vertex1, vertex2);
  TopoDS_Edge edge = edgeBuilder.Edge();

  wireBuilder.Add(edge);

  if (const auto error = wireBuilder.Error();
      error != BRepBuilderAPI_WireDone) {
    Logger::WARNING("LINE::addToWireBuilder error");
    Logger::WARNING("" + std::to_string(error));
  }
}

/**
 * Operator ==
 * @param line1  Line 1
 * @param line2 Line 2
 * @return true
 * @return false
 */
bool operator==(const DXFLine &line1, const DXFLine &line2) {
  return (line1.x1 == line2.x1) && (line1.y1 == line2.y1) &&
         (line1.z1 == line2.z1) && (line1.x2 == line2.x2) &&
         (line1.y2 == line2.y2) && (line1.z2 == line2.z2);
}
