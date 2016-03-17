#include "SepSet.h"
#include "stdio.h"
#include <exception>

Sepset::Sepset(int size_) {
  size = size_;
  val = new int[size];
  h = new int[size];
  for (int i = 0; i < size; i++) {
    val[i] = -1;
    h[i] = -1;
  }
}

void Sepset::singleton(int i) {
  if (i < 0 || i > size || val[i] != -1)
    throw "wrong index";
  val[i] = i;
  h[i] = 0;	
}

void Sepset::merge(int i, int j) {
  if (i < 0 || i > size || j < 0 || j > size || val[i] == -1 || val[j] == -1)
    throw "wrong index";
  if (i != j) {
    int a = i;
    int b = j;
    while (val[a] != a)
      a = val[a];
    while (val[b] != b)
      b = val[b];
    if (a != b) {
      if (h[a] < h[b])
        val[a] = b;
      else {
        val[b] = a;
        if (h[a] == h[b])
          ++h[a];
      }
    }
  }
}

int Sepset::poisk(int i) {
  if (i < 0 || i > size || val[i] == -1) 
    throw "wrong index";
  while (val[i] != i)
    i = val[i];	
  return val[i];
}

void Sepset::clear() {
  for (int i = 0; i < size; i++) {
    val[i] = -1;
    h[i] = -1;
  }
}

Sepset::~Sepset() {
  delete[] val;
  delete[] h;
}