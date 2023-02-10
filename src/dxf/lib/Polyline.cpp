#include "Polyline.hpp"

#include <algorithm>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>

#include "./Defs.hpp"
#include "./Utils.hpp"

#include "../../logger/Logger.hpp"

/**
 * Process
 * @param file File
 */
void DXFPolyline::process(std::ifstream &file) {
  Logger::DEBUG("POLYLINE::Process");

  std::string line;
  while (std::getline(file, line)) {
    rtrim(line);
    if (line == VERTEX) {
      DXFVertex vertex;
      vertex.process(file);

      if (!vertex.alreadyExists(this->vertices))
        this->vertices.push_back(vertex);
    } else if (line == SEQEND)
      break;
  }
}

/**
 * Is empty?
 * @return true
 * @return false
 */
bool DXFPolyline::isEmpty() const { return this->vertices.size() < 2; }

/**
 * Already exists?
 * @param polylines
 * @return true
 * @return false
 */
bool DXFPolyline::alreadyExists(
    const std::vector<DXFPolyline> &polylines) const {
  bool res = false;
  std::for_each(polylines.begin(), polylines.end(),
                [this, &res](const DXFPolyline &polyline) {
                  if (*this == polyline)
                    res = true;
                });

  return res;
}

/**
 * Add to wire builder
 * @param wireBuilder Wire builder
 */
void DXFPolyline::addToWireBuilder(BRepBuilderAPI_MakeWire &wireBuilder) const {
  Logger::DEBUG("POLYLINE::addToWireBuilder");
  const size_t size = this->vertices.size();

  for (size_t i = 0; i < size; ++i) {
    const DXFVertex v1 = this->vertices.at(i);
    const DXFVertex v2 = this->vertices.at((i + 1) % size);

    gp_Pnt point1(v1.x, v1.y, v1.z);
    gp_Pnt point2(v2.x, v2.y, v2.z);

    BRepBuilderAPI_MakeVertex vertexBuilder1(point1);
    BRepBuilderAPI_MakeVertex vertexBuilder2(point2);

    TopoDS_Vertex vertex1 = vertexBuilder1.Vertex();
    TopoDS_Vertex vertex2 = vertexBuilder2.Vertex();

    auto edgeBuilder = BRepBuilderAPI_MakeEdge(vertex1, vertex2);
    TopoDS_Edge edge = edgeBuilder.Edge();

    wireBuilder.Add(edge);

    if (const auto error = wireBuilder.Error();
        error != BRepBuilderAPI_WireDone) {
      Logger::WARNING("POLYLINE::addToWireBuilder error");
      Logger::WARNING("" + std::to_string(error));
    }
  }
}

/**
 * Operator ==
 * @param polyline1 Polyline 1
 * @param polyline2 Polyline 2
 * @return true
 * @return false
 */
bool operator==(const DXFPolyline &polyline1, const DXFPolyline &polyline2) {
  bool res = true;

  if (polyline1.vertices.size() != polyline2.vertices.size())
    return false;

  std::for_each(polyline1.vertices.begin(), polyline1.vertices.end(),
                [index = 0, &polyline2, &res](const DXFVertex vertex1) mutable {
                  if (!(vertex1 == polyline2.vertices.at(index)))
                    res = false;

                  index++;
                });

  return res;
}
