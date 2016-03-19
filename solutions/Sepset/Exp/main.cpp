#include "f.h"

void main() {
  int numb = 7000;
  int n = 100;
  int m = 100;
  int size = n*m + 2 * n;
  Sepset set(size);
  double count;
  double result = 0;
  for (int i = 0; i < numb; i++) {
    preparation(&set, n, m);
    count = 0;
    countino(&set, &count, n, m);
    result += count / (m * n);
  }
  printf("%f\n", result / numb);
}
