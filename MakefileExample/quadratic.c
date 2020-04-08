#include <math.h>
#include <stdio.h>

int quadratic_solution (a, b, c)
int a, b, c;
{ double fa, fb, fc, x, s1, s2;
  int count;

  fa = (double) a;
  fb = (double) b;
  fc = (double) c;
  x = fb * fb - 4 * fa * fc;
  s1 = ( - fb + sqrt (x) ) / (2*fa);
  s2 = ( - fb - sqrt (x) ) / (2*fa);
    printf ("(%d, %d, %d) solutions = %f, %f\n", a, b, c, s1, s2);

  count = 0;
  if ( s1 == rint(s1) ) count++;
  if ( s2 == rint(s2) ) count++;
  printf ("%d integer solutions\n", count);
}

void solve_function ()
{ int a, b, c;

  printf ("input a b c (ax^2+bx+c=0):  ");
  scanf ("%d %d %d", &a, &b, &c);
  quadratic_solution (a, b, c);
}