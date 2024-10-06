#include <iostream>
#include "ast.h"
using namespace std;

int main(void)
{
  Expression *c1 = new Exp_constant(Type_INT, 7);
  Expression *c2 = new Exp_constant(Type_CHAR, 'x');
  c1->print(cout);
  cout << endl;
  c2->print(cout);
  cout << endl;
  return 0;
}