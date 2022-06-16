// gmsh -2 Square.geo -setnumber L 1 -setnumber h 0.1

// ELEMENTARY
// Points
p = newp;
Point(p+0) = {0, 0, 0, h};
Point(p+1) = {L, 0, 0, h};
Point(p+2) = {L, L, 0, h};
Point(p+3) = {0, L, 0, h};

// Lines
l = newl;
Line(l+0) = {p+0, p+1};
Line(l+1) = {p+1, p+2};
Line(l+2) = {p+2, p+3};
Line(l+3) = {p+3, p+0};

// Line loops
ll = newll;
Line Loop(ll+0) = {l+0, l+1, l+2, l+3};

// Surface
s = news;
Plane Surface(s+0) = {ll+0};

// PHYSICAL
// Lines
Physical Line("1", 1) = {l+0};
Physical Line("2", 2) = {l+1};
Physical Line("3", 3) = {l+2};
Physical Line("4", 4) = {l+3};

// Surfaces
Physical Surface("1", 1) = {s+0};