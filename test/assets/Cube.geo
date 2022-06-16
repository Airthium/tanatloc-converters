// gmsh -3 Cube.geo -setnumber L 1 -setnumber h 0.1 -format msh2

Mesh.Optimize = 1;

// ELEMENTARY
//Points
p = newp;
Point(p+0) = {0, 0, 0, h};
Point(p+1) = {L, 0, 0, h};
Point(p+2) = {L, L, 0, h};
Point(p+3) = {0, L, 0, h};

Point(p+5) = {0, 0, L, h};
Point(p+6) = {L, 0, L, h};
Point(p+7) = {L, L, L, h};
Point(p+8) = {0, L, L, h};

//Lines
l = newl;
Line(l+0) = {p+0, p+1};
Line(l+1) = {p+1, p+2};
Line(l+2) = {p+2, p+3};
Line(l+3) = {p+3, p+0};

Line(l+5) = {p+5, p+6};
Line(l+6) = {p+6, p+7};
Line(l+7) = {p+7, p+8};
Line(l+8) = {p+8, p+5};

Line(l+10) = {p+0, p+5};
Line(l+11) = {p+1, p+6};
Line(l+12) = {p+2, p+7};
Line(l+13) = {p+3, p+8};

//Line Loops
ll = newll;
Line Loop(ll+0) = {l+0, l+1, l+2, l+3};
Line Loop(ll+1) = {l+5, l+6, l+7, l+8};
Line Loop(ll+2) = {l+0, l+11, -(l+5), -(l+10)};
Line Loop(ll+3) = {l+1, l+12, -(l+6), -(l+11)};
Line Loop(ll+4) = {l+2, l+13, -(l+7), -(l+12)};
Line Loop(ll+5) = {l+3, l+10, -(l+8), -(l+13)};

//Surfaces
s = news;
Plane Surface(s+0) = {ll+0};
Plane Surface(s+1) = {ll+1};
Plane Surface(s+2) = {ll+2};
Plane Surface(s+3) = {ll+3};
Plane Surface(s+4) = {ll+4};
Plane Surface(s+5) = {ll+5};

//Surface Loops
sl = newsl;
Surface Loop(sl+0) = {s+0, s+1, s+2, s+3, s+4, s+5};

//Volumes
v = news;
Volume(v+0) = {sl+0};

// PHYSICAL
//Surfaces
Physical Surface("1", 1) = {s+0};
Physical Surface("2", 2) = {s+1};
Physical Surface("3", 3) = {s+2};
Physical Surface("4", 4) = {s+3};
Physical Surface("5", 5) = {s+4};
Physical Surface("6", 6) = {s+5};

//Volume
Physical Volume("1", 1) = {v+0};
