%{
/*** --- SYNTAX ANALYSIS YACC/BISON FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.3.1 , x86 chip***/
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
extern contbreaklists *BClist;
extern stack <unsigned> LoopCounterStack;
extern stack <unsigned> functionLocalStack;
unsigned int scope=0,loop_open=0,func_open=0,func_id_num=0;
unsigned loopcounter = 0;
bool isMember=false;

%}

%start program 
%error-verbose
%output="syntax_analyzer.cpp"

%expect 1

%union {
    char *stringValue; 
    int intValue; 
    unsigned labelValue;
    double realValue;
    class Symbol *sym;
    class expr *expression;
    class forprefix *fprefix;
    class contbreaklists *BCLists;
    class calls *Call;
    class elists *elistV;
}

/*Integer const token*/
%token <intValue> INTCONST 
/*Real const token*/
%token <realValue> REALCONST
/*String type tokens*/
%token <stringValue> ID STRING
/*Tokens for functions*/
%type <intValue> funcbody
%type <stringValue> funcname
%type <sym> funcprefix f_def
/*Tokens for expressions*/
%type <expression> exp lvalue const primary member assign_exp term obj_def call
%type <elistV> elist
/*Tokens for labels*/
%type <labelValue> M N if_prefix else_prefix whilestart whilecond
/*Token for for prefix*/
%type <fprefix> for_prefix
/*Tokens for break and continue lists*/
%type <BCLists> stmts stmt if_stmt while_stmt for_stmt ret_stmt block loopstmt
/*Tokens for calls*/
%type <Call> normcall methodcall callsuffix
/*Tokens for keywords*/
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL
/*Tokens for operators*/
%token ASSIGN PLUS MINUS MULTIPLY DIV MOD PLUS_PLUS MINUS_MINUS LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL EQUAL NOT_EQUAL 
/*Tokens for punctuators*/
%token L_BRACE R_BRACE L_PARENTHESIS R_PARENTHESIS L_BRACKET R_BRACKET SEMICOLON COMMA COLON DOUBLE_COLON DOT DOUBLE_DOT 

%right ASSIGN
%left OR
%left AND
%nonassoc EQUAL NOT_EQUAL
%nonassoc GREATER_THAN GREATER_EQUAL LESS_THAN LESS_EQUAL
%left  PLUS MINUS
%left  MULTIPLY DIV MOD
%right NOT PLUS_PLUS MINUS_MINUS UMINUS
%left DOT DOUBLE_DOT
%left L_BRACKET R_BRACKET
%left L_PARENTHESIS R_PARENTHESIS
%nonassoc ELSE

%%

program: stmts ;

stmts: stmts stmt{
    $$->setBreakList(merge($1->getBreakList(),$2->getBreakList()));
    $$->setContList(merge($1->getContList(),$2->getContList()));
} | stmt {$$=$1;} ;

stmt:  exp SEMICOLON  {resettemp();}
      |  if_stmt 
      | {loop_open++;} while_stmt {loop_open--;} 
      | {loop_open++;} for_stmt {loop_open--;} 
      | ret_stmt 
      | BREAK SEMICOLON {
          if(!loop_open) cout << "[Syntax Analysis] ERROR: Cannot use BREAK statement while not within loop, in line " << yylineno << endl;
          $$ = BClist;
          $$->insertBreakList(nextQuad());
          emit(jump,NULL,NULL,NULL,0,yylineno);
      }
      | CONTINUE SEMICOLON {
          if(!loop_open) cout << "[Syntax Analysis] ERROR: Cannot use CONTINUE statement while not within loop, in line " << yylineno << endl;
          $$ = BClist;
          $$->insertContList(nextQuad());
          emit(jump,NULL,NULL,NULL,0,yylineno);
      }
      | block 
      | f_def 
      | SEMICOLON ;


