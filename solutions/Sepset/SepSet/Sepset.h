class Sepset {
 public:
  int *val;
  int* h;
  int size;
  Sepset() {};
  Sepset(int size_);
  void singleton(int i);
  void merge(int i, int j);
  int poisk(int i);
  void clear();
  ~Sepset();
};