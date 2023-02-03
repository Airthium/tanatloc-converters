
#include <algorithm>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRep_Builder.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Circ.hxx>

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
  this->m_index.clear();
}

void DXFConverter::processLine(std::ifstream &file) {
  Logger::DEBUG("Process LINE");

  DXFLine newLine;

  std::string line;
  bool start = false;
  while (std::getline(file, line)) {
    rtrim(line);
    if (line == "AcDbEntity")
      start = true;

    if (start) {
      if (line == TAG10) {
        file >> newLine.x1;
      } else if (line == TAG11) {
        file >> newLine.x2;
      } else if (line == TAG20) {
        file >> newLine.y1;
      } else if (line == TAG21) {
        file >> newLine.y2;
      } else if (line == TAG30) {
        file >> newLine.z1;
      } else if (line == TAG31) {
        file >> newLine.z2;
      } else if (line == TAG0)
        break;
    }
  }

  if (newLine.inXY()) {
    // Force z to 0.
    newLine.z1 = 0.;
    newLine.z2 = 0.;
    this->m_lines.push_back(newLine);

    Index index = {"line", this->m_lines.size() - 1};
    this->m_index.push_back(index);
  }
}

void DXFConverter::processCircle(std::ifstream &file) {
  Logger::DEBUG("Process CIRCLE");

  DXFCircle newCircle;

  std::string line;
  bool start = false;
  while (std::getline(file, line)) {
    rtrim(line);
    if (line == "AcDbEntity")
      start = true;

    if (start) {
      if (line == TAG10) {
        file >> newCircle.x;
      } else if (line == TAG20) {
        file >> newCircle.y;
      } else if (line == TAG30) {
        file >> newCircle.z;
      } else if (line == TAG40) {
        file >> newCircle.r;
      } else if (line == TAG0)
        break;
    }
  }

  // Force z to 0.
  newCircle.z = 0.;

  this->m_circles.push_back(newCircle);

  Index index = {"circle", this->m_circles.size() - 1};
  this->m_index.push_back(index);
}

void DXFConverter::processArc(std::ifstream &file) {
  Logger::DEBUG("Process ARC");

  DXFArc newArc;

  std::string line;
  bool start = false;
  while (std::getline(file, line)) {
    rtrim(line);
    if (line == "AcDbEntity")
      start = true;

    if (start) {
      if (line == TAG10) {
        file >> newArc.circle.x;
      } else if (line == TAG20) {
        file >> newArc.circle.y;
      } else if (line == TAG30) {
        file >> newArc.circle.z;
      } else if (line == TAG40) {
        file >> newArc.circle.r;
      } else if (line == TAG50) {
        file >> newArc.startAngle;
      } else if (line == TAG51) {
        file >> newArc.endAngle;
      } else if (line == TAG0)
        break;
    }
  }

  // Force z to 0.
  newArc.circle.z = 0.;

  this->m_arcs.push_back(newArc);

  Index index = {"arc", this->m_arcs.size() - 1};
  this->m_index.push_back(index);
}

DXFVertex DXFConverter::processVertex(std::ifstream &file) const {
  Logger::DEBUG("Process VERTEX");

  DXFVertex newVertex;

  std::string line;
  while (std::getline(file, line)) {
    rtrim(line);
    if (line == TAG10) {
      file >> newVertex.x;
    } else if (line == TAG20) {
      file >> newVertex.y;
    } else if (line == TAG30) {
      file >> newVertex.z;
    } else if (line == TAG0)
      break;
  }

  // Force z to 0.
  newVertex.z = 0.;

  return newVertex;
}

void DXFConverter::processPolyline(std::ifstream &file) {
  Logger::DEBUG("Process POLYLINE");

  DXFPolyline newPolyline;

  std::string line;
  while (std::getline(file, line)) {
    rtrim(line);
    if (line == VERTEX) {
      DXFVertex vertex = processVertex(file);
      newPolyline.vertices.push_back(vertex);
    } else if (line == SEQEND)
      break;
  }

  this->m_polylines.push_back(newPolyline);

  Index index = {"polyline", this->m_polylines.size() - 1};
  this->m_index.push_back(index);
}

bool DXFConverter::availableEntities() const {
  return this->m_lines.size() || this->m_circles.size() ||
         this->m_arcs.size() || this->m_polylines.size();
}

void DXFConverter::removeDoubles() {
  Logger::DEBUG("Remove doubles");

  // Lines
  this->m_lines.erase(std::unique(this->m_lines.begin(), this->m_lines.end()),
                      this->m_lines.end());

  // Circles
  this->m_circles.erase(
      std::unique(this->m_circles.begin(), this->m_circles.end()),
      this->m_circles.end());

  // Arcs
  this->m_arcs.erase(std::unique(this->m_arcs.begin(), this->m_arcs.end()),
                     this->m_arcs.end());

  // Polylines
  this->m_polylines.erase(
      std::unique(this->m_polylines.begin(), this->m_polylines.end()),
      this->m_polylines.end());
}

