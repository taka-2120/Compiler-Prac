#include <iostream>
#include "ast.h"
using namespace std;

int main(void)
{
  Expression *a = new Exp_variable("a");
  std::list<Expression *> args;
  args.push_back(a);

  Statement *st_function = new St_function("putint", args);
  st_function->print(std::cout);
  return 0;
}