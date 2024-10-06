#include <iostream>
#include "ast.h"
using namespace std;

int main(void)
{
  Expression *v = new Exp_variable("n");
  Expression *o = new Exp_operation1(Operator_MINUS, v);
  o->print(std::cout);
  std::cout << std::endl;
  return 0;
}