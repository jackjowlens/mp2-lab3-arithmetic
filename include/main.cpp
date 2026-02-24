
// #include "../../lists/include/TPolinom.h"
#include "art.h"
#include <iostream>
#include <map>

using namespace std;

int main() {
  cout << input("x*y+z", 5)<<endl;
  string expr = "x*y+z+z";
  map<string, TPolinom> vars = {{"x", TPolinom("1x1y1z0")},
                                {"y", TPolinom("3x0y0z0")},
                                {"z", TPolinom("9x2y1z0+2x1y1z0")}};

  TPolinom t=execute(expr, vars);
  cout<<t<<endl;
  cout<<t.Calculate_At(1,2,3)<<endl;

  return 0;
}