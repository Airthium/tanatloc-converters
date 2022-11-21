
#include <algorithm>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
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

void DXFConverter::clear() {
  this->m_lines.clear();
  this->m_circles.clear();
  this->m_arcs.clear();
  this->m_polylines.clear();
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

bool DXFConverter::isConnected(const TopoDS_Edge lastEdge,
                               const gp_Pnt point1) const {
  if (lastEdge.IsNull())
    return true;

  TopExp_Explorer explorer(lastEdge, TopAbs_VERTEX);
  explorer.Next();
  gp_Pnt lastVertex = BRep_Tool::Pnt(TopoDS::Vertex(explorer.Current()));

  return (lastVertex.X() == point1.X()) && (lastVertex.Y() == point1.Y()) &&
         (lastVertex.Z() == point1.Z());
}

void DXFConverter::process() {
  Logger::DEBUG("Process");

  if (!this->availableEntities()) {
    Logger::DEBUG("  Empty set");
    return;
  }

  this->removeDoubles();

  std::vector<TopoDS_Wire> wires;
  auto linesArcsWireBuilder = BRepBuilderAPI_MakeWire();

  // Lines
  Logger::DEBUG("  Build lines");
  std::for_each(this->m_lines.begin(), this->m_lines.end(),
                [&linesArcsWireBuilder](const DXFLine &line) {
                  gp_Pnt point1(line.x1, line.y1, line.z1);
                  gp_Pnt point2(line.x2, line.y2, line.z2);

                  BRepBuilderAPI_MakeVertex vertexBuilder1(point1);
                  BRepBuilderAPI_MakeVertex vertexBuilder2(point2);
                  TopoDS_Vertex vertex1 = vertexBuilder1.Vertex();
                  TopoDS_Vertex vertex2 = vertexBuilder2.Vertex();

                  auto edgeBuilder = BRepBuilderAPI_MakeEdge(vertex1, vertex2);
                  TopoDS_Edge edge = edgeBuilder.Edge();

                  linesArcsWireBuilder.Add(edge);
                });

  // Arcs
  Logger::DEBUG("  Build arcs");
  std::for_each(this->m_arcs.begin(), this->m_arcs.end(),
                [&linesArcsWireBuilder](const DXFArc &arc) {
                  gp_Circ occCircle;
                  gp_Pnt center(arc.circle.x, arc.circle.y, arc.circle.z);
                  occCircle.SetLocation(center);
                  occCircle.SetRadius(arc.circle.r);

                  GC_MakeArcOfCircle occArc(
                      occCircle, 2. * M_PI * arc.startAngle / 360.,
                      2. * M_PI * arc.endAngle / 360., true);

                  Handle(Geom_TrimmedCurve) curve = occArc.Value();

                  auto edgeBuilder = BRepBuilderAPI_MakeEdge(curve);
                  TopoDS_Edge edge = edgeBuilder.Edge();

                  linesArcsWireBuilder.Add(edge);
                });

  if (linesArcsWireBuilder.IsDone())
    wires.push_back(linesArcsWireBuilder.Wire());

  Logger::DEBUG("  Build circles");
  std::for_each(this->m_circles.begin(), this->m_circles.end(),
                [&wires](const DXFCircle &circle) {
                  gp_Circ occCircle;
                  gp_Pnt center(circle.x, circle.y, circle.z);
                  occCircle.SetLocation(center);
                  occCircle.SetRadius(circle.r);

                  auto edgeBuilder = BRepBuilderAPI_MakeEdge(occCircle);
                  TopoDS_Edge occEdge = edgeBuilder.Edge();

                  auto wireBuilder = BRepBuilderAPI_MakeWire(occEdge);
                  TopoDS_Wire occWire = wireBuilder.Wire();

                  wires.push_back(occWire);
                });

  Logger::DEBUG("  Build polylines");
  std::for_each(this->m_polylines.begin(), this->m_polylines.end(),
                [&wires](const DXFPolyline &polyline) {
                  const size_t size = polyline.vertices.size();

                  auto wireBuilder = BRepBuilderAPI_MakeWire();
                  for (size_t i = 0; i < size; ++i) {
                    const DXFVertex v1 = polyline.vertices.at(i);
                    const DXFVertex v2 = polyline.vertices.at((i + 1) % size);

                    gp_Pnt point1(v1.x, v1.y, v1.z);
                    gp_Pnt point2(v2.x, v2.y, v2.z);

                    BRepBuilderAPI_MakeVertex vertexBuilder1(point1);
                    BRepBuilderAPI_MakeVertex vertexBuilder2(point2);

                    TopoDS_Vertex vertex1 = vertexBuilder1.Vertex();
                    TopoDS_Vertex vertex2 = vertexBuilder2.Vertex();

                    auto edgeBuilder =
                        BRepBuilderAPI_MakeEdge(vertex1, vertex2);
                    TopoDS_Edge edge = edgeBuilder.Edge();

                    wireBuilder.Add(edge);
                  }

                  wires.push_back(wireBuilder.Wire());
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
