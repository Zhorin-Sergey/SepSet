#include "f.h"

void main(int argc, char *argv[]) {
  int numb = 1000;
  int n = 100;
  int m = 100;
  if (argc != 1) {
    numb = atoi(argv[1]);
    n = atoi(argv[2]);
    m = atoi(argv[3]);
  }
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
