
#include <algorithm>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
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

/**
 * Convert
 * @return Status
 */
bool DXFConverter::convert() {
  std::ifstream file(this->m_input, std::ios::in);
  if (!file)
    return false;

  std::string line;
  while (std::getline(file, line)) {
    if (line == "LINE") {
      this->processLine(file);
    } else if (line == "CIRCLE") {
      this->processCircle(file);
    } else if (line == "ARC") {
      std::cerr << "Not implemented yet" << std::endl;
    } else if (line == "POLYLINE") {
      std::cerr << "Not implemented yet" << std::endl;
    }
  }

  process();

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

  this->m_wires.clear();
  this->m_faces.clear();
}

void DXFConverter::processLine(std::ifstream &file) {
  Logger::DEBUG("Process LINE");

  Line newLine;

  std::string line;
  bool start = false;
  while (std::getline(file, line)) {
    if (line == "AcDbLine")
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

  if (newLine.inXY())
    this->m_lines.push_back(newLine);
}

void DXFConverter::processCircle(std::ifstream &file) {
  Logger::DEBUG("Process CIRCLE");

  Circle newCircle;

  std::string line;
  bool start = false;
  while (std::getline(file, line)) {
    if (line == "AcDbCircle")
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

  this->m_circles.push_back(newCircle);
}

void DXFConverter::process() {
  Logger::DEBUG("Process");

  // Remove double lines
  this->m_lines.erase(std::unique(this->m_lines.begin(), this->m_lines.end()),
                      this->m_lines.end());

  // Remove double circles
  this->m_circles.erase(
      std::unique(this->m_circles.begin(), this->m_circles.end()),
      this->m_circles.end());

  // Build wires

  auto wireBuilder = BRepBuilderAPI_MakeWire();
  std::for_each(
      this->m_lines.begin(), this->m_lines.end(),
      [&wireBuilder](const Line &line) {
        gp_Pnt point1(line.x1, line.y1, line.z1);
        gp_Pnt point2(line.x2, line.y2, line.z2);

        BRepBuilderAPI_MakeVertex vertexBuilder1(point1);
        BRepBuilderAPI_MakeVertex vertexBuilder2(point2);
        TopoDS_Vertex vertex1 = vertexBuilder1.Vertex();
        TopoDS_Vertex vertex2 = vertexBuilder2.Vertex();

        TopoDS_Edge lastEdge = wireBuilder.Edge();
        if (lastEdge.IsNull()) {
          auto edgeBuilder = BRepBuilderAPI_MakeEdge(vertex1, vertex2);
          TopoDS_Edge edge = edgeBuilder.Edge();

          wireBuilder.Add(edge);
        } else {
          TopExp_Explorer explorer(lastEdge, TopAbs_VERTEX);
          explorer.Next();
          gp_Pnt lastVertex =
              BRep_Tool::Pnt(TopoDS::Vertex(explorer.Current()));

          if (lastVertex.X() == point1.X() && lastVertex.Y() == point1.Y() &&
              lastVertex.Z() == point1.Z()) {
            auto edgeBuilder = BRepBuilderAPI_MakeEdge(vertex1, vertex2);
            TopoDS_Edge edge = edgeBuilder.Edge();

            wireBuilder.Add(edge);
          }
        }
      });

  this->m_wires.push_back(wireBuilder.Wire());

  std::for_each(this->m_circles.begin(), this->m_circles.end(),
                [this](const Circle &circle) {
                  gp_Circ occCircle;
                  gp_Pnt center(circle.x, circle.y, circle.z);
                  occCircle.SetLocation(center);
                  occCircle.SetRadius(circle.r);

                  auto edgeBuilder = BRepBuilderAPI_MakeEdge(occCircle);
                  TopoDS_Edge occEdge = edgeBuilder.Edge();

                  auto wireBuilder = BRepBuilderAPI_MakeWire(occEdge);
                  TopoDS_Wire occWire = wireBuilder.Wire();

                  this->m_wires.push_back(occWire);
                });

  // Build faces
  std::for_each(this->m_wires.begin(), this->m_wires.end(),
                [this](const TopoDS_Wire &wire) {
                  auto faceBuilder = BRepBuilderAPI_MakeFace(wire, true);
                  TopoDS_Shape face = faceBuilder.Shape();

                  this->m_faces.push_back(face);
                });

  std::cout << this->m_faces.size() << std::endl;

  // Build compound
  BRep_Builder builder;
  builder.MakeCompound(this->m_compound);
  for (uint i = 0; i < this->m_faces.size(); ++i) {
    builder.Add(this->m_compound, this->m_faces[i]);
  }
  // std::for_each(this->m_faces.begin(), this->m_faces.end(),
  //               [this, builder](const TopoDS_Shape &face) {
  //                 builder.Add(this->m_compound, face);
  //               });

  this->clear();
}
