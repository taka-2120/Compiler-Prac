#include <iostream>
#include "ast.h"
using namespace std;

int main(void)
{
  Variable *a = new Variable(Type_INT, "a");
  a->print(cout);
  return 0;
}