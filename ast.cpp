//=====================================================================
//
//     ast.cpp: mini-C プログラムの抽象構文木 (実装)
//
//            コンパイラ実習  2004 (c) 平岡佑介, 石浦菜岐佐
//
//=====================================================================

#include "ast.h"

//---------------------------------------------------------------------
//   Type_string の実装
//---------------------------------------------------------------------
std::string Type_string(Type t)
{
  switch (t)
  {
  case Type_INT:
    return "int";
  case Type_CHAR:
    return "char";
  default:
    return "UNDEF";
  }
}

//---------------------------------------------------------------------
//   Opeartor_string の実装
//---------------------------------------------------------------------
std::string Operator_string(Operator o)
{
  switch (o)
  {
  case Operator_PLUS:
    return "+";
  case Operator_MINUS:
    return "-";
  case Operator_MUL:
    return "*";
  case Operator_DIV:
    return "/";
  case Operator_MOD:
    return "%";
  case Operator_LT:
    return "<";
  case Operator_GT:
    return ">";
  case Operator_LE:
    return "<=";
  case Operator_GE:
    return ">=";
  case Operator_NE:
    return "!=";
  case Operator_EQ:
    return "==";
  default:
    return "???";
  }
}

static int tmp = 0;

//---------------------------------------------------------------------
//  tab(int)
//    インデントを行うための関数 (ast.cpp の中だけで用いる)
//    indent で指定された段数の 2 倍のスペースを返す
//---------------------------------------------------------------------
static std::string tab(int indent)
{
  std::string tab_ = "";
  for (int i = 0; i < indent; i++)
  {
    tab_ += "  ";
  }
  return tab_;
}

//---------------------------------------------------------------------
//   Exp_constant::print の実装
//---------------------------------------------------------------------
void Exp_constant::print(std::ostream &os) const
{
  switch (type())
  {
  case Type_INT:
    os << value();
    break;
  case Type_CHAR:
  {
    os << '\'';
    if (value() == '\n')
    {
      os << '\\';
      os << 'n';
    }
    else if (value() == '\t')
    {
      os << '\\';
      os << 't';
    }
    else if (value() == '\\')
    {
      os << '\\';
      os << '\\';
    }
    else
    {
      os << (char)value();
    }
    os << '\'';
    break;
  }
  default:
    assert(0);
  }
}

//---------------------------------------------------------------------
//   Exp_variable::print の実装
//---------------------------------------------------------------------
void Exp_variable::print(std::ostream &os) const
{
  os << name();
}

