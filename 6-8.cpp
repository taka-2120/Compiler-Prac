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

int main()
{
  Statement *s = make_if(); // 前回の演習で作成したもの
  std::map<std::string, Function *> func;
  std::map<std::string, int> gvar;
  std::map<std::string, int> lvar;
  // 真の場合
  lvar["i"] = -5;
  lvar["s"] = 10;
  Return_t rd1 = s->run(func, gvar, lvar);
  std::cout << "s = " << lvar["s"] << std::endl;
  // 偽の場合
  lvar["i"] = 7;
  lvar["s"] = 10;
  Return_t rd0 = s->run(func, gvar, lvar);
  std::cout << "s = " << lvar["s"] << std::endl;

  return 0;
}