exp: assign_exp {$$=$1;}
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
        emit(if_less, $1, $3, $$, nextQuad()+3, yylineno);

        expr *temp=new expr(boolexp_e);
        temp->setboolConst(false);
        emit(assign,temp,NULL,$$,0,yylineno); 

        emit(jump,NULL,NULL,NULL,nextQuad()+3,yylineno);

        temp=new expr(boolexp_e);
        temp->setboolConst(true);
        emit(assign,temp,NULL,$$,0,yylineno); 
    }
    | exp GREATER_THAN exp{
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
        emit(if_greater, $1, $3, $$, nextQuad()+3, yylineno);

        expr *temp=new expr(boolexp_e);
        temp->setboolConst(false);
        emit(assign,temp,NULL,$$,0,yylineno); 

        emit(jump,NULL,NULL,NULL,nextQuad()+3,yylineno);

        temp=new expr(boolexp_e);
        temp->setboolConst(true);
        emit(assign,temp,NULL,$$,0,yylineno); 
    }
    | exp LESS_EQUAL exp{
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
        emit(if_lesseq, $1, $3, $$, nextQuad()+4, yylineno);

        expr *temp=new expr(boolexp_e);
        temp->setboolConst(false);
        emit(assign,temp,NULL,$$,0,yylineno); 

        emit(jump,NULL,NULL,NULL,nextQuad()+4,yylineno);

        temp=new expr(boolexp_e);
        temp->setboolConst(true);
        emit(assign,temp,NULL,$$,0,yylineno); 
    }
    | exp GREATER_EQUAL exp {
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
        emit(if_greatereq, $1, $3, $$, nextQuad()+4, yylineno);

        expr *temp=new expr(boolexp_e);
        temp->setboolConst(false);
        emit(assign,temp,NULL,$$,0,yylineno); 

        emit(jump,NULL,NULL,NULL,nextQuad()+3,yylineno);

        temp=new expr(boolexp_e);
        temp->setboolConst(true);
        emit(assign,temp,NULL,$$,0,yylineno); 
    }
    | exp AND M exp {
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
        emit(andb, $1, $4, $$, 0, yylineno);
       
        backpatch($1->getTrueList(),$3);
        $$->setTrueList($4->getTrueList());
        $$->setFalseList(merge($1->getFalseList(),$4->getFalseList()));


    }
    | exp OR M exp {
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
        emit(orb, $1, $4, $$, 0, yylineno);
       
        backpatch($1->getFalseList(),$3);
        $$->setTrueList(merge($1->getTrueList(),$4->getTrueList()));
        $$->setFalseList($4->getFalseList());

    }
    | exp EQUAL exp{
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
        emit(if_eq, $1, $3, $$, nextQuad()+4, yylineno);

        expr *temp=new expr(boolexp_e);
        temp->setboolConst(false);
        emit(assign,temp,NULL,$$,0,yylineno); 

        emit(jump,NULL,NULL,NULL,nextQuad()+3,yylineno);

        temp=new expr(boolexp_e);
        temp->setboolConst(true);
        emit(assign,temp,NULL,$$,0,yylineno); 
    }
    | exp NOT_EQUAL exp{
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
        emit(if_noteq, $1, $3, $$, nextQuad()+4, yylineno);

        expr *temp=new expr(boolexp_e);
        temp->setboolConst(false);
        emit(assign,temp,NULL,$$,0,yylineno); 

        emit(jump,NULL,NULL,NULL,nextQuad()+3,yylineno);

        temp=new expr(boolexp_e);
        temp->setboolConst(true);
        emit(assign,temp,NULL,$$,0,yylineno); 
    } 
    | term {$$=$1;};

