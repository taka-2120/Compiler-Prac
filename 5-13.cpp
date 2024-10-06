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

Function *make_asum()
{
  // Declaration
  Variable *n = new Variable(Type_INT, "n");
  Variable *s = new Variable(Type_INT, "s");
  Variable *i = new Variable(Type_INT, "i");
  std::list<Variable *> args;
  args.push_back(n);
  std::list<Variable *> lvars;
  lvars.push_back(s);
  lvars.push_back(i);

  // Body
  Exp_variable *n_v = new Exp_variable("n");
  Exp_variable *s_v = new Exp_variable("s");
  Exp_variable *i_v = new Exp_variable("i");
  Statement *st_assign_s = new St_assign(s_v, new Exp_constant(Type_INT, 0));
  Statement *st_assign_i = new St_assign(i_v, new Exp_operation1(Operator_MINUS, n_v));

  Expression *cond = new Exp_operation2(Operator_LE, i_v, n_v);
  Statement *st_if = make_if();
  Statement *st_while = new St_while(cond, st_if);

  Statement *st_return = new St_return(s_v);

  list<Statement *> body;
  body.push_back(st_assign_s);
  body.push_back(st_assign_i);
  body.push_back(st_while);
  body.push_back(st_return);
  Statement *st_list = new St_list(body);

  Function *f = new Function(Type_INT, "asum", args, lvars, st_list);
  return f;
}

int main(void)
{
  make_asum()->print(cout);
  return 0;
}