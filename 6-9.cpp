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

int main()
{
  Statement *s = make_while(); // 前回の演習で作成したもの
  std::map<std::string, Function *> func;
  std::map<std::string, int> gvar;
  std::map<std::string, int> lvar;
  lvar["i"] = -3;
  lvar["n"] = 3;
  lvar["s"] = 0;
  Return_t rd = s->run(func, gvar, lvar);
  std::cout << "i = " << lvar["i"] << std::endl;
  std::cout << "n = " << lvar["n"] << std::endl;
  std::cout << "s = " << lvar["s"] << std::endl;

  return 0;
}