term: L_PARENTHESIS exp R_PARENTHESIS {$$=$2;}
    | MINUS exp %prec UMINUS {
        checkuminus($2);
        $$=new expr(arithexp_e);
        $$->insertSymbol(newtemp());
        emit(uminus,$2,NULL,$$,0,yylineno);
    }
    | NOT exp {
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
        emit(notb,$2,NULL,$$,0,yylineno);
        $$->setTrueList($2->getFalseList());
        $$->setFalseList($2->getTrueList());
    }
    | PLUS_PLUS lvalue {
        Symbol *symbol=table.Lookup($2->getSymbol()->getName());
        if(symbol->getType()==programfunc_s || symbol->getType()==libraryfunc_s)
            cout <<"[Syntax Analysis] ERROR: Trying to left increase the function \"" << $2 << "\" by 1, in line " <<  yylineno << endl;

        if($2->getType() == tableitem_e){
            $$ =  emit_iftableitem($2);
            expr *temp = new expr(costnum_e);
            temp->setnumconst(1);
            emit(add,$$,temp,$$,0,yylineno);
            emit(tablesetelem,$2,$2->getIndex(),$$,0,yylineno);
        }
        else{
            expr *temp = new expr(costnum_e);
            temp->setnumconst(1);
            emit(add,$2,temp,$2,0,yylineno);
            $$ = new expr(arithexp_e);
            $$->insertSymbol(newtemp());
            emit(assign,$2,NULL,$$,0,yylineno);
        }

    }
    | lvalue PLUS_PLUS {
        Symbol *symbol=table.Lookup($1->getSymbol()->getName());
        if(symbol->getType()==programfunc_s || symbol->getType()==libraryfunc_s)
            cout <<"[Syntax Analysis] ERROR: Trying to right increase the function \"" << $1 << "\" by 1, in line " <<yylineno << endl;
    
        $$ = new expr(var_e);
        $$->insertSymbol(newtemp());
        if($1->getType() == tableitem_e){
            expr *value = emit_iftableitem($1);
            expr *temp = new expr(costnum_e);
            temp->setnumconst(1);
            emit(assign,value,NULL,$$,0,yylineno);
            emit(add,value,temp,value,0,yylineno);
            emit(tablesetelem,$1,$1->getIndex(),value,0,yylineno);
        }
        else{
            emit(assign,$1,NULL,$$,0,yylineno);
            expr *temp = new expr(costnum_e);
            temp->setnumconst(1);
            emit(add,$1,temp,$1,0,yylineno);
        }
    }
    | MINUS_MINUS lvalue {
        Symbol *symbol=table.Lookup($2->getSymbol()->getName());
        if(symbol->getType()==programfunc_s || symbol->getType()==libraryfunc_s)
            cout <<"[Syntax Analysis] ERROR: Trying to left decrease the function \"" << $2 << "\" by 1, in line" << yylineno << endl;
    
        if($2->getType() == tableitem_e){
            $$ =  emit_iftableitem($2);
            expr *temp = new expr(costnum_e);
            temp->setnumconst(-1);
            emit(add,$$,temp,$$,0,yylineno);
            emit(tablesetelem,$2,$2->getIndex(),$$,0,yylineno);
        }
        else{
            expr *temp = new expr(costnum_e);
            temp->setnumconst(-1);
            emit(add,$2,temp,$2,0,yylineno);
            $$ = new expr(arithexp_e);
            $$->insertSymbol(newtemp());
            emit(assign,$2,NULL,$$,0,yylineno);
        }    
    }
    | lvalue MINUS_MINUS {
        Symbol *symbol=table.Lookup($1->getSymbol()->getName());
        if(symbol->getType()==programfunc_s || symbol->getType()==libraryfunc_s)
            cout <<"[Syntax Analysis] ERROR: Trying to right decrease the function \"" << $1 << "\" by 1, in line " << yylineno << endl;
            
        $$ = new expr(var_e);
        $$->insertSymbol(newtemp());
        if($1->getType() == tableitem_e){
            expr *value = emit_iftableitem($1);
            expr *temp = new expr(costnum_e);
            temp->setnumconst(-1);
            emit(assign,value,NULL,$$,0,yylineno);
            emit(add,value,temp,value,0,yylineno);
            emit(tablesetelem,$1,$1->getIndex(),value,0,yylineno);
        }
        else{
            emit(assign,$1,NULL,$$,0,yylineno);
            expr *temp = new expr(costnum_e);
            temp->setnumconst(-1);
            emit(add,$1,temp,$1,0,yylineno);
        }    }
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
        
        if($1->getType()==tableitem_e)
            emit(tablesetelem,$1,$1->getIndex(),$3,0,yylineno);
        else{
            emit(assign,$3,NULL,$1,0,yylineno);
            $$ = new expr(assignexp_e);
            $$->insertSymbol(newtemp());
            emit(assign,$1,NULL,$$,0,yylineno);
        }
};

