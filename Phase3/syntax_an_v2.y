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
#include "SymbolTable_v2.h"
#include "iCode.h"

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
    class expr *expression;
}

%output="syntax_analyzer.cpp"

%token <stringValue> ID
%token <intValue> INTCONST
%token <realValue> REALCONST
%token <stringValue> STRING
/*Tokens for expressions*/
%type <expression> exp lvalue const primary f_def member assign_exp term obj_def
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

stmt:  exp SEMICOLON  {resettemp();}
      |  if_stmt | {loop_open++;} while_stmt {loop_open--;} | {loop_open++;} for_stmt {loop_open--;} | ret_stmt 
      | BREAK SEMICOLON {if(!loop_open) cout << "[Syntax Analysis] ERROR: Cannot use BREAK statement while not within loop, in line " << yylineno << endl;}
      | CONTINUE SEMICOLON {if(!loop_open) cout << "[Syntax Analysis] ERROR: Cannot use CONTINUE statement while not within loop, in line " << yylineno << endl;}
      | block | f_def | SEMICOLON ;

exp: assign_exp 
    | exp PLUS exp{
       $$ = new expr(arithexp_e);
       $$->insertSymbol(newtemp());
       emit(add, $1, $3, $$,0,yylineno);
    } 
    | exp MINUS exp{
       $$ = new expr(arithexp_e);
       $$->insertSymbol(newtemp());
       emit(sub, $1, $3, $$,0,yylineno);
    } 
    | exp MULTIPLY exp{
       $$ = new expr(arithexp_e);
       $$->insertSymbol(newtemp());
       emit(mul, $1, $3, $$,0,yylineno);
    }
    | exp DIV exp{
       $$ = new expr(arithexp_e);
       $$->insertSymbol(newtemp());
       emit(divide, $1, $3, $$,0,yylineno);
    }
    | exp MOD exp{
       $$ = new expr(arithexp_e);
       $$->insertSymbol(newtemp());
       emit(mod, $1, $3, $$, 0, yylineno);
    }
    | exp LESS_THAN exp{
       $$ = new expr(boolexp_e);
       $$->insertSymbol(newtemp());
       emit(if_less, $1, $3, $$, 0, yylineno);
    }
    | exp GREATER_THAN exp{
       $$ = new expr(boolexp_e);
       $$->insertSymbol(newtemp());
       emit(if_greater, $1, $3, $$, 0, yylineno);
    }
    | exp LESS_EQUAL exp{
       $$ = new expr(boolexp_e);
       $$->insertSymbol(newtemp());
       emit(if_lesseq, $1, $3, $$, 0, yylineno);
    }
    | exp GREATER_EQUAL exp {
       $$ = new expr(boolexp_e);
       $$->insertSymbol(newtemp());
       emit(if_greatereq, $1, $3, $$, 0, yylineno);
    }
    | exp AND exp {
       $$ = new expr(boolexp_e);
       $$->insertSymbol(newtemp());
       emit(andb, $1, $3, $$, 0, yylineno);
    }
    | exp OR exp {
       $$ = new expr(boolexp_e);
       $$->insertSymbol(newtemp());
       emit(orb, $1, $3, $$, 0, yylineno);
    }
    | exp EQUAL exp{
       $$ = new expr(boolexp_e);
       $$->insertSymbol(newtemp());
       emit(if_eq, $1, $3, $$, 0, yylineno);
    }
    | exp NOT_EQUAL exp{
       $$ = new expr(boolexp_e);
       $$->insertSymbol(newtemp());
       emit(notb, $1, $3, $$, 0, yylineno);
    } 
    | term {$$=$1;};


