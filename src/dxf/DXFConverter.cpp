
#include <algorithm>

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Wire.hxx>

#include "DXFConverter.hpp"

#include "../logger/Logger.hpp"

/**
 * Constructor
 */
DXFConverter::DXFConverter() { this->clear(); }

/**
 * Set input
 * @param input
 */
void DXFConverter::setInput(const std::string &input) { this->m_input = input; }

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

/**
 * Convert
 * @return Status
 */
bool DXFConverter::convert() {
  std::ifstream file(this->m_input, std::ios::in);
  if (!file)
    return false;

  // https://images.autodesk.com/adsk/files/autocad_2012_pdf_dxf-reference_enu.pdf

  std::string line;
  while (std::getline(file, line)) {
    rtrim(line);
    if (line == "LINE") {
      this->processLine(file);
    } else if (line == "CIRCLE") {
      this->processCircle(file);
    } else if (line == "ARC") {
      this->processArc(file);
    } else if (line == "POLYLINE") {
      this->processPolyline(file);
    } else if (line == "SPLINE") {
      this->processSpline(file);
    } else if (line == "ENDSEC") {
      this->process();
    }
  }

  finish();

  return true;
}

/**
 * Get compound
 *
 * @return TopoDS_Compound
 */
TopoDS_Compound DXFConverter::getCompound() const { return this->m_compound; }

// PRIVATE

/**
 * Clear
 */
void DXFConverter::clear() {
  this->m_lines.clear();
  this->m_circles.clear();
  this->m_arcs.clear();
  this->m_polylines.clear();
  this->m_splines.clear();
  this->m_index.clear();

  this->needReverse = false;
}

/**
 * Process line
 * @param file File
 */
void DXFConverter::processLine(std::ifstream &file) {
  Logger::DEBUG("Process LINE");

  DXFLine newLine;
  newLine.process(file);

  if (!newLine.isEmpty() && !newLine.alreadyExists(this->m_lines)) {
    this->m_lines.push_back(newLine);

    Index index = {"line", this->m_lines.size() - 1};
    this->m_index.push_back(index);
  }
}

/**
 * Process circle
 * @param file File
 */
void DXFConverter::processCircle(std::ifstream &file) {
  Logger::DEBUG("Process CIRCLE");

  DXFCircle newCircle;
  newCircle.process(file);

  if (!newCircle.isEmpty() && !newCircle.alreadyExists(this->m_circles)) {
    this->m_circles.push_back(newCircle);

    Index index = {"circle", this->m_circles.size() - 1};
    this->m_index.push_back(index);
  }
}

/**
 * Process arc
 * @param file File
 */
void DXFConverter::processArc(std::ifstream &file) {
  Logger::DEBUG("Process ARC");

  DXFArc newArc;
  newArc.process(file);

  if (!newArc.isEmpty() && !newArc.alreadyExists(this->m_arcs)) {
    this->m_arcs.push_back(newArc);

    Index index = {"arc", this->m_arcs.size() - 1};
    this->m_index.push_back(index);
  }
}

/**
 * Process polyline
 * @param file File
 */
void DXFConverter::processPolyline(std::ifstream &file) {
  Logger::DEBUG("Process POLYLINE");

  DXFPolyline newPolyline;
  newPolyline.process(file);

  if (!newPolyline.isEmpty() && !newPolyline.alreadyExists(this->m_polylines)) {
    this->m_polylines.push_back(newPolyline);

    Index index = {"polyline", this->m_polylines.size() - 1};
    this->m_index.push_back(index);
  }
}

/**
 * Process spline
 * @param file File
 */
void DXFConverter::processSpline(std::ifstream &file) {
  Logger::DEBUG("Process SPLINE");

  DXFSpline newSpline;
  newSpline.process(file);

  if (!newSpline.isEmpty() && !newSpline.alreadyExists(this->m_splines)) {
    this->m_splines.push_back(newSpline);

    Index index = {"spline", this->m_splines.size() - 1};
    this->m_index.push_back(index);
  }
}

/**
 * Available entitites
 * @return true
 * @return false
 */
bool DXFConverter::availableEntities() const {
  return this->m_lines.size() || this->m_circles.size() ||
         this->m_arcs.size() || this->m_polylines.size() ||
         this->m_splines.size();
}

/**
 * Process
 */
void DXFConverter::process() {
  Logger::DEBUG("Process");

  if (!this->availableEntities()) {
    Logger::DEBUG("  Empty set");
    return;
  }

  std::vector<TopoDS_Wire> wires;
  auto wireBuilder = BRepBuilderAPI_MakeWire();

  Logger::DEBUG("  Loop index");
  std::for_each(this->m_index.begin(), this->m_index.end(),
                [this, &wireBuilder](const Index &index) {
                  std::string type = index.type;
                  if (type == "line") {
                    auto line = this->m_lines.at(index.index);
                    line.addToWireBuilder(wireBuilder);
                  } else if (type == "arc") {
                    auto arc = this->m_arcs.at(index.index);
                    if (arc.addToWireBuilder(wireBuilder)) {
                      this->needReverse = true;
                    }
                  } else if (type == "polyline") {
                    auto polyline = this->m_polylines.at(index.index);
                    polyline.addToWireBuilder(wireBuilder);
                  } else if (type == "spline") {
                    auto spline = this->m_splines.at(index.index);
                    spline.addToWireBuilder(wireBuilder);
                  }
                });

  if (wireBuilder.IsDone())
    wires.push_back(wireBuilder.Wire());

  Logger::DEBUG("  Build circles");
  std::for_each(
      this->m_circles.begin(), this->m_circles.end(),
      [&wires](const DXFCircle &circle) { wires.push_back(circle.toWire()); });

  // Build face
  if (const size_t wiresSize = wires.size(); !wiresSize) {
    return;
  } else if (wiresSize == 1) {
    auto wire = wires.at(0);

    // Reverse?
    if (this->needReverse)
      wire.Reverse();

    auto faceBuilder = BRepBuilderAPI_MakeFace(wire);
    TopoDS_Shape face = faceBuilder.Shape();
    this->m_faces.push_back(face);
  } else {
    auto wire = wires.at(0);

    // Reverse?
    if (this->needReverse)
      wire.Reverse();

    auto faceBuilder = BRepBuilderAPI_MakeFace(wire);

    // Faces loop
    for (size_t i = 1; i < wiresSize; ++i) {
      auto innerWire = wires.at(i);

      // Reverse?
      if ((!this->needReverse && ((i + 1) % 2 == 0)) ||
          (this->needReverse && (i % 2 == 0)))
        innerWire.Reverse();

      faceBuilder.Add(innerWire);
    }

    TopoDS_Shape faceWithHole = faceBuilder.Shape();
    this->m_faces.push_back(faceWithHole);
  }

  this->clear();
}

/**
 * Finish
 */
void DXFConverter::finish() {
  Logger::DEBUG("Finish");

  // Build compound
  BRep_Builder builder;
  builder.MakeCompound(this->m_compound);
  std::for_each(this->m_faces.begin(), this->m_faces.end(),
                [this, builder](const TopoDS_Shape &face) {
                  builder.Add(this->m_compound, face);
                });
}
