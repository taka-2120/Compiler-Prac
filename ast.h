//=====================================================================
//
//     ast.h: mini-C プログラムの抽象構文木 (ヘッダ)
//
//            コンパイラ実習  2004 (c) 平岡佑介, 石浦菜岐佐
//
//=====================================================================

#ifndef INCLUDE_AST_H_
#define INCLUDE_AST_H_

#include <assert.h>
#include <iostream>
#include <string>
#include <list>
#include <map>

class Function;

struct Return_t
{
  bool val_is_returned; // return 文が実行されたかどうか (true/false)
  int return_val;       // return 文が実行された場合, その返り値
  Return_t() : val_is_returned(false), return_val(0) {}
  Return_t(bool r, int v) : val_is_returned(r), return_val(v) {}
};

//---------------------------------------------------------------------
//   Type
//   型 (int か char) を表す列挙型
//---------------------------------------------------------------------
enum Type
{
  Type_INT,
  Type_CHAR
};

//---------------------------------------------------------------------
// Type_string
// Type の表示用文字列
//---------------------------------------------------------------------
std::string Type_string(Type t);

//---------------------------------------------------------------------
//  Operator
//  演算子を表す列挙型
//---------------------------------------------------------------------
enum Operator
{
  Operator_PLUS,  // +
  Operator_MINUS, // -
  Operator_MUL,   // *
  Operator_DIV,   // /
  Operator_MOD,   // %

  Operator_LT, // <
  Operator_GT, // >
  Operator_LE, // <=
  Operator_GE, // >=
  Operator_NE, // !=
  Operator_EQ, // ==
};

// static int tmp = 0;

//---------------------------------------------------------------------
//  Operator_string
//  Operator の表示用文字列
//---------------------------------------------------------------------
std::string Operator_string(Operator o);

//---------------------------------------------------------------------
//  class Expression
//  「式」の抽象基底
//---------------------------------------------------------------------
class Expression
{
private:
  Expression(const Expression &);
  Expression &operator=(const Expression &);

public:
  Expression() {}
  virtual ~Expression() {}
  virtual void print(std::ostream &os) const = 0;
  virtual int run(
      std::map<std::string, Function *> &func,
      std::map<std::string, int> &gvar,
      std::map<std::string, int> &lvar) const = 0;
};

//---------------------------------------------------------------------
//  class Exp_constant
//  式中の定数を表す
//---------------------------------------------------------------------
class Exp_constant : public Expression
{
private:
  Type type_;
  int value_;

public:
  Exp_constant(Type t, int i) : type_(t), value_(i) {}
  ~Exp_constant() {}
  const int value() const { return value_; }
  const Type type() const { return type_; }
  void print(std::ostream &os) const;
  int run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const { return value(); }
};

//---------------------------------------------------------------------
//  class Exp_variable
//---------------------------------------------------------------------
class Exp_variable : public Expression
{
private:
  std::string name_;

public:
  Exp_variable(const std::string &n) : name_(n) {}
  ~Exp_variable() {}
  const std::string &name() const { return name_; }
  void print(std::ostream &os) const;
  int run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const;
};

//---------------------------------------------------------------------
//  class Exp_operation1
//---------------------------------------------------------------------
class Exp_operation1 : public Expression
{
private:
  Operator operation_;
  Expression *operand_;

public:
  Exp_operation1(Operator op, Expression *ex) : operation_(op), operand_(ex) {}
  ~Exp_operation1() {}
  Operator operation() const { return operation_; }
  const Expression *operand() const { return operand_; }
  void print(std::ostream &os) const;
  int run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const;
};

//---------------------------------------------------------------------
//  class Exp_operation2
//---------------------------------------------------------------------
class Exp_operation2 : public Expression
{
private:
  Operator operation_;
  Expression *operand1_;
  Expression *operand2_;

public:
  Exp_operation2(Operator op, Expression *ex1, Expression *ex2) : operation_(op), operand1_(ex1), operand2_(ex2) {}
  ~Exp_operation2() {}
  Operator operation() const { return operation_; }
  const Expression *operand1() const { return operand1_; }
  const Expression *operand2() const { return operand2_; }
  void print(std::ostream &os) const;
  int run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const;
};

//---------------------------------------------------------------------
//  class Exp_function
//---------------------------------------------------------------------
class Exp_function : public Expression
{
private:
  std::string name_;
  std::list<Expression *> args_;

public:
  Exp_function(const std::string &nm, const std::list<Expression *> &args) : name_(nm), args_(args) {}
  ~Exp_function();
  const std::string &name() const { return name_; }
  const std::list<Expression *> &args() const { return args_; }
  void print(std::ostream &os) const;
  int run(std::map<std::string, Function *> &func, std::map<std::string, int> &gvar, std::map<std::string, int> &lvar) const;
};

class Statement
{
public:
  Statement() {}                                                  // コンストラクタ
  virtual ~Statement() {}                                         // デストラクタ
  virtual void print(std::ostream &os, int indent = 0) const = 0; // 表示
  virtual Return_t run(
      std::map<std::string, Function *> &func,
      std::map<std::string, int> &gvar,
      std::map<std::string, int> &lvar) const = 0;

private:
  Statement(const Statement &);            // コピーコンスタラクタは禁止
  Statement &operator=(const Statement &); // 代入演算は禁止
};

