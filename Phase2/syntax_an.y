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
unsigned int func_num=0,func_open=0;

bool loop_open=false,return_state=false;


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
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL
%token MOD GREATER_THAN GREATER_EQUAL LESS_THAN LESS_EQUAL PLUS_PLUS PLUS MINUS MULTIPLY DIV  MINUS_MINUS ASSIGN EQUAL NOT_EQUAL 
%token LEFT_BRACE RIGHT_BRACE LEFT_PARENTHESIS RIGHT_PARENTHESIS LEFT_BRACKET RIGHT_BRACKET SEMICOLON COLON DOUBLE_COLON DOT DOUBLE_DOT COMMA
%type <stringValue> lvalue

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
%right THEN ELSE
%%

program: stmts 
        ;
stmt:  exp SEMICOLON 
      | if_stmt | while_stmt | for_stmt | ret_stmt 
      | BREAK SEMICOLON {
        if(!loop_open) 
            cout << "[Syntax Analysis] ERROR: Wrong declaration of BREAK statement in line " << yylineno << endl;
      }
      | CONTINUE SEMICOLON {
        if(!loop_open) 
            cout << "[Syntax Analysis] ERROR: Wrong declaration of CONTIUE statement in line " << yylineno << endl;
      }
      | block| f_def | SEMICOLON ;

stmts: stmts stmt | ;

exp: assign_exp | exp PLUS exp | exp MINUS exp | exp MULTIPLY exp | exp DIV exp | exp MOD exp 
    | exp LESS_THAN exp | exp GREATER_THAN exp | exp LESS_EQUAL exp | exp GREATER_EQUAL exp 
    | exp AND exp | exp OR exp | exp EQUAL exp | exp NOT_EQUAL exp | term ;

term: LEFT_PARENTHESIS exp RIGHT_PARENTHESIS | MINUS exp %prec UMINUS | NOT exp 
    | PLUS_PLUS lvalue {
        SymbolTableEntry *symbol=table.Lookup($2);
        if(symbol->getType()==USERFUNC || symbol->getType()==LIBFUNC)
            cout <<"[Syntax Analysis] ERROR: trying to left increase the function " << $2 << "in line" <<  yylineno << endl;
    }
    | lvalue PLUS_PLUS {
        SymbolTableEntry *symbol=table.Lookup($1);
        if(symbol->getType()==USERFUNC || symbol->getType()==LIBFUNC)
            cout <<"[Syntax Analysis] ERROR: trying to right increase the function " << $1 << "in line" <<yylineno << endl;
    }
    | MINUS_MINUS lvalue {
        SymbolTableEntry *symbol=table.Lookup($2);
        if(symbol->getType()==USERFUNC || symbol->getType()==LIBFUNC)
            cout <<"[Syntax Analysis] ERROR: trying to left decrease the function " << $2 << "in line" << yylineno << endl;
    }

    | lvalue MINUS_MINUS {
             SymbolTableEntry *symbol=table.Lookup($1);
        if(symbol->getType()==USERFUNC || symbol->getType()==LIBFUNC)
            cout <<"[Syntax Analysis] ERROR: trying to right decrease the function " << $1 << "in line" << yylineno << endl;
    }
    | primary  
    ;
assign_exp: lvalue ASSIGN exp {
    SymbolTableEntry *symbol;
    for(int i=scope; i>=0; i--){
        symbol=table.LookupScope($1,i);
        if(symbol!=NULL && (symbol->getType()==USERFUNC || symbol->getType()==LIBFUNC)){
            cout << "[Syntax Analysis] ERROR: trying to assign a value in " << $1 << " in line "<< yylineno << endl;
            break;
        }  
    }   
    
}
    ;
primary: lvalue | call | obj_def | LEFT_PARENTHESIS f_def RIGHT_PARENTHESIS | const ;

