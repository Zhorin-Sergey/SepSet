#ifndef SOLUTIONS_SEPSET_SEPSET_SEPSET_H_
#define SOLUTIONS_SEPSET_SEPSET_SEPSET_H_

class Sepset {
 public:
  int *val;
  int* h;
  int size;
  Sepset() {}
  explicit Sepset(int size_);
  void singleton(int i);
  void merge(int i, int j);
  int poisk(int i);
  void clear();
  ~Sepset();
};

#endif  // SOLUTIONS_SEPSET_SEPSET_SEPSET_H_