int Exp_variable::run(
    std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const
{
  std::map<std::string, int>::const_iterator p;
  if ((p = lvar.find(name())) != lvar.end())
  {
    return p->second;
  }
  if ((p = gvar.find(name())) != gvar.end())
  {
    return p->second;
  }
  std::cerr << "undefined variable " << name() << std::endl;
  exit(1);
}

//---------------------------------------------------------------------
//   Exp_operation1::print の実装
//---------------------------------------------------------------------
void Exp_operation1::print(std::ostream &os) const
{
  os << Operator_string(operation());
  if (operand())
  {
    operand()->print(os);
  }
  else
  {
    os << "UNDEF";
  }
}

int Exp_operation1::run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const
{
  assert(operand() != NULL);
  int value = operand()->run(func, gvar, lvar);

  if (operation() == Operator_MINUS)
  {
    return -value;
  }
  return value;
}

//---------------------------------------------------------------------
//   Exp_operation2::print の実装
//---------------------------------------------------------------------
void Exp_operation2::print(std::ostream &os) const
{
  if (operand1())
  {
    operand1()->print(os);
  }
  else
  {
    os << "UNDEF";
  }
  os << " " << Operator_string(operation()) << " ";
  if (operand2())
  {
    operand2()->print(os);
  }
  else
  {
    os << "UNDEF";
  }
}

int Exp_operation2::run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const
{
  assert(operand1() != NULL);
  assert(operand2() != NULL);
  int value1 = operand1()->run(func, gvar, lvar);
  int value2 = operand2()->run(func, gvar, lvar);

  switch (operation())
  {
  case Operator_PLUS:
    return value1 + value2;
  case Operator_MINUS:
    return value1 - value2;
  case Operator_MUL:
    return value1 * value2;
  case Operator_DIV:
    return value1 / value2;
  case Operator_MOD:
    return value1 % value2;
  case Operator_LT:
    return value1 < value2;
  case Operator_GT:
    return value1 > value2;
  case Operator_LE:
    return value1 <= value2;
  case Operator_GE:
    return value1 >= value2;
  default:
    std::cerr << "unknown operator" << std::endl;
    exit(1);
  }
}

//---------------------------------------------------------------------
//   Exp_function
//---------------------------------------------------------------------
Exp_function::~Exp_function()
{
  for (std::list<Expression *>::const_iterator it = args().begin(); it != args().end(); it++)
  {
    delete *it;
  }
}

void Exp_function::print(std::ostream &os) const
{
  os << name();
  os << "(";
  for (std::list<Expression *>::const_iterator it = args().begin(); it != args().end(); it++)
  {
    (*it)->print(os);
    if (it != --args_.end())
    {
      os << ", ";
    }
  }
  os << ")";
}

int Exp_function::run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const
{
  std::list<int> i_args;
  for (std::list<Expression *>::const_iterator it = args().begin(); it != args().end(); it++)
  {
    i_args.push_back((*it)->run(func, gvar, lvar));
  }

  if (name() == "getint")
  {
    int value;
    std::cin >> value;
    return value;
  }
  else if (name() == "getchar")
  {
    char value;
    std::cin >> value;
    return value;
  }
  else if (name() == "putint")
  {
    std::cout << i_args.front();
    return 0;
  }
  else if (name() == "putchar")
  {
    std::cout << (char)i_args.front();
    return 0;
  }
  else
  {
    std::map<std::string, Function *>::const_iterator p;
    if ((p = func.find(name())) != func.end())
    {
      Function *f = p->second;
      return f->run(func, gvar, i_args);
    }
    else
    {
      std::cerr << "undefined function " << name() << std::endl;
      exit(1);
    }
  }
}

//---------------------------------------------------------------------
//   St_assign
//---------------------------------------------------------------------
void St_assign::print(std::ostream &os, int indent) const
{
  os << tab(indent); // インデント (1 段につきスペース 2 個) をつける
  if (lhs())
  {                   // 左辺が NULL でなければ
    lhs()->print(os); // 左辺を表示
  }
  else
  {
    os << "UNDEF"; // 左辺が NULL なら取り敢えず UNDEF と表示
  }
  os << " = ";
  if (rhs())
  {                   // 右辺が NULL でなければ
    rhs()->print(os); // 右辺を表示
  }
  else
  {
    os << "UNDEF";
  }
  os << ";" << std::endl;
}

Return_t St_assign::run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const
{
  assert(lhs() != NULL);
  assert(rhs() != NULL);
  int rhs_value = rhs()->run(func, gvar, lvar);

  std::map<std::string, int>::const_iterator p;
  if ((p = lvar.find(lhs()->name())) != lvar.end())
  {
    lvar[lhs()->name()] = rhs_value;
  }
  else if ((p = gvar.find(lhs()->name())) != gvar.end())
  {
    gvar[lhs()->name()] = rhs_value;
  }
  else
  {
    std::cerr << "undefined variable " << lhs()->name() << std::endl;
  }
  return Return_t(false, 0);
}

//---------------------------------------------------------------------
//   St_list
//---------------------------------------------------------------------
void St_list::print(std::ostream &os, int indent) const
{
  for (std::list<Statement *>::const_iterator it = statements_.begin(); it != statements_.end(); it++)
  {
    (*it)->print(os, indent);
  }
}

Return_t St_list::run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const
{
  for (std::list<Statement *>::const_iterator it = statements().begin();
       it != statements().end(); it++)
  {
    assert(*it);                                // 念のため NULL でないことを確認
    Return_t rd = (*it)->run(func, gvar, lvar); // 実行
    if (rd.val_is_returned)
      return rd; // return 文が実行されていたら, rd をそのまま返す
  }
  return Return_t(false, 0);
}

//---------------------------------------------------------------------
//   St_if
//---------------------------------------------------------------------
void St_if::print(std::ostream &os, int indent) const
{
  os << tab(indent) << "if (";
  condition()->print(os);
  os << ") {" << std::endl;

  then_part()->print(os, indent + 1);

  if (else_part() != NULL)
  {
    os << tab(indent) << "} else {" << std::endl;
    else_part()->print(os, indent + 1);
  }
  os << tab(indent) << "}" << std::endl;
}

Return_t St_if::run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const
{
  assert(condition() != NULL);

  int cond = condition()->run(func, gvar, lvar);
  if (cond && then_part() != NULL)
  {
    Return_t rd = then_part()->run(func, gvar, lvar);
    if (rd.val_is_returned)
      return rd;
  }
  else if (!cond && else_part() != NULL)
  {
    Return_t rd = else_part()->run(func, gvar, lvar);
    if (rd.val_is_returned)
      return rd;
  }

  return Return_t(false, 0);
}

//---------------------------------------------------------------------
//   St_while
//---------------------------------------------------------------------
void St_while::print(std::ostream &os, int indent) const
{
  os << tab(indent) << "while (";
  condition()->print(os);
  os << ") {" << std::endl;

  body()->print(os, indent + 1);

  os << tab(indent) << "}" << std::endl;
}

Return_t St_while::run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const
{
  assert(condition() != NULL);

  while (condition()->run(func, gvar, lvar))
  {
    Return_t rd = body()->run(func, gvar, lvar);
    if (rd.val_is_returned)
      return rd;
  }

  return Return_t(false, 0);
}

//---------------------------------------------------------------------
//   St_return
//---------------------------------------------------------------------
void St_return::print(std::ostream &os, int indent) const
{
  os << tab(indent) << "return ";
  value()->print(os);
  os << ";" << std::endl;
}

Return_t St_return::run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const
{
  assert(value());
  int returnValue = value()->run(func, gvar, lvar);
  return Return_t(true, returnValue);
}

//---------------------------------------------------------------------
//   St_function
//---------------------------------------------------------------------
void St_function::print(std::ostream &os, int indent) const
{
  os << tab(indent) << name() << "(";
  for (std::list<Expression *>::const_iterator it = args().begin(); it != args().end(); it++)
  {
    (*it)->print(os);
    if (it != --args().end())
    {
      os << ", ";
    }
  }
  os << ");" << std::endl;
}

Return_t St_function::run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const
{
  function_.run(func, gvar, lvar);
  return Return_t(false, 0);
}

//---------------------------------------------------------------------
//   Variable
//---------------------------------------------------------------------
void Variable::print(std::ostream &os) const
{
  os << Type_string(type()) << " " << name();
}

//---------------------------------------------------------------------
//   Function
//---------------------------------------------------------------------
void Function::print(std::ostream &os) const
{
  os << Type_string(type()) << " " << name() << "(";
  for (std::list<Variable *>::const_iterator it = args().begin(); it != args().end(); it++)
  {
    (*it)->print(os);
    if (it != --args().end())
    {
      os << ", ";
    }
  }
  os << ") {" << std::endl;
  // ローカル変数の表示
  for (std::list<Variable *>::const_iterator it = lvarlist().begin(); it != lvarlist().end(); it++)
  {
    os << tab(1);
    (*it)->print(os);
    os << ";" << std::endl;
  }
  os << std::endl;
  body()->print(os, 1);
  os << "}" << std::endl;
}

int Function::run(
    std::map<std::string, Function *> &func,
    std::map<std::string, int> &gvar,
    std::list<int> &i_args) const
{
  return i_args.front();
}

//---------------------------------------------------------------------
//   Program
//---------------------------------------------------------------------
void Program::print(std::ostream &os) const
{
  // ローカル変数の表示
  for (std::list<Variable *>::const_iterator it = lvarlist().begin(); it != lvarlist().end(); it++)
  {
    (*it)->print(os);
    os << ";" << std::endl;
  }
  os << std::endl;

  for (std::list<Function *>::const_iterator it = funclist().begin(); it != funclist().end(); it++)
  {
    (*it)->print(os);
    os << std::endl;
  }

  main()->print(os);
}