class St_assign : public Statement
{
private:
  Exp_variable *lhs_; // 左辺 (変数)
  Expression *rhs_;   // 右辺 (式)
public:
  St_assign(Exp_variable *lexp, Expression *rexp) : lhs_(lexp), rhs_(rexp) {} // コンストラクタ
  ~St_assign()
  {              // デストラクタ
    delete lhs_; // 子の抽象構文木は delete する
    delete rhs_; // 子の抽象構文木は delete する
  }
  const Exp_variable *lhs() const { return lhs_; }    // 左辺の読み出し
  const Expression *rhs() const { return rhs_; }      // 右辺の読み出し
  void print(std::ostream &os, int indent = 0) const; // 表示
  Return_t run(
      std::map<std::string, Function *> &func,
      std::map<std::string, int> &gvar,
      std::map<std::string, int> &lvar) const;
};

class St_list : public Statement
{
private:
  std::list<Statement *> statements_;

public:
  St_list(const std::list<Statement *> &li) : statements_(li) {}
  ~St_list() {}
  const std::list<Statement *> &statements() const { return statements_; }
  void print(std::ostream &os, int indent = 0) const;
  Return_t run(
      std::map<std::string, Function *> &func,
      std::map<std::string, int> &gvar,
      std::map<std::string, int> &lvar) const;
};

class St_if : public Statement
{
private:
  Expression *cond_;
  Statement *then_;
  Statement *else_;

public:
  St_if(Expression *cond, Statement *then, Statement *els) : cond_(cond), then_(then), else_(els) {}
  ~St_if() {}
  const Expression *condition() const { return cond_; }
  const Statement *then_part() const { return then_; }
  const Statement *else_part() const { return else_; }
  void print(std::ostream &os, int indent = 0) const;
  Return_t run(
      std::map<std::string, Function *> &func,
      std::map<std::string, int> &gvar,
      std::map<std::string, int> &lvar) const;
};

class St_while : public Statement
{
private:
  Expression *cond_;
  Statement *body_;

public:
  St_while(Expression *cond, Statement *body) : cond_(cond), body_(body) {}
  ~St_while() {}
  const Expression *condition() const { return cond_; }
  const Statement *body() const { return body_; }
  void print(std::ostream &os, int indent = 0) const;
  Return_t run(
      std::map<std::string, Function *> &func,
      std::map<std::string, int> &gvar,
      std::map<std::string, int> &lvar) const;
};

class St_return : public Statement
{
private:
  Expression *value_;

public:
  St_return(Expression *value) : value_(value) {}
  ~St_return() {}
  const Expression *value() const { return value_; }
  Return_t run(
      std::map<std::string, Function *> &func,
      std::map<std::string, int> &gvar,
      std::map<std::string, int> &lvar) const;
  void print(std::ostream &os, int indent = 0) const;
};

class St_function : public Statement
{
private:
  Exp_function function_;

public:
  St_function(const std::string &name, const std::list<Expression *> &args) : function_(name, args) {}
  ~St_function() {}
  const std::string &name() const { return function_.name(); }
  const std::list<Expression *> &args() const { return function_.args(); }
  void print(std::ostream &os, int indent = 0) const;
  Return_t run(
      std::map<std::string, Function *> &func,
      std::map<std::string, int> &gvar,
      std::map<std::string, int> &lvar) const;
};

class Variable
{
private:
  Type type_;
  std::string name_;

public:
  Variable(Type t, const std::string &n) : type_(t), name_(n) {}
  ~Variable() {}
  const Type type() const { return type_; }
  const std::string &name() const { return name_; }
  void print(std::ostream &os) const;
};

class Function
{
private:
  Type type_;
  std::string name_;
  std::list<Variable *> args_;
  std::list<Variable *> lvarlist_;
  Statement *body_;

public:
  Function(Type t, const std::string &n, const std::list<Variable *> &args, const std::list<Variable *> &lvarlist, Statement *b) : type_(t), name_(n), args_(args), lvarlist_(lvarlist), body_(b) {}
  ~Function() {}
  const Type type() const { return type_; }
  const std::string &name() const { return name_; }
  const std::list<Variable *> &args() const { return args_; }
  const std::list<Variable *> &lvarlist() const { return lvarlist_; }
  const Statement *body() const { return body_; }
  void print(std::ostream &os) const;
  int run(
      std::map<std::string, Function *> &func,
      std::map<std::string, int> &gvar,
      std::list<int> &i_args) const;
};

class Program
{
private:
  std::list<Variable *> lvarlist_;
  std::list<Function *> funclist_;
  Function *main_;

public:
  Program(const std::list<Variable *> &lvarlist, const std::list<Function *> &funclist, Function *main) : lvarlist_(lvarlist), funclist_(funclist), main_(main) {}
  ~Program() {}
  const std::list<Variable *> &lvarlist() const { return lvarlist_; }
  const std::list<Function *> &funclist() const { return funclist_; }
  const Function *main() const { return main_; }
  void print(std::ostream &os) const;
  int run();
};

#endif // ifndef INCLUDE_AST_H_