primary: lvalue { $$ = emit_iftableitem($1);}
        | call 
        | obj_def 
        | L_PARENTHESIS f_def R_PARENTHESIS {
            $$ = new expr(programfunc_e);
            $$->insertSymbol($2);
        }
        | const {$$=$1;} ;

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

member:  lvalue DOT ID   {$$ = member_item($1,$3);}
    | lvalue L_BRACKET exp R_BRACKET {
        $1 = emit_iftableitem($1);
        $$ = new expr(tableitem_e);
        $$->insertSymbol($1->getSymbol());
        $$->setIndex($3);
    }
    | call DOT ID 
    | call L_BRACKET exp R_BRACKET ;

call: call L_PARENTHESIS elist R_PARENTHESIS { $$ = make_call($1,$3); }
    | lvalue callsuffix {
        if($2->getMethod()){
            expr *self = $1;
            //$1 = emit_iftableitem(member_item(self,$2->getName()));  // TO DO!
            $2->getElist()->pushfrontElistItem(self);
        }
        $$ = make_call($1,$2->getElist());

    }
    | L_PARENTHESIS f_def R_PARENTHESIS L_PARENTHESIS elist R_PARENTHESIS{
        expr *func = new expr(programfunc_e);
        func->insertSymbol($2);
        $$ = make_call(func,$5);
    } ; 
   
callsuffix: normcall {$$=$1;}
    | methodcall {$$=$1;} ;

normcall: L_PARENTHESIS elist R_PARENTHESIS { $$ = new calls("nil",false,$2);};

methodcall: DOUBLE_DOT ID L_PARENTHESIS elist R_PARENTHESIS {
    $$ = new calls($2,true,$4);
} ;

elist: exp {$$=new elists($1);}
    | elist COMMA exp {$$=$1; $$->insertElistItem($3);}
    | {}; 

obj_def:  L_BRACKET indexed R_BRACKET 
    | L_BRACKET elist R_BRACKET {
        expr *temp = new expr(newtable_e),*tempi;
        temp->insertSymbol(newtemp());
        emit(tablecreate,NULL,NULL,temp,0,yylineno);
        int count = 0;
        for(list <expr>::iterator i = $2->getElist()->begin() ; i!=$2->getElist()->end() ; i++){
            tempi = new expr(costnum_e);
            count++;
            tempi->setnumconst(count);
            emit(tablesetelem,temp,tempi,&*i,0,yylineno);
        }
        $$ = temp;
    }  ;

indexed: index_el 
    | indexed COMMA index_el ;

index_el: L_BRACE exp COLON exp R_BRACE ;

block: L_BRACE {scope++;} stmts  R_BRACE {table.Hide(scope); scope--;};

funcblockstart: {LoopCounterStack.push(loopcounter); loopcounter=0; };

funcblockend:  { loopcounter = LoopCounterStack.top(); LoopCounterStack.pop(); };

funcname: ID { $$ = $1; } 
            | { string fname = "n$" + to_string(func_id_num); func_id_num++; $$ = strcpy(new char[fname.length() + 1], fname.c_str()); };   


funcprefix: FUNCTION funcname{
        Symbol *symbol;
        symbol=table.LookupScope($2,scope);
        if(symbol!=NULL && symbol->IsActive()){
            if(symbol->getType()==programfunc_s)
             {
                cout<< "[Syntax Analysis] ERROR: Redefinition of \"" << $2 << "\" as function, in line " << yylineno << endl;
                cout<< "\tNote: Previous definition of \""<< $2 << "\" in line " << symbol->getLine() << endl;
             }   
            if(!scope && symbol->getType()==libraryfunc_s){
                //$$ = new expr(libraryfunc_e);
                //$$->insertSymbol(symbol);
                cout << "[Syntax Analysis] ERROR: Collision with library function \"" << $2 << "\", in line " << yylineno << endl;                
            }
            if(symbol!=NULL  && symbol->getType()==var_s){
                cout << "[Syntax Analysis] ERROR: Redefinition of \"" << $2 << "\" as function, in line " << yylineno << endl;
                cout<< "\tNote: Previous definition of \""<< $2 << "\" in line " << symbol->getLine() << endl;
            }
        }
        else{
            symbol = new Symbol(programfunc_s,$2,currscopespace(),scope,yylineno,currscopeoffset());
            table.Insert(symbol);
            symbol->setiAddress(nextQuad());
            $$ = symbol;
            emit(funcstart,NULL,NULL,lvalue_exp(symbol),0,yylineno);
            functionLocalStack.push(getfunctionLocalOffset());
            enterscopespace();
            resetformalargoffset();

        }

    }
         
