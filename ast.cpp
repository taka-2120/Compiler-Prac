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

//---------------------------------------------------------------------
//   St_return
//---------------------------------------------------------------------
void St_return::print(std::ostream &os, int indent) const
{
  os << tab(indent) << "return ";
  value()->print(os);
  os << ";" << std::endl;
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