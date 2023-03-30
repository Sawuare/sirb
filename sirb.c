#include <math.h>
#include <stdio.h>

#define FILENAME "sirb.svg"

static FILE *file;

static double o; // Rhombus opening angle
static double a; // Rhombus acute angle
static double s; // Rhombus side
static double d; // Rhombus diagonal that bisects a
static double l; // Coordinate limit

static double shoulder1x, shoulder1y, shoulder2x, shoulder2y;

void rhombus(double x, double y, double direction, _Bool set_shoulders) {
  double vertex2x = x + cos(direction + o) * s;
  double vertex2y = y + sin(direction + o) * s;

  double vertex4x = x + cos(direction - o) * s;
  double vertex4y = y + sin(direction - o) * s;

  double vertex3x = x + cos(direction) * d;
  double vertex3y = y + sin(direction) * d;

  fprintf(file,
    "  <polygon points='%+f,%+f %+f,%+f %+f,%+f %+f,%+f'/>\n",
    x, -y, vertex2x, -vertex2y, vertex3x, -vertex3y, vertex4x, -vertex4y);

  if (set_shoulders) {
    shoulder1x = vertex2x;
    shoulder1y = vertex2y;
    shoulder2x = vertex4x;
    shoulder2y = vertex4y;
  }
}

int main(void) {
  o = M_PI / 12;
  a = 2 * o;
  s = 2;
  d = 2 * cos(o) * s;
  l = 4 * s;

  file = fopen(FILENAME, "w");

  if (!file)
    return 1;

  fprintf(file,
    "<?xml version='1.0'?>\n"
    "<svg xmlns='http://www.w3.org/2000/svg' "
    "viewBox='%g %g %g %g' width='512px' height='512px' fill='white'>\n"
    "  <rect x='%g' y='%g' width='100%%' height='100%%' fill='black'/>\n",
    -l, -l, 2 * l, 2 * l, -l, -l);

  double direction = a;

  for (int bird = 0; bird < 6; ++bird) {
    rhombus(cos(direction - a) * d, sin(direction - a) * d, direction, 1);
    rhombus(shoulder1x, shoulder1y, direction + 2 * M_PI / 3, 0);
    rhombus(shoulder2x, shoulder2y, direction - 2 * M_PI / 3, 0);

    direction += M_PI / 3;
  }

  fputs("</svg>\n", file);
  printf("Wrote %s\n", FILENAME);
}