funcargs:  L_PARENTHESIS {scope++;} idlist R_PARENTHESIS {
    enterscopespace();
    resetfunctionlocaloffset();
} ;

funcbody: { scope--; func_open++;}funcblockstart block funcblockend {func_open--; exitscopespace(); } ;

f_def: funcprefix funcargs funcbody{ exitscopespace(); }
        
const:  INTCONST { 
                    $$ = new expr(costnum_e);
                    $$->setnumconst($1);
                }   
        | REALCONST {
                    $$ = new expr(costnum_e);
                    $$->setnumconst($1);
                }
        | STRING {
                    $$ = new expr(conststring_e);
                    $$->setstrConst($1);

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

if_stmt: if_prefix stmt {
        patchlabel($1, nextQuad());
    }
    | if_prefix stmt else_prefix stmt {
        patchlabel($1,$3+1); 
        patchlabel($3, nextQuad());        
} ;

if_prefix: IF L_PARENTHESIS exp R_PARENTHESIS{
    expr *temp = new expr(constbool_e);
    temp->setboolConst(true);
    emit(if_eq, $3, temp,NULL, nextQuad()+2 , yylineno); 
    $$ = nextQuad();
    emit(jump,NULL,NULL,NULL,0,yylineno);
} ;


else_prefix: ELSE {
    $$ = nextQuad();   
    emit(jump,NULL,NULL,NULL,0,yylineno);
} ;

loopstart : {++loopcounter;};

loopend : {--loopcounter;};

loopstmt : loopstart stmt loopend { $$ = $2; } ;

while_stmt: whilestart whilecond loopstmt{
    emit(jump,NULL,NULL,NULL,$1,yylineno); 
    patchlabel($2, nextQuad()); 
    patchlabelBC($$->getBreakList(), nextQuad()); 
    patchlabelBC($$->getContList(), $1);
} ;


whilestart: WHILE {$$=nextQuad();};

whilecond: L_PARENTHESIS exp R_PARENTHESIS  {
    expr *temp = new expr(constbool_e);
    temp->setboolConst(true);
    emit(if_eq, $2, temp, NULL, nextQuad()+2,yylineno); 
    $$ = nextQuad();
    emit(jump,NULL,NULL,NULL,0,yylineno);
};

for_prefix: FOR L_PARENTHESIS elist SEMICOLON M exp SEMICOLON{
    $$ = new forprefix($5,nextQuad());
    expr *temp = new expr(constbool_e);
    temp->setboolConst(true);
    emit(if_eq,$6,temp,NULL,0,yylineno);
} ; 

for_stmt: for_prefix N elist R_PARENTHESIS N loopstmt N{
    patchlabel($1->getEnter(),$5+1); // true jump
    patchlabel($2,nextQuad()); // false jump
    patchlabel($5,$1->getTest()); // loop jump
    patchlabel($7,$2+1); // closure jump
    
    patchlabelBC($$->getBreakList(),nextQuad());  //false jump
    patchlabelBC($$->getContList(),$2+1);  //closure jump
} ;

ret_stmt: RETURN SEMICOLON {
        if(!func_open) cout << "[Syntax Analysis] ERROR: Cannot use RETURN statement while not within function, in line " << yylineno << endl;
        emit(ret,NULL,NULL,NULL,0,yylineno);
    }
    | RETURN exp SEMICOLON {
        if(!func_open) cout << "[Syntax Analysis] ERROR: Cannot use RETURN statement while not within function, in line "<< yylineno << endl;
        emit(ret,NULL,NULL,$2,0,yylineno);
    };

N: {$$=nextQuad(); emit(jump,NULL,NULL,NULL,0,yylineno);};

M: {$$=nextQuad();};
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
