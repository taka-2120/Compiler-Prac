#include <iostream>
#include "ast.h"
using namespace std;

int main(void)
{
  Expression *c1 = new Exp_constant(Type_INT, 7);
  Expression *c2 = new Exp_constant(Type_CHAR, 'x');

  // 実行とその結果の表示
  std::map<std::string, Function *> func;
  std::map<std::string, int> gvar;
  std::map<std::string, int> lvar;
  std::cout << c1->run(func, gvar, lvar);
  std::cout << std::endl;
  std::cout << c2->run(func, gvar, lvar);
  std::cout << std::endl;
  return 0;
}