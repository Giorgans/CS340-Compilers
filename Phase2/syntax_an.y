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
#include "SymbolTable.h"
#define YY_DECL int alpha_yylex (void* yylval)
using namespace std;

int yyerror(const char *yaccProvidedMessage);
int yylex(void);
extern int yylineno;
extern char *yytext; 
extern FILE *yyin;

SymbolTable table;
unsigned int scope=0;
bool func_open=false;

%}

%start program
%error-verbose
 
%expect 1

%union {
    char *stringValue; 
    int intValue; 
    double realValue;
}

%output="syntax_analyzer.cpp"


%token <stringValue> ID
%token <intValue> INTCONST
%token <realValue> REALCONST
%token <stringValue> STRING
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL
%token MOD GREATER_THAN GREATER_EQUAL LESS_THAN LESS_EQUAL PLUS_PLUS PLUS MINUS MULTIPLY DIV  MINUS_MINUS ASSIGN EQUAL NOT_EQUAL 
%token LEFT_BRACE RIGHT_BRACE LEFT_PARENTHESIS RIGHT_PARENTHESIS LEFT_BRACKET RIGHT_BRACKET SEMICOLON COLON DOUBLE_COLON DOT DOUBLE_DOT COMMA
%type <entry> lvalue

%right ASSIGN
%left OR
%left AND
%nonassoc EQUAL NOT_EQUAL
%nonassoc GREATER_THAN GREATER_EQUAL LESS_THAN LESS_EQUAL
%left  PLUS MINUS
%left  MULTIPLY DIV MOD
%right NOT PLUS_PLUS MINUS_MINUS UMINUS
%left DOT DOUBLE_DOT
%left LEFT_BRACKET RIGHT_BRACKET
%left LEFT_PARENTHESIS RIGHT_PARENTHESIS

%%

program: stmts {}
        ;
stmt:  exp SEMICOLON {}
      | if_stmt {}
      | while_stmt {}
      | for_stmt {}
      | ret_stmt {}
      | BREAK SEMICOLON {}
      | CONTINUE SEMICOLON {}
      | block {}
      | f_def {}
      | SEMICOLON {}
      ;
stmts: stmts stmt {}
    | {}
    ;
exp: assign_exp {}
    | exp PLUS exp {}
    | exp MINUS exp {}
    | exp MULTIPLY exp {}
    | exp DIV exp {}
    | exp MOD exp {}
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
    | PLUS_PLUS lvalue {}
    | lvalue PLUS_PLUS {}
    | MINUS_MINUS lvalue {}
    | lvalue MINUS_MINUS {}
    | primary {}
    ;
assign_exp: lvalue ASSIGN exp {}
    ;
primary: lvalue {}
    | call {}
    | obj_def {}
    | LEFT_PARENTHESIS f_def RIGHT_PARENTHESIS {}
    | const {}
    ;
lvalue: ID {
            SymbolTableEntry *symbol = table.Lookup($1),*temp;
            bool found = false;

            if(symbol!=NULL){
                for (int tempscope=scope; tempscope>=0 ; --tempscope){
                    temp = table.LookupScope($1,tempscope);
                    if(temp!=NULL && temp->IsActive()) found = true;
                }
                if(found) break;
            }
            if(!scope)
                table.Insert($1,GLOBAL,scope,yylineno);
            else if(found && func_open && temp->getType()!=USERFUNC)
                cout << "ERROR! Cannot access" <<  $1  <<  "in line" << yylineno << endl;
            else
                table.Insert($1,LOCALV,scope,yylineno);
}
    |   LOCAL ID {

            
    }
    |   DOUBLE_COLON ID {}
    |   member {}
    ;
member:  lvalue DOT ID {}
    | lvalue LEFT_BRACKET exp RIGHT_BRACKET {}
    | call DOT ID {}
    | call LEFT_BRACKET exp RIGHT_BRACKET {}
    ;
call: call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
    | lvalue callsuffix {}
    | LEFT_PARENTHESIS f_def RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
    ; 
   
callsuffix: normcall {}
    | methodcall {}
    ;
normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
    ;
methodcall: DOUBLE_DOT ID LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
    ;

elist: exp {}
    |elist COMMA exp {}
    |{}
    ;

obj_def:  LEFT_BRACKET indexed RIGHT_BRACKET {}
    | LEFT_BRACKET elist RIGHT_BRACKET {}
    ;
indexed: index_el {} 
    |indexed COMMA index_el {}
    ;
index_el: LEFT_BRACE exp COLON exp RIGHT_BRACE {}
    ;
block: LEFT_BRACE {} stmts RIGHT_BRACE {}
    ;

f_def: FUNCTION ID{
            SymbolTableEntry *symbol = table.Lookup($2);
            SymbolTableEntry *temp = table.LookupScope($2,0);
            bool found = false;

            if(symbol!=NULL){

            }
            
    
       }
        LEFT_PARENTHESIS {scope++;} 
        idlist RIGHT_PARENTHESIS {scope--;} {func_open=true;} 
        block {} {func_open=false;}
    |  FUNCTION LEFT_PARENTHESIS {scope++;} idlist RIGHT_PARENTHESIS {scope--;}{func_open=true;}  block {}  {func_open=false;}
    ;
const: INTCONST {}
    |  REALCONST {}
    |  STRING {}
    |  NIL {}
    |  TRUE {}
    |  FALSE {}
    ;
idlist: ID{}
    | idlist COMMA ID {}
    | {}
    ;
if_stmt: IF LEFT_PARENTHESIS exp RIGHT_PARENTHESIS stmt
    | if_stmt ELSE stmt{}
    ;
while_stmt: WHILE LEFT_PARENTHESIS exp RIGHT_PARENTHESIS stmt {}
    ;
for_stmt: FOR LEFT_PARENTHESIS elist SEMICOLON exp SEMICOLON elist RIGHT_PARENTHESIS stmt {}
    ;
ret_stmt: RETURN SEMICOLON {}
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