lvalue: ID {
            SymbolTableEntry *symbol,*temp;
            unsigned int temp_scope;
            bool found = false;
            symbol = table.LookupScope($1,scope);
            if(symbol==NULL){
                for (int i=scope; i>=0 ; --i){
                    temp = table.LookupScope($1,i);
                    if(temp!=NULL && temp->IsActive()) {
                        found = true;
                        temp_scope=i;
                        break;
                    }
                } 
            
                if(!scope)
                    table.Insert($1,GLOBAL,scope,yylineno);
                else if(found && func_open && temp_scope && symbol->getType()!=USERFUNC)
                    cout << "[Syntax Analysis] ERROR: Cannot access " <<  $1  <<  " in line " << yylineno << endl;
                else 
                    if(!found) table.Insert($1,LOCALV,scope,yylineno);
           
                $$ = strdup($1);
            }
}
    |   LOCAL ID {
                unsigned int temp_scope,found;
                SymbolTableEntry *symbol,*temp;
            symbol =table.Lookup($2);
            if(symbol==NULL){
                for(int i=scope ; i>=0 ; i--){
                    temp=table.LookupScope($2,i);
                    if(temp!=NULL && temp->IsActive() && temp->getType()!=USERFUNC){
                        found=true;
                        temp_scope=i;
                    }
                    if(found) break;

                }
                temp=table.LookupScope($2,0);
                if(temp!=NULL && temp_scope==0 && temp->IsActive() && temp->getType()==LIBFUNC)
                    cout << "[Syntax Analysis] ERROR: Trying to overshadow a library function in line " << yylineno << endl;
                else if(!temp_scope)
                    table.Insert($2,GLOBAL,scope,yylineno);
                else if(found && func_open && temp_scope)
                    cout << "[Syntax Analysis] ERROR: Cannot access " <<  $2 <<" a library function in line " << yylineno << endl;
                else 
                    table.Insert($2,LOCALV,scope,yylineno);

                $$=strdup($2);
            }
            else{
                if(symbol->getType()==USERFUNC)
                    cout << "[Syntax Analysis] ERROR: redeclaration of " << $2 << " as variable in line " << yylineno << endl;
                if(symbol->getType()==LIBFUNC)
                    cout << "[Syntax Analysis] ERROR: collision with library function << $2 << in line " << yylineno << endl;
            }

    }
    |   DOUBLE_COLON ID {
        if(table.LookupScope($2,0)==NULL)
                cout << "[Syntax Analysis] ERROR! there is no declaration of global var " << $2 <<" in line " << yylineno << endl;
    }|   member ;

member:  lvalue DOT ID | lvalue LEFT_BRACKET exp RIGHT_BRACKET | call DOT ID | call LEFT_BRACKET exp RIGHT_BRACKET ;

call: call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS | lvalue callsuffix | LEFT_PARENTHESIS f_def RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS ; 
   
callsuffix: normcall | methodcall ;

normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS ;

methodcall: DOUBLE_DOT ID LEFT_PARENTHESIS elist RIGHT_PARENTHESIS ;

elist: exp | elist COMMA exp | {};

obj_def:  LEFT_BRACKET indexed RIGHT_BRACKET | LEFT_BRACKET elist RIGHT_BRACKET  ;

indexed: index_el | indexed COMMA index_el ;

index_el: LEFT_BRACE exp COLON exp RIGHT_BRACE ;

block: LEFT_BRACE {scope++;} stmts RIGHT_BRACE { table.Hide(scope); scope--;};

