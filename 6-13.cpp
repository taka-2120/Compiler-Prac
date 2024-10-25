#include <iostream>
#include "ast.h"

Statement *make_if()
{
  Exp_variable *i = new Exp_variable("i");
  Exp_variable *s = new Exp_variable("s");
  Exp_constant *zero = new Exp_constant(Type_INT, 0);

  Expression *cond = new Exp_operation2(Operator_LT, i, zero);

  Expression *then_exp = new Exp_operation2(Operator_MINUS, s, i);
  Statement *then = new St_assign(s, then_exp);

  Expression *else_exp = new Exp_operation2(Operator_PLUS, s, i);
  Statement *els = new St_assign(s, else_exp);

  return new St_if(cond, then, els);
}

Statement *make_while()
{

  Expression *v11 = new Exp_variable("i");
  Expression *v12 = new Exp_variable("n");
  Expression *cond = new Exp_operation2(Operator_LE, v11, v12);

  std::list<Statement *> slist;

  Statement *b1 = make_if();
  slist.push_back(b1);

  Expression *v21 = new Exp_variable("i");
  Expression *c2 = new Exp_constant(Type_INT, 1);
  Expression *e2 = new Exp_operation2(Operator_PLUS, v21, c2);
  Exp_variable *v22 = new Exp_variable("i");
  Statement *b2 = new St_assign(v22, e2);
  slist.push_back(b2);
  Statement *body = new St_list(slist);

  Statement *s = new St_while(cond, body);
  return s;
}

Function *make_asum()
{

  Variable *n = new Variable(Type_INT, "n");
  Variable *s = new Variable(Type_INT, "s");
  Variable *i = new Variable(Type_INT, "i");
  std::list<Variable *> args;
  args.push_back(n);
  std::list<Variable *> lvars;
  lvars.push_back(s);
  lvars.push_back(i);

  Exp_variable *n_v = new Exp_variable("n");
  Exp_variable *s_v = new Exp_variable("s");
  Exp_variable *i_v = new Exp_variable("i");
  Statement *st_assign_s = new St_assign(s_v, new Exp_constant(Type_INT, 0));
  Statement *st_assign_i = new St_assign(i_v, new Exp_operation1(Operator_MINUS, n_v));

  Expression *cond = new Exp_operation2(Operator_LE, i_v, n_v);
  Statement *st_if = make_if();
  Statement *st_while = make_while();

  Statement *st_return = new St_return(s_v);

  std::list<Statement *> body;
  body.push_back(st_assign_s);
  body.push_back(st_assign_i);
  body.push_back(st_while);
  body.push_back(st_return);
  Statement *st_list = new St_list(body);

  Function *f = new Function(Type_INT, "asum", args, lvars, st_list);
  return f;
}

Function *make_main()
{
  std::list<Variable *> args;
  std::list<Variable *> lvars;
  Variable *a = new Variable(Type_INT, "a");
  lvars.push_back(a);

  Exp_variable *a_v = new Exp_variable("a");

  Exp_variable *g_v = new Exp_variable("g");
  Expression *c1 = new Exp_constant(Type_INT, 3);
  Statement *g_as = new St_assign(g_v, c1);

  std::list<Expression *> args1;
  args1.push_back(g_v);
  Expression *asum_func = new Exp_function("asum", args1);
  Statement *a_as = new St_assign(a_v, asum_func);

  std::list<Expression *> args2;
  args2.push_back(a_v);
  Statement *putint = new St_function("putint", args2);

  std::list<Statement *> body;
  body.push_back(g_as);
  body.push_back(a_as);
  body.push_back(putint);
  St_list *st_list = new St_list(body);

  return new Function(Type_INT, "main", args, lvars, st_list);
}

int main()
{
  std::list<Variable *> gvar;
  std::list<Function *> func;
  gvar.push_back(new Variable(Type_INT, "g"));
  func.push_back(make_asum());
  Program *p = new Program(gvar, func, make_main());

  p->run();

  return 0;
}