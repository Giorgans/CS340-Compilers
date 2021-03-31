%{
/*** ---SYNTAX ANALYSIS---
     Georgios Zervos AM:3384;
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 ***/
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <list>
#include <stack>
#include "parser.h"
#define YY_DECL int alpha_yylex (void* yylval)
using namespace std;

int yyerror(const char *yaccProvidedMessage);
int yylex(void);

extern int yylineno;
extern char *yytext; 
extern FILE *yyin;

%}
%start program
%error-verbose

%expect 0

%union {
    string stringValue; 
    int intValue; 
    double realValue;
}
%token <stringValue> ID
%token <intValue> INTCONST
%token <realValue> REALCONST
%token <stringValue> STRING
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL
%token MODULO GREATER_THAN GREATER_EQUAL LESS_THAN LESS_EQUAL INCREMENT PLUS MINUS MULTIPLY DIVISION  DECREMENT ASSIGN EQUAL NOT_EQUAL 
%token LEFT_BRACE RIGHT_BRACE LEFT_PARENTHESIS RIGHT_PARENTHESIS LEFT_BRACKET RIGHT_BRACKET SEMICOLON COLON DOUBLE_COLON DOT DOUBLE_DOT COMMA
%type <entry> lvalue

%right ASSIGN
%left OR
%left AND
%nonassoc EQUAL NOT_EQUAL
%nonassoc GREATER_THAN GREATER_EQUAL LESS_THAN LESS_EQUAL
%left  PLUS MINUS
%left  MULTIPLY DIVISION MODULO
%right NOT INCREMENT DECREMENT UMINUS
%left DOT DOUBLE_DOT
%left LEFT_BRACKET RIGHT_BRACKET
%left LEFT_PARENTHESIS RIGHT_PARENTHESIS

%%
program: program stat {}
         | 
         ;
stat: exp SEMICOLON {}
      | if_stat {}
      | while_stat {}
      | for_stat {}
      | ret_stat {}
      | BREAK SEMICOLON {}
      | CONTINUE SEMICOLON {}
      | f_def {}
      | SEMICOLON
      ;
exp: ass_exp {}
    | exp PLUS exp {}
    | exp MINUS exp {}
    | exp MULTIPLY exp {}
    | exp DIVISION exp {}
    | exp MODULO exp {}
    | exp LESS_THAN exp {}
    | exp GREATER_THAN exp {}
    | exp LESS_EQUAL exp {}
    | exp GREATER_EQUAL exp {}
    | exp AND exp {}
    | exp OR exp {}
    | exp EQUAL exp {}
    | exp NOT_EQUAL exp {}
    | term {}
    ;
term: LEFT_PARENTHESIS exp RIGHT_PARENTHESIS {}
    | MINUS exp %prec UMINUS {}
    | NOT exp {}
    | INCREMENT lvalue {}
    | lvalue INCREMENT {}
    | DECREMENT lvalue {}
    | prim {}
    ;
ass_exp: lvalue ASSIGN exp {}
    ;
prim: lvalue {}
    | call {}
    | obj_def {}
    | LEFT_PARENTHESIS f_def RIGHT_PARENTHESIS {}
    | const {}
    ;
lvalue: ID {}
    |   LOCAL ID {}
    |   DOUBLE_COLON ID {}
    |   mem {}
    ;
mem:  lvalue DOT ID {}
    | lvalue LEFT_BRACKET exp RIGHT_BRACKET {}
    | call DOT ID {}
    | call LEFT_BRACKET exp RIGHT_BRACKET {}
    ;
call: call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
    | lvalue callsuffix {}
    | LEFT_PARENTHESIS f_def RIGHT_PARENTHESIS {}
    | LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
    ;
callsuffix: normcall {}
    | methodcall {}
    ;
normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
    ;
methodcall: DOUBLE_DOT ID LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
    ;
elist: exp {}
    |  elist COMMA exp {}
    | {}
    ;
obj_def: LEFT_BRACKET elist RIGHT_BRACKET {}
    | LEFT_BRACKET indexed RIGHT_BRACKET {}
    ;
indexed: index_el {}
    | indexed COMMA index_el {}
    ;
index_el: LEFT_BRACE exp COLON exp RIGHT_BRACE {}
    ;
block: LEFT_BRACE {} stats RIGHT_BRACE {}
    ;
stats: stats stat {} 
    | {}
    ;
f_def: FUNCTION ID {} LEFT_PARENTHESIS {} idlist {} RIGHT_PARENTHESIS {}
    |  FUNCTION {} LEFT_PARENTHESIS {} idlist {} RIGHT_PARENTHESIS
    ;
const: INTCONST {}
    |  REALCONST {}
    |  STRING {}
    |  NIL {}
    |  TRUE {}
    |  FALSE {}
    ;
idlist: ID{}
    | ID COMMA ID {}
    | {}
    ;
if_stat: IF LEFT_PARENTHESIS exp RIGHT_PARENTHESIS stat {}
    | IF LEFT_PARENTHESIS exp RIGHT_PARENTHESIS stat ELSE stat {}
    ;
while_stat: WHILE LEFT_PARENTHESIS exp RIGHT_PARENTHESIS stat {}
    ;
for_stat: FOR LEFT_PARENTHESIS elist SEMICOLON exp SEMICOLON elist RIGHT_PARENTHESIS stat {}
    ;
ret_stat: RETURN SEMICOLON {}
    | RETURN exp SEMICOLON {}
    ;

%%


int yyerror(const char* yaccProvidedMessage){
	fprintf(stderr, "%s: at line %d before token: %s\n", yaccProvidedMessage, yylineno, yytext);
	fprintf(stderr, "INPUT NOT VALID\n");
  return 0;
}

int main(int argc, char **argv) { 
  if(argc>1){
    if(!(yyin = fopen(argv[1],"r"))){
      fprintf(stderr, "Cannot read file %s\n",argv[1]);
      return 1;
    }
  }
  else yyin = stdin;    

  yyparse();
    
  return 0; 
}