f_def: FUNCTION ID{
        SymbolTableEntry *symbol;
        symbol=table.Lookup($2);
            if(symbol!=NULL){
                if(symbol->getType()==LIBFUNC)
                    cout << "[Syntax Analysis] ERROR: Trying to overshadow a library function in line " << yylineno << endl;           
                else if(symbol->getType()==USERFUNC){
                    if(symbol->getVar()->getScope()==scope){
                    cout<< "[Syntax Analysis] ERROR: redefinition of " << $2 << " in line: " << yylineno << endl;
                    cout<< "\tNote: Previous definition of "<< $2 << " in line: " << symbol->getVar()->getLine() << endl;
                    }
                    else 
                    table.Insert($2,USERFUNC,scope,yylineno);
                }
                else if(symbol->getType()==GLOBAL){
                    cout<< "[Syntax Analysis] ERROR: redeclaration of Global Variable " << $2 << " as function in line: "<< yylineno<<endl;
                }
                else if(symbol->getType()==LOCALV || symbol->getType()==FORMAL){
                    if(symbol->getVar()->getScope()==scope)
                        cout<< "[Syntax Analysis] ERROR: redeclaration of local variable " << $2 <<" as function in line: "<< yylineno<<endl;
                    else
                        table.Insert($2,USERFUNC,scope,yylineno);

                }
            }
            else{
                table.Insert($2,USERFUNC,scope,yylineno);
            }

        }
        LEFT_PARENTHESIS {scope++;} 
        idlist RIGHT_PARENTHESIS  { scope--; func_open++;} 
        block {func_open--;}
        
    |  FUNCTION { string fname = "_$" + to_string(func_num);
                   func_num++;
                   table.Insert(fname,USERFUNC,scope,yylineno);

          }
         LEFT_PARENTHESIS {scope++;} idlist RIGHT_PARENTHESIS { scope--; func_open++;}  
    block{func_open--;}
    ;

const: INTCONST | REALCONST | STRING | NIL | TRUE | FALSE ;

idlist: ID {
    SymbolTableEntry *symbol = table.LookupScope($1,scope),*temp;

    if(symbol==NULL){
	    temp = table.LookupScope($1,0);
        if(temp!=NULL && temp->getType()==LIBFUNC )
            cout << "[Syntax Analysis] ERROR: Trying to overshadow a library function in line " << yylineno << endl;           
        else
            table.Insert($1,FORMAL,scope,yylineno);
    }
    else {
        if(symbol->getType()==LIBFUNC)
            cout << "[Syntax Analysis] ERROR: Trying to overshadow a library function in line " << yylineno << endl;           
        else if(symbol->IsActive()){
            cout<< "[Syntax Analysis] ERROR: redefinition of " << $1 << " in line: " << yylineno << endl;
            cout<< "\tNote: Previous definition of "<< $1 << " in line: " << symbol->getVar()->getLine() << endl;
        }
    }
}
    | idlist COMMA ID {
        SymbolTableEntry *symbol = table.LookupScope($3,scope),*temp;

        if(symbol==NULL){
	        temp = table.LookupScope($3,0);
            if(temp!=NULL && temp->getType()==LIBFUNC)
                cout << "[Syntax Analysis] ERROR: Trying to overshadow a library function in line " << yylineno << endl;           
            else 
                table.Insert($3,FORMAL,scope,yylineno);
        }
        else {
            if(symbol->getType()==LIBFUNC)
                cout << "[Syntax Analysis] ERROR: Trying to overshadow a library function in line " << yylineno << endl;           
            else if(symbol->IsActive()){
                cout<< "[Syntax Analysis] ERROR: redefinition of " << $3 << " in line: " << yylineno << endl;
                cout<< "\tNote: Previous definition of "<< $3 << " in line: " << symbol->getVar()->getLine() << endl;
	    }
  }

    }
    | ;

if_stmt: IF LEFT_PARENTHESIS exp RIGHT_PARENTHESIS stmt | IF LEFT_PARENTHESIS exp RIGHT_PARENTHESIS stmt ELSE stmt;

while_stmt: WHILE LEFT_PARENTHESIS{loop_open=true;} exp RIGHT_PARENTHESIS{loop_open=false;} stmt ;

for_stmt: FOR LEFT_PARENTHESIS {loop_open=true;} elist SEMICOLON exp SEMICOLON elist RIGHT_PARENTHESIS {loop_open=false;} stmt ;

ret_stmt: RETURN SEMICOLON {
    if(!func_open) cout << "[Syntax Analysis] ERROR: Wrong use of RETURN statement in line " << yylineno << endl;}
    | RETURN exp SEMICOLON {
        if(!func_open) cout << "[Syntax Analysis] ERROR: Wrong use of RETURN statement in line "<< yylineno << endl;};
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
    cout << "\n--------------------------- Errors/Warnings ---------------------------" << endl;
    yyparse();
    cout << "-----------------------------------------------------------------------" << endl;

  table.printSymbols();

  return 0; 
}
