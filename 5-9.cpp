#include <iostream>
#include "ast.h"
using namespace std;

Statement *make_if()
{
  Exp_variable *i = new Exp_variable("i");
  Exp_variable *s = new Exp_variable("s");
  Exp_constant *zero = new Exp_constant(Type_INT, 0);
  // cond
  Expression *cond = new Exp_operation2(Operator_LT, i, zero);
  // then
  Expression *then_exp = new Exp_operation2(Operator_MINUS, s, i);
  Statement *then = new St_assign(s, then_exp);
  // else
  Expression *else_exp = new Exp_operation2(Operator_PLUS, s, i);
  Statement *els = new St_assign(s, else_exp);

  return new St_if(cond, then, els);
}

int main(void)
{
  Exp_variable *i = new Exp_variable("i");
  Exp_variable *n = new Exp_variable("n");
  Expression *cond = new Exp_operation2(Operator_LE, i, n);

  Statement *st_if = make_if();

  Statement *st_while = new St_while(cond, st_if);
  st_while->print(std::cout);
  return 0;
}