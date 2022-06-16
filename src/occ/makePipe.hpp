#ifndef _MAKE_PIPE_
#define _MAKE_PIPE_

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRep_Tool.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <gp_Ax1.hxx>
#include <gp_Circ.hxx>
#include <gp_Dir.hxx>
#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>

TopoDS_Shape makePipe(const double radius, const TopoDS_Edge edge) {
  // Analysis
  ShapeAnalysis_Edge analysis;

  // Profil
  TopoDS_Vertex firstV = analysis.FirstVertex(edge);
  gp_Pnt center = BRep_Tool::Pnt(firstV);

  gp_Pln plan(center, gp_Dir(0, 0, 1));
  BRepBuilderAPI_MakeFace faceBuilder(plan);
  TopoDS_Face face = faceBuilder.Face();

  gp_Pnt2d position(center.X(), center.Y());
  gp_Vec2d tangent;
  analysis.GetEndTangent2d(edge, face, false, position, tangent);

  gp_Circ circle;
  circle.SetRadius(radius);
  circle.SetLocation(center);
  circle.SetAxis(gp_Ax1(center, gp_Dir(tangent.X(), tangent.Y(), 0)));

  BRepBuilderAPI_MakeEdge circleEdgeBuilder(circle);
  BRepBuilderAPI_MakeWire circleWireBuilder(circleEdgeBuilder.Edge());
  BRepBuilderAPI_MakeFace profileBuilder(circleWireBuilder.Wire());
  TopoDS_Shape profil = profileBuilder.Shape();

  // Wire
  BRepBuilderAPI_MakeWire wireBuilder(edge);
  TopoDS_Wire wire = wireBuilder.Wire();

  // Pipe
  BRepOffsetAPI_MakePipe pipeBuilder(wire, profil);

  // Shape
  TopoDS_Shape pipe = pipeBuilder.Shape();

  // Only the generated face
  TopExp_Explorer explorer;
  explorer.Init(pipe, TopAbs_FACE);
  explorer.Next();

  return explorer.Current();
}

#endif //_MAKE_PIPE_
