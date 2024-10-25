#include <iostream>
#include "ast.h"

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

Statement *make_while()
{
  // cond: i<=n
  Expression *v11 = new Exp_variable("i");
  Expression *v12 = new Exp_variable("n");
  Expression *cond = new Exp_operation2(Operator_LE, v11, v12);

  // body
  std::list<Statement *> slist;
  // if 文
  Statement *b1 = make_if();
  slist.push_back(b1);
  // i = i + 1;
  Expression *v21 = new Exp_variable("i");
  Expression *c2 = new Exp_constant(Type_INT, 1);
  Expression *e2 = new Exp_operation2(Operator_PLUS, v21, c2);
  Exp_variable *v22 = new Exp_variable("i");
  Statement *b2 = new St_assign(v22, e2);
  slist.push_back(b2);
  Statement *body = new St_list(slist);

  // while 文全体
  Statement *s = new St_while(cond, body);
  return s;
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

int main()
{
  // Function *function_asum = make_asum();
  // std::map<std::string, Function *> func;
  // std::map<std::string, int> gvar;
  // std::list<int> i_arglist;
  // i_arglist.push_back(3);
  // int r = function_asum->run(func, gvar, i_arglist);
  // std::cout << r << std::endl;

  std::list<Expression *> arglist;
  arglist.push_back(new Exp_constant(Type_INT, 5));
  Expression *exp_function_asum = new Exp_function("asum", arglist);
  std::map<std::string, Function *> func;
  std::map<std::string, int> gvar;
  std::map<std::string, int> lvar;
  func["asum"] = make_asum();
  int asum5 = exp_function_asum->run(func, gvar, lvar);
  std::cout << "asum(5) = " << asum5 << std::endl;

  return 0;
}
