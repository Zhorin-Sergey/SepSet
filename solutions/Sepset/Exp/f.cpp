#include "f.h"

void countino(Sepset *set, int *co, int n, int m) {
  bool f = false;
  int x;
  int y;
  srand(time(0));
  int coord;
  while (f == false && (*co) < n*m) {
    do {
      x = rand() % n;
      y = rand() % m;
    } while ((*set).val[x + (y + 1)*n] != -1);
    (*co)++;
    coord = x + (y + 1)*n;
    (*set).singleton(coord);
    if (x != 0)
      if ((*set).val[coord - 1] != -1)
        (*set).merge(coord, coord - 1);
    if ((*set).val[coord - n] != -1)
      (*set).merge(coord, coord - n);
    if (x != n - 1)
      if ((*set).val[coord + 1] != -1)
        (*set).merge(coord, coord + 1);
    if ((*set).val[coord + n] != -1)
      (*set).merge(coord, coord + n);
    f = ((*set).poisk(0) == (*set).poisk((*set).size - 1));
  }
}

void preparation(Sepset *set, int n, int m) {
  (*set).clear();
  for (int i = 0; i < n; i++) {
    (*set).singleton(i);
    (*set).singleton((*set).size - i - 1);
  }
  for (int i = 1; i < n; i++) {
    (*set).merge(0, i);
    (*set).merge((*set).size - 1, (*set).size - i - 1);
  }
}