term: L_PARENTHESIS exp R_PARENTHESIS | MINUS exp %prec UMINUS | NOT exp 
    | PLUS_PLUS lvalue {
        Symbol *symbol=table.Lookup($2->getSymbol()->getName());
        if(symbol->getType()==programfunc_s || symbol->getType()==libraryfunc_s)
            cout <<"[Syntax Analysis] ERROR: Trying to left increase the function \"" << $2 << "\" by 1, in line " <<  yylineno << endl;}
    | lvalue PLUS_PLUS {
        Symbol *symbol=table.Lookup($1->getSymbol()->getName());
        if(symbol->getType()==programfunc_s || symbol->getType()==libraryfunc_s)
            cout <<"[Syntax Analysis] ERROR: Trying to right increase the function \"" << $1 << "\" by 1, in line " <<yylineno << endl;}
    | MINUS_MINUS lvalue {
        Symbol *symbol=table.Lookup($2->getSymbol()->getName());
        if(symbol->getType()==programfunc_s || symbol->getType()==libraryfunc_s)
            cout <<"[Syntax Analysis] ERROR: Trying to left decrease the function \"" << $2 << "\" by 1, in line" << yylineno << endl;}
    | lvalue MINUS_MINUS {
        Symbol *symbol=table.Lookup($1->getSymbol()->getName());
        if(symbol->getType()==programfunc_s || symbol->getType()==libraryfunc_s)
            cout <<"[Syntax Analysis] ERROR: Trying to right decrease the function \"" << $1 << "\" by 1, in line " << yylineno << endl;}
    | primary {$$=$1;} ;

assign_exp: lvalue ASSIGN exp {
    Symbol *symbol;
        symbol=table.LookupScope($1->getSymbol()->getName(),0);
        if(symbol!=NULL && !isMember && symbol->getType()==libraryfunc_s)
            cout << "[Syntax Analysis] ERROR: Trying to assign a value to \"" << $1 << "\" library function, in line "<< yylineno << endl;
        for(int i=scope ; i>=0 ; i--){
            symbol=table.LookupScope($1->getSymbol()->getName(),i);
            if(symbol!=NULL && !isMember && scope==i && symbol->getType()==programfunc_s)
                cout << "[Syntax Analysis] ERROR: Trying to assign a value to \"" << $1 << "\" user function, in line "<< yylineno << endl;
        }  
        if(isMember) isMember=false;
};

primary: lvalue {$$=$1;}| call | obj_def | L_PARENTHESIS f_def R_PARENTHESIS | const {$$=$1;} ;

lvalue: ID {

            Symbol *symbol,*temp;
            expr *expression;
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
                table.Insert(new Symbol(var_s,$1,currscopespace(),scope,yylineno,currscopeoffset()));
            }
            else{
                temp=table.LookupScope($1,temp_scope);
                if(temp!=NULL && temp->IsActive() && !loop_open && func_open && scope>temp_scope && !isMember && temp->getType()==var_s)
                    cout << "[Syntax Analysis] ERROR: Cannot access \"" << $1 << "\", in line " << yylineno << endl;
                
            }
            symbol=table.LookupScope($1,scope);
            $$ = lvalue_exp(symbol);
            

}
    |   LOCAL ID {
            Symbol *symbol,*temp;
            symbol = table.LookupScope($2,scope);
            if(symbol==NULL){
                temp=table.LookupScope($2,0);
                if(temp!=NULL && temp->getType()==libraryfunc_s)
                    cout << "[Syntax Analysis] ERROR: Collision with library function \""<< $2 << "\", in line " << yylineno << endl;                
                else{
                    table.Insert(new Symbol(var_s,$2,currscopespace(),scope,yylineno,currscopeoffset()));
                }   
            }
            symbol=table.LookupScope($2,scope);
            $$ = lvalue_exp(symbol);

    }
    |   DOUBLE_COLON ID { 
            Symbol *symbol=table.LookupScope($2,0);
            if(symbol==NULL)
                cout << "[Syntax Analysis] ERROR: There is no declaration of global var \"" << $2 <<"\", in line " << yylineno << endl;  
            $$ = lvalue_exp(symbol);

            
    }

                    
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
        Symbol *symbol;
        symbol=table.LookupScope($2,scope);
        if(symbol!=NULL && symbol->IsActive()){
            if(symbol->getType()==programfunc_s)
             {
                cout<< "[Syntax Analysis] ERROR: Redefinition of \"" << $2 << "\" as function, in line " << yylineno << endl;
                cout<< "\tNote: Previous definition of \""<< $2 << "\" in line " << symbol->getLine() << endl;
             }   
            if(!scope && symbol->getType()==libraryfunc_s){
                $$ = new expr(libraryfunc_e);
                $$->insertSymbol(symbol);
                cout << "[Syntax Analysis] ERROR: Collision with library function \"" << $2 << "\", in line " << yylineno << endl;                
            }
            if(symbol!=NULL  && symbol->getType()==var_s){
                cout << "[Syntax Analysis] ERROR: Redefinition of \"" << $2 << "\" as function, in line " << yylineno << endl;
                cout<< "\tNote: Previous definition of \""<< $2 << "\" in line " << symbol->getLine() << endl;
            }
        }
        else{
            table.Insert(new Symbol(programfunc_s,$2,currscopespace(),scope,yylineno,currscopeoffset()));
            $$ = new expr(programfunc_e);
            $$->insertSymbol(table.LookupScope($2,scope));
        }
    }
        L_PARENTHESIS {scope++;} idlist R_PARENTHESIS  { scope--; func_open++;} block {func_open--;}
        
    |  FUNCTION { 
            string fname = "n$" + to_string(func_id_num); func_id_num++; table.Insert(new Symbol(programfunc_s,fname,currscopespace(),scope,yylineno,currscopeoffset()));
            
            $$ = new expr(programfunc_e);
            $$->insertSymbol(table.LookupScope(fname,scope));
        }
         L_PARENTHESIS {scope++;} idlist R_PARENTHESIS { scope--; func_open++;}  block{func_open--;};

