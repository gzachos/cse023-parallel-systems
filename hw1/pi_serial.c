/* pi.c
 * ----
 *   Serial pi calculation.
 *
 * VVD
 */
#include <stdio.h>

#define N     10000000      /* Intervals */


main()
{
  int    i;
  double W = 1.0 / ((double) N), W2, pi = 0.0;

  W2 = W*W;   /* Save some calculations */
  W = 4*W;
  for (i = 0; i < N; i ++)
    pi += W / (1.0 + (0.5 + (double) i)*(0.5 + (double) i)*W2);
  printf("pi = %.10lf\n", pi);
}


