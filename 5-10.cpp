#include <iostream>
#include "ast.h"
using namespace std;

int main(void)
{
  Exp_variable *s = new Exp_variable("s");
  Statement *st_return = new St_return(s);
  st_return->print(std::cout);
  return 0;
}