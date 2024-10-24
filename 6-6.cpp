#include <iostream>
#include "ast.h"

int main()
{
  Exp_variable *v1 = new Exp_variable("i");
  Expression *c1 = new Exp_constant(Type_INT, 0);
  Statement *s1 = new St_assign(v1, c1);

  // テスト
  std::map<std::string, Function *> func;
  std::map<std::string, int> gvar;
  std::map<std::string, int> lvar;
  std::cout << "before" << std::endl;

  // i の初期値を 5 に設定して実行
  lvar["i"] = 5;
  Return_t rd = s1->run(func, gvar, lvar);
  std::cout << "i = " << lvar["i"] << std::endl;

  return 0;
}