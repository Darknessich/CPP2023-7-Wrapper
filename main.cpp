#include "Wrapper.h"
#include "Engine.h"

class Subject {
public:
  int f1(int x) { return -x; };
  int f2(int x, int y, int z) { return x + y + z; };
  int f3(int x, int y) { return x * y; };
};

#include <iostream>
int main() {
  Subject subj;

  Wrapper wrapper(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0}});

  std::cout << wrapper({ {"arg1", 9}, {"arg2", 2} }) << std::endl;
}