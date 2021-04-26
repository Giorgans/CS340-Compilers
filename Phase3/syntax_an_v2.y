%{
/*** --- SYNTAX ANALYSIS YACC/BISON FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.2.3 , x86 chip***/
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
unsigned int scope=0,loop_open=0,func_open=0,func_id_num=0;
bool isMember=false;

%}

%start program 
%error-verbose
 
%expect 0

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
%type <stringValue> lvalue
/*Tokens for keywords*/
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL
/*Tokens for operators*/
%token ASSIGN PLUS MINUS MULTIPLY DIV MOD PLUS_PLUS MINUS_MINUS LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL EQUAL NOT_EQUAL 
/*Tokens for punctuators*/
%token L_BRACE R_BRACE L_PARENTHESIS R_PARENTHESIS L_BRACKET R_BRACKET SEMICOLON COMMA COLON DOUBLE_COLON DOT DOUBLE_DOT 

%left L_PARENTHESIS R_PARENTHESIS
%left L_BRACKET R_BRACKET
%left DOT DOUBLE_DOT
%right NOT PLUS_PLUS MINUS_MINUS UMINUS
%left  MULTIPLY DIV MOD
%left  PLUS MINUS
%nonassoc GREATER_THAN GREATER_EQUAL LESS_THAN LESS_EQUAL
%nonassoc EQUAL NOT_EQUAL
%left AND
%left OR
%right ASSIGN
%nonassoc ELSE

%%

program: stmts ;

stmts: stmts stmt | ;

stmt:  exp SEMICOLON 
      |  if_stmt | {loop_open++;} while_stmt {loop_open--;} | {loop_open++;} for_stmt {loop_open--;} | ret_stmt 
      | BREAK SEMICOLON {if(!loop_open) cout << "[Syntax Analysis] ERROR: Cannot use BREAK statement while not within loop, in line " << yylineno << endl;}
      | CONTINUE SEMICOLON {if(!loop_open) cout << "[Syntax Analysis] ERROR: Cannot use CONTINUE statement while not within loop, in line " << yylineno << endl;}
      | block | f_def | SEMICOLON ;

exp: assign_exp | exp PLUS exp | exp MINUS exp | exp MULTIPLY exp | exp DIV exp | exp MOD exp 
    | exp LESS_THAN exp | exp GREATER_THAN exp | exp LESS_EQUAL exp | exp GREATER_EQUAL exp 
    | exp AND exp | exp OR exp | exp EQUAL exp | exp NOT_EQUAL exp  | term ;


term: L_PARENTHESIS exp R_PARENTHESIS | MINUS exp %prec UMINUS | NOT exp 
    | PLUS_PLUS lvalue {
        SymbolTableEntry *symbol=table.Lookup($2);
        if(symbol->getType()==USERFUNC || symbol->getType()==LIBFUNC)
            cout <<"[Syntax Analysis] ERROR: Trying to left increase the function \"" << $2 << "\" by 1, in line " <<  yylineno << endl;}
    | lvalue PLUS_PLUS {
        SymbolTableEntry *symbol=table.Lookup($1);
        if(symbol->getType()==USERFUNC || symbol->getType()==LIBFUNC)
            cout <<"[Syntax Analysis] ERROR: Trying to right increase the function \"" << $1 << "\" by 1, in line " <<yylineno << endl;}
    | MINUS_MINUS lvalue {
        SymbolTableEntry *symbol=table.Lookup($2);
        if(symbol->getType()==USERFUNC || symbol->getType()==LIBFUNC)
            cout <<"[Syntax Analysis] ERROR: Trying to left decrease the function \"" << $2 << "\" by 1, in line" << yylineno << endl;}
    | lvalue MINUS_MINUS {
        SymbolTableEntry *symbol=table.Lookup($1);
        if(symbol->getType()==USERFUNC || symbol->getType()==LIBFUNC)
            cout <<"[Syntax Analysis] ERROR: Trying to right decrease the function \"" << $1 << "\" by 1, in line " << yylineno << endl;}
    | primary  ;

