#include <iostream>
#include "Wrapper.h"
#include "Engine.h"

class Subject {
public:
  int f1(int x) { return -x; };
  int f2(int x, int y, int z) { return x + y + z; };
  int f3(int x, int y) { return x * y; };
};


int main() {
  Subject subj;
  Wrapper wrapper1(&subj, &Subject::f1, { {"x", 1}});
  Wrapper wrapper2(&subj, &Subject::f2, { {"a", 0}, {"b", 2}, {"c", 0}});
  Wrapper wrapper3(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });

  Engine engine;

  engine.registerCommand(&wrapper1, "negative");
  engine.registerCommand(&wrapper2, "summarize3");
  engine.registerCommand(&wrapper3, "multiply");

  std::cout << engine.execute("negative", { {"x", 4} }) << std::endl;
  std::cout << engine.execute("summarize3", { {"a", 4}, {"b", 10}, {"c", 1}}) << std::endl;
  std::cout << engine.execute("multiply", { {"arg1", 4}, {"arg2", 5} }) << std::endl;
}