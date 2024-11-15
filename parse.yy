%{
//=====================================================================
//
//     parse.yy: mini-C プログラムの構文解析系生成用記述
//
//            コンパイラ演習  2004 (c) 平岡佑介, 石浦菜岐佐
//
//=====================================================================

// --------------------------------------------------------------------
// [Part-1] ヘッダやグローバル変数, 関数の宣言
// --------------------------------------------------------------------

#include "ast.h"
#include <cstdio>

extern int linenum;
extern std::FILE* yyin;

int yylex(void);
void yyerror(const char*);

%}

// --------------------------------------------------------------------
// [Part-2] 構文木の各節点が返す値を格納する union
// --------------------------------------------------------------------

%union{
  char* string;
  int val;
  Expression* expression;
  Exp_variable* exp_variable;
  std::list<Expression*>* explist;
}

// --------------------------------------------------------------------
// [Part-3] トークンの宣言
// --------------------------------------------------------------------

%token lex_KW_INT
%token lex_KW_CHAR
%token lex_KW_IF
%token lex_KW_ELSE
%token lex_KW_WHILE
%token lex_KW_RETURN

%token lex_PLUS lex_MINUS lex_STAR lex_SLASH lex_PERCENT
%token lex_AND
%token lex_EQ lex_EQEQ lex_NE lex_LT lex_GT lex_LE lex_GE
%token lex_COMMA lex_SEMICOLON
%token lex_LPAREN lex_RPAREN
%token lex_LBRACE lex_RBRACE
%token lex_LBRACK lex_RBRACK

%token <val> lex_INT lex_CHAR
%token <string> lex_ID

// --------------------------------------------------------------------
// [Part-4] 型の宣言
// --------------------------------------------------------------------

%type <expression> expression4
%type <expression> expression3
%type <expression> expression2
%type <expression> expression
%type <exp_variable> exp_variable
%type <expression> exp_function
%type <explist> explist

// --------------------------------------------------------------------
// [Part-5] 開始記号の宣言
// --------------------------------------------------------------------

%start program

%%
// --------------------------------------------------------------------
// [Part-6] 文法とアクションの記述
// --------------------------------------------------------------------

program
:  expression
{
  $1->print(std::cout); std::cout<<std::endl;
}

expression
: expression2
{
  $$ = $1
}
| expression lex_LT expression2
{
  $$ = new Exp_operation2(Operator_LT, $1, $3);
}
| expression lex_GT expression2
{
  $$ = new Exp_operation2(Operator_GT, $1, $3);
}
| expression lex_LE expression2
{
  $$ = new Exp_operation2(Operator_LE, $1, $3);
}
| expression lex_GE expression2
{
  $$ = new Exp_operation2(Operator_GE, $1, $3);
}
| expression lex_EQEQ expression2
{
  $$ = new Exp_operation2(Operator_EQ, $1, $3);
}
| expression lex_NE expression2
{
  $$ = new Exp_operation2(Operator_NE, $1, $3);
}

expression2
: expression3
{
  $$ = $1;
}
| lex_PLUS expression3
{
  $$ = new Exp_operation1(Operator_PLUS, $2);
}
| lex_MINUS expression3
{
  $$ = new Exp_operation1(Operator_MINUS, $2);
}
| expression2 lex_PLUS expression3
{
  $$ = new Exp_operation2(Operator_PLUS, $1, $3);
}
| expression2 lex_MINUS expression3
{
  $$ = new Exp_operation2(Operator_MINUS, $1, $3);
}

expression3
: expression4
{
  $$ = $1;
}
| expression3 lex_STAR expression4
{
  $$ = new Exp_operation2(Operator_MUL, $1, $3);
}
| expression3 lex_SLASH expression4
{
  $$ = new Exp_operation2(Operator_DIV, $1, $3);
}
| expression3 lex_PERCENT expression4
{
  $$ = new Exp_operation2(Operator_MOD, $1, $3);
}

expression4
: lex_INT
{
  $$ = new Exp_constant(Type_INT, $1);
}
| lex_CHAR
{
  $$ = new Exp_constant(Type_CHAR, $1);
}
| exp_variable
{
  $$ = $1;
}
| lex_LPAREN expression lex_RPAREN
{
  $$ = $2;
}
| exp_function
{
  $$ = $1;
}

exp_function
: lex_ID lex_LPAREN explist lex_RPAREN
{
  $$ = new Exp_function($1, *$3);
  delete $3;
}

explist
:
{
  $$ = new std::list<Expression*>;
}
| expression
{
  $$ = new std::list<Expression*>;
  $$->push_back($1);
}
| explist lex_COMMA expression
{
  $1->push_back($3);
  $$ = $1;
}

exp_variable
: lex_ID
{
  $$ = new Exp_variable($1);
}

%%
// --------------------------------------------------------------------
// [Part-7] 関数本体の記述
// --------------------------------------------------------------------

int main(int argc, char *argv[])
{
  if(std::FILE* fp = std::fopen(argv[1], "r"))
  {
    linenum = 1;  // lex の行番号を 1 に初期設定
    yyin = fp;    // lex のファイルポインタをセット
    yyparse();    // 構文解析関数を呼び出す
  }
  else {
    printf("ファイル '%s' が開けません\n", argv[1]);
  }
}

