// gmsh -1 Point.geo -setnumber h 0.1

// ELEMENTARY
// Points
p = newp;
Point(p+0) = {0, 0, 0, h};

// PHYSICAL
// Points
Physical Point("1", 1) = {p+0};