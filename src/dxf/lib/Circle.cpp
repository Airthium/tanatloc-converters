#include "Circle.hpp"

#include <algorithm>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopoDS_Edge.hxx>
#include <gp_Circ.hxx>

#include "./Defs.hpp"
#include "./Utils.hpp"

#include "../../logger/Logger.hpp"

/**
 * Process
 * @param file File
 */
void DXFCircle::process(std::ifstream &file) {
  Logger::DEBUG("CIRCLE::Process");

  std::string line;
  bool start = false;
  while (std::getline(file, line)) {
    rtrim(line);
    if (line == "AcDbEntity")
      start = true;

    if (start) {
      if (line == TAG10) {
        file >> this->x;
      } else if (line == TAG20) {
        file >> this->y;
      } else if (line == TAG30) {
        file >> this->z;
      } else if (line == TAG40) {
        file >> this->r;
      } else if (line == TAG0)
        break;
    }
  }

  // Force z to 0.
  this->z = 0.;
}

/**
 * Is empty ?
 * @return true
 * @return false
 */
bool DXFCircle::isEmpty() const { return this->r == 0; }

/**
 * Already exists?
 * @return true
 * @return false
 */
bool DXFCircle::alreadyExists(const std::vector<DXFCircle> &circles) const {
  bool res = false;
  std::for_each(circles.begin(), circles.end(),
                [this, &res](const DXFCircle &circle) {
                  if (*this == circle)
                    res = true;
                });

  return res;
}

/**
 * To wire
 * @return TopoDS_Wire
 */
TopoDS_Wire DXFCircle::toWire() const {
  Logger::DEBUG("CIRCLE::toWire");
  gp_Circ occCircle;
  gp_Pnt center(this->x, this->y, this->z);
  occCircle.SetLocation(center);
  occCircle.SetRadius(this->r);

  auto edgeBuilder = BRepBuilderAPI_MakeEdge(occCircle);
  TopoDS_Edge occEdge = edgeBuilder.Edge();

  auto circleWireBuilder = BRepBuilderAPI_MakeWire(occEdge);
  TopoDS_Wire occWire = circleWireBuilder.Wire();

  return occWire;
}

/**
 * Operator ==
 * @param circle1 Circle 1
 * @param circle2 Circle 2
 * @return true
 * @return false
 */
bool operator==(const DXFCircle &circle1, const DXFCircle &circle2) {
  return (circle1.x == circle2.x) && (circle1.y == circle2.y) &&
         (circle1.z == circle2.z) && (circle1.r == circle2.r);
}