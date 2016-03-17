#include "f.h"

void main() {
  int numb = 1000;
  int n = 5;
  int m = 5;
  int size = n*m + 2 * n;
  Sepset set(size);
  int count;
  double result = 0;
  for (int i = 0; i < numb; i++) {
    preparation(&set, n, m);
    count = 0;
    countino(&set, &count, n, m);
    result += count;
  }
  printf("%f\n", result / numb);
}
