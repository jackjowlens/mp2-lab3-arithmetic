#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

#ifndef __TStack_H__
#define __TStack_H__

template <typename T> class TStack {
protected:
  int size;
  T *mem;
  int top;

public:
  TStack(int size = 10) {
    top = -1;
    this->size = size;
    mem = new T[this->size];
  }
  ~TStack() { delete[] mem; }
  void push(const T &val) {
    if (top < size - 1) {
      mem[top + 1] = val;
      top++;
    } else {
      T *tmp = new T[size * 2];
      int oldCount = top + 1;
      size *= 2;
      copy(mem, mem + oldCount, tmp);
      swap(mem, tmp);
      delete[] tmp;
      mem[top + 1] = val;
      top++;
    }
  }
  T pop() {
    if (top >= 0) {
      T val = mem[top];
      top--;
      return val;
    } else {
      throw invalid_argument("Stack now is empty");
    }
  }
  bool isEmpty() { return top == -1; }

  T Top() {
    if (top == -1) {
      throw invalid_argument("Stack now is empty");
    } else {
      return mem[top];
    }
  }

  TStack<T> reverse() {
    TStack<T> rev;
    int count = top + 1;
    T *tmp = new T[count];
    for (int i = 0; i < count; i++) {
      tmp[count - 1 - i] = mem[i];
    }
    for (int i = 0; i < count; i++) {
      rev.push(tmp[i]);
    }
    delete[] tmp;
    return rev;
  }

  int get_size() { return (size); }

  int get_count() { return top + 1; }
};

#endif