assign_exp: lvalue ASSIGN exp {
    SymbolTableEntry *symbol;
        symbol=table.LookupScope($1,0);
        if(symbol!=NULL && !isMember && symbol->getType()==LIBFUNC)
            cout << "[Syntax Analysis] ERROR: Trying to assign a value to \"" << $1 << "\" library function, in line "<< yylineno << endl;
        for(int i=scope ; i>=0 ; i--){
            symbol=table.LookupScope($1,i);
            if(symbol!=NULL && !isMember && scope==i && symbol->getType()==USERFUNC)
                cout << "[Syntax Analysis] ERROR: Trying to assign a value to \"" << $1 << "\" user function, in line "<< yylineno << endl;
        }  
        if(isMember) isMember=false;
};

primary: lvalue | call | obj_def | L_PARENTHESIS f_def R_PARENTHESIS | const ;

lvalue: ID {
            SymbolTableEntry *symbol,*temp;
            unsigned int temp_scope;
            bool found = false;
            symbol = table.LookupScope($1,scope);
           
            for(int i=scope ; i>=0 ; i--){
                temp=table.LookupScope($1,i);
                if(temp!=NULL && temp->IsActive() ){
                    found=true;
                    temp_scope=i;
                    break;
                }
            }

            if(!found){
                if(!scope) table.Insert($1,GLOBAL,scope,yylineno);
                else table.Insert($1,LOCALV,scope,yylineno);
            }
            else{
                temp=table.LookupScope($1,temp_scope);
                if(temp!=NULL && temp->IsActive() && !loop_open && func_open && scope>temp_scope && !isMember && (temp->getType()==LOCALV || temp->getType()==FORMAL))
                    cout << "[Syntax Analysis] ERROR: Cannot access \"" << $1 << "\", in line " << yylineno << endl;
            }
            $$ = strdup($1);
}
    |   LOCAL ID {
            SymbolTableEntry *symbol,*temp;
            symbol = table.LookupScope($2,scope);
            if(symbol==NULL){
                temp=table.LookupScope($2,0);
                if(temp!=NULL && temp->getType()==LIBFUNC)
                    cout << "[Syntax Analysis] ERROR: Collision with library function \""<< $2 << "\", in line " << yylineno << endl;                
                else{
                    if(!scope) table.Insert($2,GLOBAL,scope,yylineno);
                    else table.Insert($2,LOCALV,scope,yylineno);
                }   
            }
            $$ = strdup($2);
    }
    |   DOUBLE_COLON ID { 
            if(table.LookupScope($2,0)==NULL)
                cout << "[Syntax Analysis] ERROR: There is no declaration of global var \"" << $2 <<"\", in line " << yylineno << endl;  $$=strdup($2);}
                    
    |   member {isMember=true;};

member:  lvalue DOT ID  | lvalue L_BRACKET exp R_BRACKET | call DOT ID | call L_BRACKET exp R_BRACKET ;

call: call L_PARENTHESIS elist R_PARENTHESIS | lvalue callsuffix | L_PARENTHESIS f_def R_PARENTHESIS L_PARENTHESIS elist R_PARENTHESIS ; 
   
callsuffix: normcall | methodcall ;

normcall: L_PARENTHESIS elist R_PARENTHESIS ;

methodcall: DOUBLE_DOT ID L_PARENTHESIS elist R_PARENTHESIS ;

elist: exp | elist COMMA exp | {};

obj_def:  L_BRACKET indexed R_BRACKET | L_BRACKET elist R_BRACKET  ;

indexed: index_el | indexed COMMA index_el ;

index_el: L_BRACE exp COLON exp R_BRACE ;

block: L_BRACE {scope++;} stmts  R_BRACE {table.Hide(scope); scope--;};

