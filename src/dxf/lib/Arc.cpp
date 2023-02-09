#include "Arc.hpp"

#include <algorithm>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <gp_Circ.hxx>

#include "./Defs.hpp"
#include "./Utils.hpp"

#include "../../logger/Logger.hpp"

/**
 * Process
 * @param file File
 */
void DXFArc::process(std::ifstream &file) {
  Logger::DEBUG("ARC::Process");

  std::string line;
  bool start = false;
  while (std::getline(file, line)) {
    rtrim(line);
    if (line == "AcDbEntity")
      start = true;

    if (start) {
      if (line == TAG10) {
        file >> this->circle.x;
      } else if (line == TAG20) {
        file >> this->circle.y;
      } else if (line == TAG30) {
        file >> this->circle.z;
      } else if (line == TAG40) {
        file >> this->circle.r;
      } else if (line == TAG50) {
        file >> this->startAngle;
      } else if (line == TAG51) {
        file >> this->endAngle;
      } else if (line == TAG0)
        break;
    }
  }

  // Force z to 0.
  this->circle.z = 0.;
}

/**
 * Is empty?
 * @return true
 * @return false
 */
bool DXFArc::isEmpty() const {
  return this->circle.isEmpty() || this->startAngle == this->endAngle;
}

/**
 * Already exists?
 * @param arcs Arcs
 * @return true
 * @return false
 */
bool DXFArc::alreadyExists(const std::vector<DXFArc> &arcs) const {
  bool res = false;
  std::for_each(arcs.begin(), arcs.end(), [this, &res](const DXFArc &arc) {
    if (*this == arc)
      res = true;
  });

  return res;
}

/**
 * Add to wire builder
 * @param wireBuilder Wire builder
 */
void DXFArc::addToWireBuilder(BRepBuilderAPI_MakeWire &wireBuilder) const {
  Logger::DEBUG("ARC::addToWireBuilder");
  gp_Circ occCircle;
  gp_Pnt center(this->circle.x, this->circle.y, this->circle.z);
  occCircle.SetLocation(center);
  occCircle.SetRadius(this->circle.r);

  GC_MakeArcOfCircle occArc(occCircle, 2. * M_PI * this->startAngle / 360.,
                            2. * M_PI * this->endAngle / 360., true);

  Handle(Geom_TrimmedCurve) curve = occArc.Value();

  auto edgeBuilder = BRepBuilderAPI_MakeEdge(curve);
  TopoDS_Edge edge = edgeBuilder.Edge();

  wireBuilder.Add(edge);
  const auto error = wireBuilder.Error();
  if (error != BRepBuilderAPI_WireDone) {
    Logger::WARNING("ARC::addToWireBuilder error");
    Logger::WARNING("" + std::to_string(error));
  }
}

/**
 * Operator ==
 * @param arc1 Arc 1
 * @param arc2 Arc 2
 * @return true
 * @return false
 */
bool operator==(const DXFArc &arc1, const DXFArc &arc2) {
  return (arc1.circle == arc2.circle) && (arc1.startAngle == arc2.startAngle) &&
         (arc1.endAngle == arc2.endAngle);
}
