#include "Spline.hpp"

#include <algorithm>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierCurve.hxx>

#include "./Defs.hpp"
#include "./Utils.hpp"

#include "../../logger/Logger.hpp"

/**
 * Process
 * @param file File
 */
void DXFSpline::process(std::ifstream &file) {
  Logger::DEBUG("SPLINE::Process");

  std::string line;
  bool start = false;
  DXFVertex tmpVertex;
  while (std::getline(file, line)) {
    rtrim(line);
    if (line == "AcDbSpline")
      start = true;

    if (start) {
      if (line == TAG210) {
        file >> this->normal.x;
      } else if (line == TAG220) {
        file >> this->normal.y;
      } else if (line == TAG230) {
        file >> this->normal.z;
      } else if (line == TAG71) {
        file >> this->degree;
      } else if (line == TAG72) {
        file >> this->numberOfKnots;
      } else if (line == TAG73) {
        file >> this->numberOfControlPoints;
      } else if (line == TAG74) {
        file >> this->numberOfFitPoints;
      } else if (line == TAG40) {
        float knotValue;
        file >> knotValue;
        this->knotsValues.push_back(knotValue);
      } else if (line == TAG10) {
        file >> tmpVertex.x;
      } else if (line == TAG20) {
        file >> tmpVertex.y;
      } else if (line == TAG30) {
        file >> tmpVertex.z;
        // Force z to 0
        tmpVertex.z = 0.;
        this->controlPoints.push_back(tmpVertex);
      } else if (line == TAG11) {
        file >> tmpVertex.x;
      } else if (line == TAG21) {
        file >> tmpVertex.y;
      } else if (line == TAG31) {
        file >> tmpVertex.z;
        // Force z to 0
        tmpVertex.z = 0.;
        this->fitPoints.push_back(tmpVertex);
      } else if (line == TAG0)
        break;
    }
  }

  // Check XY
  if (!this->inXY()) {
    this->numberOfKnots = 0;
    this->numberOfControlPoints = 0;
    this->numberOfFitPoints = 0;
    this->knotsValues.clear();
    this->controlPoints.clear();
    this->fitPoints.clear();
  }
}

/**
 * Is empty?
 * @return true
 * @return false
 */
bool DXFSpline::isEmpty() const {
  return this->knotsValues.empty() || this->controlPoints.empty();
}

/**
 * In X-Y?
 * @return true
 * @return false
 */
bool DXFSpline::inXY() const {
  if (this->normal.x == 0. && this->normal.y == 0. && this->normal.z == 0.) {
    // Unknown normal
    return true;
  }

  return (this->normal.x == 0 && this->normal.y == 0.);
}

/**
 * Already exists?
 * @param splines Splines
 * @return true
 * @return false
 */
bool DXFSpline::alreadyExists(const std::vector<DXFSpline> &splines) const {
  bool res = false;
  std::for_each(splines.begin(), splines.end(),
                [this, &res](const DXFSpline &spline) {
                  if (*this == spline)
                    res = true;
                });

  return res;
}

/**
 * Add to wire builder
 * @param wireBuilder Wire builder
 */
void DXFSpline::addToWireBuilder(BRepBuilderAPI_MakeWire &wireBuilder) const {
  Logger::DEBUG("SPLINE::addToWireBuilder");
  TColgp_Array1OfPnt poles(1, this->numberOfControlPoints);
  TColStd_Array1OfReal weights(1, this->numberOfControlPoints);
  TColStd_Array1OfReal knots(1, this->numberOfKnots);
  TColStd_Array1OfInteger multiplicities(1, this->numberOfKnots);

  for (int i = 0; i < this->numberOfControlPoints; ++i) {
    DXFVertex vertex = this->controlPoints.at(i);
    gp_Pnt controlPoint(vertex.x, vertex.y, vertex.z);
    poles.SetValue(i + 1, controlPoint);

    weights.SetValue(i + 1, 1.);
  }

  for (int i = 0; i < this->numberOfKnots; ++i) {
    float knot = this->knotsValues.at(i);
    knots.SetValue(i + 1, knot);

    multiplicities.SetValue(i + 1, 1.);
  }

  //   auto splineCurve = Geom_BSplineCurve(poles, weights, knots,
  //   multiplicities, this->degree, false, false);

  auto splineCurve = Geom_BezierCurve(poles); // TODO replace by B-Spline

  const auto U1 = splineCurve.FirstParameter();
  const auto U2 = splineCurve.LastParameter();

  int numberOfDisplay = this->numberOfControlPoints + 2;
  std::vector<gp_Pnt> vertices;
  for (int i = 0; i < numberOfDisplay; ++i) {
    const auto point = U1 + (U2 - U1) * (i / (numberOfDisplay - 1.));
    vertices.push_back(splineCurve.Value(point));
  }

  for (int i = 0; i < numberOfDisplay - 1; ++i) {
    const auto &point1 = vertices.at(i);
    const auto &point2 = vertices.at(i + 1);

    BRepBuilderAPI_MakeVertex vertexBuilder1(point1);
    BRepBuilderAPI_MakeVertex vertexBuilder2(point2);

    TopoDS_Vertex vertex1 = vertexBuilder1.Vertex();
    TopoDS_Vertex vertex2 = vertexBuilder2.Vertex();

    auto edgeBuilder = BRepBuilderAPI_MakeEdge(vertex1, vertex2);
    TopoDS_Edge edge = edgeBuilder.Edge();

    wireBuilder.Add(edge);
    const auto error = wireBuilder.Error();
    if (error != BRepBuilderAPI_WireDone) {
      Logger::WARNING("SPLINE::addToWireBuilder error");
      Logger::WARNING("" + std::to_string(error));
    }
  }
}

/**
 * Operator ==
 * @param spline1 Spline 1
 * @param spline2 Spline 2
 * @return true
 * @return false
 */
bool operator==(const DXFSpline &spline1, const DXFSpline &spline2) {
  bool res;

  if (spline1.degree != spline2.degree)
    return false;
  if (spline1.numberOfKnots != spline2.numberOfKnots)
    return false;
  if (spline1.numberOfControlPoints != spline2.numberOfControlPoints)
    return false;
  if (spline1.numberOfFitPoints != spline2.numberOfFitPoints)
    return false;

  std::for_each(spline1.knotsValues.begin(), spline1.knotsValues.end(),
                [index = 0, &spline2, &res](const float knot1) mutable {
                  if (knot1 != spline2.knotsValues.at(index))
                    res = false;

                  index++;
                });

  std::for_each(
      spline1.controlPoints.begin(), spline1.controlPoints.end(),
      [index = 0, &spline2, &res](const DXFVertex controlPoint) mutable {
        if (!(controlPoint == spline2.controlPoints.at(index)))
          res = false;

        index++;
      });

  std::for_each(spline1.fitPoints.begin(), spline1.fitPoints.end(),
                [index = 0, &spline2, &res](const DXFVertex fitPoint) mutable {
                  if (!(fitPoint == spline2.fitPoints.at(index)))
                    res = false;

                  index++;
                });

  return true;
}