f_def: FUNCTION ID{
        SymbolTableEntry *symbol;
        symbol=table.LookupScope($2,scope);
        if(symbol!=NULL && symbol->IsActive()){
            if(symbol->getType()==USERFUNC || symbol->getType()==GLOBAL || symbol->getType()==LOCALV)
             {
                cout<< "[Syntax Analysis] ERROR: Redefinition of \"" << $2 << "\" as function, in line " << yylineno << endl;
                cout<< "\tNote: Previous definition of \""<< $2 << "\" in line " << symbol->getVar()->getLine() << endl;
             }   
            if(!scope && symbol->getType()==LIBFUNC)
                cout << "[Syntax Analysis] ERROR: Collision with library function \"" << $2 << "\", in line " << yylineno << endl;                
            if(symbol!=NULL  && symbol->getType()==FORMAL){
                cout << "[Syntax Analysis] ERROR: Redefinition of \"" << $2 << "\" as function, in line " << yylineno << endl;
                cout<< "\tNote: Previous definition of \""<< $2 << "\" in line " << symbol->getVar()->getLine() << endl;
            }
        }
        else table.Insert($2,USERFUNC,scope,yylineno);
    }
        L_PARENTHESIS {scope++;} idlist R_PARENTHESIS  { scope--; func_open++;} block {func_open--;}
        
    |  FUNCTION { string fname = "_$" + to_string(func_id_num); func_id_num++;table.Insert(fname,USERFUNC,scope,yylineno);}
         L_PARENTHESIS {scope++;} idlist R_PARENTHESIS { scope--; func_open++;}  block{func_open--;};

const: INTCONST | REALCONST | STRING | NIL | TRUE | FALSE ;

idlist: ID {
    SymbolTableEntry *symbol = table.LookupScope($1,scope),*temp;
    if(symbol!=NULL){
            cout<< "[Syntax Analysis] ERROR: Redefinition of \"" << $1 << "\" as variable, in line: " << yylineno << endl;
            cout<< "\tNote: Previous definition of \""<< $1 << "\" in line: " << symbol->getVar()->getLine() << endl;
    }
    else  {
         temp=table.LookupScope($1,0);
         if(temp!=NULL && temp->getType()==LIBFUNC)
            cout << "[Syntax Analysis] ERROR: Collision with library function \""<< $1 << "\", in line " << yylineno << endl;                
          else table.Insert($1,FORMAL,scope,yylineno);
    }
}
    | idlist COMMA ID {
    SymbolTableEntry *symbol = table.LookupScope($3,scope),*temp;
    if(symbol!=NULL){
        cout<< "[Syntax Analysis] ERROR: Redefinition of \"" << $3 << "\" as formal, in line: " << yylineno << endl;
        cout<< "\tNote: Previous definition of \""<< $3 << "\" in line: " << symbol->getVar()->getLine() << endl;
    }
    else  {
        temp=table.LookupScope($3,0);
        if(temp!=NULL && temp->getType()==LIBFUNC)
            cout << "[Syntax Analysis] ERROR: Collision with library function \""<< $3 << "\", in line " << yylineno << endl;                
        else 
            table.Insert($3,FORMAL,scope,yylineno);
    }
    }
    | ;

if_stmt: IF L_PARENTHESIS exp R_PARENTHESIS stmt | IF L_PARENTHESIS exp R_PARENTHESIS stmt ELSE stmt;

while_stmt: WHILE L_PARENTHESIS exp R_PARENTHESIS stmt ;

for_stmt: FOR L_PARENTHESIS elist SEMICOLON exp SEMICOLON elist R_PARENTHESIS stmt ;

ret_stmt: RETURN SEMICOLON {
    if(!func_open) cout << "[Syntax Analysis] ERROR: Cannot use RETURN statement while not within function, in line " << yylineno << endl;}
    | RETURN exp SEMICOLON {if(!func_open) cout << "[Syntax Analysis] ERROR: Cannot use RETURN statement while not within function, in line "<< yylineno << endl;};
%%

int yyerror(const char* yaccProvidedMessage){
	fprintf(stderr, "[Syntax Analysis] ERROR: %s before token \"%s\", in line %d\n", yaccProvidedMessage,yytext, yylineno);
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
    cout << "\n--------------------------- Errors/Warnings ---------------------------" << endl;
    yyparse();
    cout << "-----------------------------------------------------------------------" << endl;

  table.printSymbols();
  

  return 0; 
}
