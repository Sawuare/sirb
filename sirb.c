// sirb.c - write Sirb to an SVG file

#include <math.h>
#include <stdio.h>

// Full angle in radians
#define TAU 6.28318530717958647692

typedef struct point {
  double x, y;
} point;

static point left_shoulder, right_shoulder;

static FILE *file;

static double a; // Rhombus acute angle
static double o; // Rhombus opening angle
static double s; // Rhombus side
static double d; // Rhombus diagonal that bisects a
static double l; // Coordinate limit

static void rhombus(point base_vertex, double direction, _Bool set_shoulders) {
  point left_vertex = {
    base_vertex.x + cos(direction + o) * s,
    base_vertex.y + sin(direction + o) * s
  };

  point right_vertex = {
    base_vertex.x + cos(direction - o) * s,
    base_vertex.y + sin(direction - o) * s
  };

  point apex_vertex = {
    base_vertex.x + cos(direction) * d,
    base_vertex.y + sin(direction) * d
  };

  fprintf(file,
    "  <polygon points='%+f,%+f %+f,%+f %+f,%+f %+f,%+f'/>\n",
    base_vertex.x,  -base_vertex.y,
    left_vertex.x,  -left_vertex.y,
    apex_vertex.x,  -apex_vertex.y,
    right_vertex.x, -right_vertex.y);

  if (set_shoulders) {
    left_shoulder = left_vertex;
    right_shoulder = right_vertex;
  }
}

int main(void) {
  a = TAU / 12;
  o = a / 2;
  s = 1;
  d = 2 * cos(o) * s;
  l = 4 * s;

  file = fopen("sirb.svg", "w");

  if (!file)
    return 1;

  fprintf(file,
    "<?xml version='1.0'?>\n"
    "<svg xmlns='http://www.w3.org/2000/svg' "
    "viewBox='%g %g %g %g' width='256px' height='256px' fill='black'>\n"
    "  <rect x='%g' y='%g' width='100%%' height='100%%' fill='white'/>\n",
    -l, -l, 2 * l, 2 * l, -l, -l);

  int birds = 6;
  double angle = 0;
  double direction = angle + a;
  double rotation = TAU / birds;
  double wing_offset = TAU / 3;

  while (birds--) {
    point tail = {cos(angle) * d, sin(angle) * d};

    rhombus(tail, direction, 1); // Torso
    rhombus(left_shoulder,  direction + wing_offset, 0); // Left wing
    rhombus(right_shoulder, direction - wing_offset, 0); // Right wing

    angle += rotation;
    direction += rotation;
  }

  fputs("</svg>\n", file);
  fclose(file);
}