void DXFConverter::addLineToWireBuilder(
    const DXFLine &line, BRepBuilderAPI_MakeWire &wireBuilder) const {
  gp_Pnt point1(line.x1, line.y1, line.z1);
  gp_Pnt point2(line.x2, line.y2, line.z2);

  BRepBuilderAPI_MakeVertex vertexBuilder1(point1);
  BRepBuilderAPI_MakeVertex vertexBuilder2(point2);
  TopoDS_Vertex vertex1 = vertexBuilder1.Vertex();
  TopoDS_Vertex vertex2 = vertexBuilder2.Vertex();

  auto edgeBuilder = BRepBuilderAPI_MakeEdge(vertex1, vertex2);
  TopoDS_Edge edge = edgeBuilder.Edge();

  wireBuilder.Add(edge);
}

void DXFConverter::addArcToWireBuilder(
    const DXFArc &arc, BRepBuilderAPI_MakeWire &wireBuilder) const {
  gp_Circ occCircle;
  gp_Pnt center(arc.circle.x, arc.circle.y, arc.circle.z);
  occCircle.SetLocation(center);
  occCircle.SetRadius(arc.circle.r);

  GC_MakeArcOfCircle occArc(occCircle, 2. * M_PI * arc.startAngle / 360.,
                            2. * M_PI * arc.endAngle / 360., true);

  Handle(Geom_TrimmedCurve) curve = occArc.Value();

  auto edgeBuilder = BRepBuilderAPI_MakeEdge(curve);
  TopoDS_Edge edge = edgeBuilder.Edge();

  wireBuilder.Add(edge);
}

void DXFConverter::addPolylineToWireBuilder(
    const DXFPolyline &polyline, BRepBuilderAPI_MakeWire &wireBuilder) const {
  const size_t size = polyline.vertices.size();

  for (size_t i = 0; i < size; ++i) {
    const DXFVertex v1 = polyline.vertices.at(i);
    const DXFVertex v2 = polyline.vertices.at((i + 1) % size);

    gp_Pnt point1(v1.x, v1.y, v1.z);
    gp_Pnt point2(v2.x, v2.y, v2.z);

    BRepBuilderAPI_MakeVertex vertexBuilder1(point1);
    BRepBuilderAPI_MakeVertex vertexBuilder2(point2);

    TopoDS_Vertex vertex1 = vertexBuilder1.Vertex();
    TopoDS_Vertex vertex2 = vertexBuilder2.Vertex();

    auto edgeBuilder = BRepBuilderAPI_MakeEdge(vertex1, vertex2);
    TopoDS_Edge edge = edgeBuilder.Edge();

    wireBuilder.Add(edge);
  }
}

void DXFConverter::process() {
  Logger::DEBUG("Process");

  if (!this->availableEntities()) {
    Logger::DEBUG("  Empty set");
    return;
  }

  this->removeDoubles();

  std::vector<TopoDS_Wire> wires;
  auto wireBuilder = BRepBuilderAPI_MakeWire();

  Logger::DEBUG("  Loop index");
  std::for_each(this->m_index.begin(), this->m_index.end(),
                [this, &wireBuilder](const Index &index) {
                  std::string type = index.type;
                  if (type == "line") {
                    auto line = this->m_lines.at(index.index);
                    this->addLineToWireBuilder(line, wireBuilder);
                  } else if (type == "arc") {
                    auto arc = this->m_arcs.at(index.index);
                    this->addArcToWireBuilder(arc, wireBuilder);
                  } else if (type == "polyline") {
                    auto polyline = this->m_polylines.at(index.index);
                    this->addPolylineToWireBuilder(polyline, wireBuilder);
                  }
                });

  if (wireBuilder.IsDone())
    wires.push_back(wireBuilder.Wire());

  Logger::DEBUG("  Build circles");
  std::for_each(this->m_circles.begin(), this->m_circles.end(),
                [&wires](const DXFCircle &circle) {
                  gp_Circ occCircle;
                  gp_Pnt center(circle.x, circle.y, circle.z);
                  occCircle.SetLocation(center);
                  occCircle.SetRadius(circle.r);

                  auto edgeBuilder = BRepBuilderAPI_MakeEdge(occCircle);
                  TopoDS_Edge occEdge = edgeBuilder.Edge();

                  auto circleWireBuilder = BRepBuilderAPI_MakeWire(occEdge);
                  TopoDS_Wire occWire = circleWireBuilder.Wire();

                  wires.push_back(occWire);
                });

  // Build face
  if (const size_t wiresSize = wires.size(); !wiresSize) {
    return;
  } else if (wiresSize == 1) {
    auto faceBuilder = BRepBuilderAPI_MakeFace(wires.at(0));

    TopoDS_Shape face = faceBuilder.Shape();
    this->m_faces.push_back(face);
  } else {
    auto faceBuilder = BRepBuilderAPI_MakeFace(wires.at(0));

    for (size_t i = 1; i < wiresSize; ++i) {
      TopoDS_Wire wire = wires.at(i);
      wire.Reverse();
      faceBuilder.Add(wire);
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