const: INTCONST { 
                    $$ = new expr(costnum_e);
                    $$->setnumconst(stoi(yytext));
                }   
    | REALCONST {
                    $$ = new expr(costnum_e);
                    $$->setnumconst(stoi(yytext));
                }
       | STRING {
                    $$ = new expr(conststring_e);
                    $$->setstrConst(yytext);

                }
        | NIL  {
                    $$ = new expr(nil_e);
                }
        | TRUE {
                    $$ = new expr(constbool_e);
                    $$->setboolConst(true);
                } 
        | FALSE {
                    $$ = new expr(constbool_e);
                    $$->setboolConst(false);
                } ;

idlist: ID {
    Symbol *symbol = table.LookupScope($1,scope),*temp;
    if(symbol!=NULL){
            cout<< "[Syntax Analysis] ERROR: Redefinition of \"" << $1 << "\" as variable, in line: " << yylineno << endl;
            cout<< "\tNote: Previous definition of \""<< $1 << "\" in line: " << symbol->getLine() << endl;
    }
    else  {
         temp=table.LookupScope($1,0);
         if(temp!=NULL && temp->getType()==libraryfunc_s)
            cout << "[Syntax Analysis] ERROR: Collision with library function \""<< $1 << "\", in line " << yylineno << endl;                
          else table.Insert(new Symbol(var_s,$1,currscopespace(),scope,yylineno,currscopeoffset()));
    }
}
    | idlist COMMA ID {
    Symbol *symbol = table.LookupScope($3,scope),*temp;
    if(symbol!=NULL){
        cout<< "[Syntax Analysis] ERROR: Redefinition of \"" << $3 << "\" as formal, in line: " << yylineno << endl;
        cout<< "\tNote: Previous definition of \""<< $3 << "\" in line: " << symbol->getLine() << endl;
    }
    else  {
        temp=table.LookupScope($3,0);
        if(temp!=NULL && temp->getType()==libraryfunc_s)
            cout << "[Syntax Analysis] ERROR: Collision with library function \""<< $3 << "\", in line " << yylineno << endl;                
        else 
            table.Insert(new Symbol(var_s,$3,currscopespace(),scope,yylineno,currscopeoffset()));
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
  print_quads();


  return 0; 
}
