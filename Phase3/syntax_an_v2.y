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
bool partial=false;

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
    if($$==NULL){
        $$ = new contbreaklists();
        $1 = new contbreaklists();
    }
    //$$->setBreakList(merge($1->getBreakList(),$2->getBreakList()));
    //$$->setContList(merge($1->getContList(),$2->getContList()));
} |  ;

stmt:  exp SEMICOLON  {
            resettemp();
        expr *temp=new expr(constbool_e);
        temp->setboolConst(false);
        if($1->getType()==boolexp_e){
            /*Backpatching falselist*/
            for(int i=0 ;i<$1->getFalseList().size() ; i++)
                backpatch(nextQuad(),$1->getFalseList().at(i));
        
            emit(assign,temp,NULL,$1,0,yylineno);
            emit(jump,NULL,NULL,NULL,nextQuad()+3,yylineno);
        
            /*Backpatching truelist*/
            for(int i=0 ;i<$1->getTrueList().size() ; i++)
                backpatch(nextQuad(),$1->getTrueList().at(i));
            
            temp=new expr(constbool_e);
            temp->setboolConst(true);
            emit(assign,temp,NULL,$1,0,yylineno); 
        }
    }
      |  if_stmt {
            $$ = new contbreaklists();
            //$$->setBreakList(merge($$->getBreakList(),$1->getBreakList()));
            //$$->setContList(merge($$->getContList(),$1->getContList()));

      }
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
      | block {  $$ = $1;}
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
        
        $$->insertTrueLabel(nextQuad());
        emit(if_less,$1,$3,NULL ,0, yylineno);
 
        $$->insertFalseLabel(nextQuad());
        emit(jump,NULL,NULL,NULL,0,yylineno);

    }
    | exp GREATER_THAN exp{
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
       
        $$->insertTrueLabel(nextQuad());
        emit(if_greater,$1,$3,NULL ,0, yylineno);
        
        $$->insertFalseLabel(nextQuad());
        emit(jump,NULL,NULL,NULL,0,yylineno);

    }
    | exp LESS_EQUAL exp{
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
        
        $$->insertTrueLabel(nextQuad());
        emit(if_lesseq,$1,$3,NULL ,0, yylineno);
        
        $$->insertFalseLabel(nextQuad());
        emit(jump,NULL,NULL,NULL,0,yylineno);

    }
    | exp GREATER_EQUAL exp {
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
        
        $$->insertTrueLabel(nextQuad());
        emit(if_greatereq,$1,$3,NULL ,0, yylineno);
        
        $$->insertFalseLabel(nextQuad());
        emit(jump,NULL,NULL,NULL,0,yylineno);

    }
    | exp AND M exp {
        /*cout << "\nM value: " << $3 ; 
        cout << "\nAND E1 TRUE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$1->getTrueList().size() ; i++ )
            cout <<  $1->getTrueList().at(i);
        cout << "\nAND E2 TRUE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$4->getTrueList().size() ; i++ )
            cout <<  $4->getTrueList().at(i);
	    
        cout << "\nAND E1 FALSE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$1->getFalseList().size() ; i++ )
            cout <<  $1->getFalseList().at(i);
        cout << "\nAND E2 FALSE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$4->getFalseList().size() ; i++ )
            cout <<  $4->getFalseList().at(i);
        cout << "\n";*/
        
        for(int i=0 ;i<$1->getTrueList().size() ; i++)
            backpatch($3,$1->getTrueList().at(i));
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
            $$->setTrueList($4->getTrueList());
            $$->setFalseList(merge($1->getFalseList(),$4->getFalseList() ));
        /*cout << "\nAND E TRUE LIST" << "\t" ;
	    for(unsigned i=0 ; i<$$->getTrueList().size() ; i++ )
            cout <<  $$->getTrueList().at(i) << " ";
        cout << "\nAND E FALSE LIST" << "\t" ;
	    for(unsigned i=0 ; i<$$->getFalseList().size() ; i++ )
            cout <<  $$->getFalseList().at(i) << " ";*/
    }
    | exp OR M exp {
        /*cout << "\n\nM value: " << $3 ; 
        cout << "\nOR E1 TRUE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$1->getTrueList().size() ; i++ )
            cout <<  $1->getTrueList().at(i);
        cout << "\nOR E2 TRUE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$4->getTrueList().size() ; i++ )
            cout <<  $4->getTrueList().at(i);
	    
        cout << "\nOR E1 FALSE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$1->getFalseList().size() ; i++ )
            cout <<  $1->getFalseList().at(i);
        cout << "\nOR E2 FALSE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$4->getFalseList().size() ; i++ )
            cout <<  $4->getFalseList().at(i);*/

        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
        for(int i=0 ;i<$1->getFalseList().size() ; i++)
            backpatch($3,$1->getFalseList().at(i));
        
        $$->setTrueList(merge($1->getTrueList(),$4->getTrueList()));
        $$->setFalseList($4->getFalseList());
        
        /*cout << "\nOR E TRUE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$$->getTrueList().size() ; i++ )
            cout <<  $$->getTrueList().at(i) << " ";
        cout << "\nOR E FALSE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$$->getFalseList().size() ; i++ )
            cout <<  $$->getFalseList().at(i) << " \n";*/
    }
    | exp EQUAL exp{
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
       
        $$->insertTrueLabel(nextQuad());
        emit(if_eq,$1,$3,NULL ,0, yylineno);
        
        $$->insertFalseLabel(nextQuad());
        emit(jump,NULL,NULL,NULL,0,yylineno);
    }
    | exp NOT_EQUAL exp{
        $$ = new expr(boolexp_e);
        $$->insertSymbol(newtemp());
       
        $$->insertTrueLabel(nextQuad());
        emit(if_noteq,$1,$3,NULL ,0, yylineno);
        
        $$->insertFalseLabel(nextQuad());
        emit(jump,NULL,NULL,NULL,0,yylineno);
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
        
        if($3->getType()==boolexp_e){
            expr *temp=new expr(constbool_e);
            temp->setboolConst(false);
        
            /*Backpatching falselist*/
            for(int i=0 ;i<$3->getFalseList().size() ; i++)
                backpatch(nextQuad(),$3->getFalseList().at(i));
        
            emit(assign,temp,NULL,$3,0,yylineno);
            emit(jump,NULL,NULL,NULL,nextQuad()+3,yylineno);
        
            /*Backpatching truelist*/
            for(int i=0 ;i<$3->getTrueList().size() ; i++)
                backpatch(nextQuad(),$3->getTrueList().at(i));

            temp=new expr(constbool_e);
            temp->setboolConst(true);
            emit(assign,temp,NULL,$3,0,yylineno); 
        }

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
                symbol = new Symbol(var_s,$1,currscopespace(),scope,yylineno,currscopeoffset());
                table.Insert(symbol);
                $$ = lvalue_exp(symbol);
            }
            else{
                symbol=table.LookupScope($1,temp_scope);
                if(temp!=NULL && temp->IsActive() && !loop_open && func_open && scope>temp_scope && !isMember && temp->getType()==var_s)
                    cout << "[Syntax Analysis] ERROR: Cannot access \"" << $1 << "\", in line " << yylineno << endl;
                $$ = lvalue_exp(symbol);
            }
            
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

block: L_BRACE {scope++;} stmts   R_BRACE {table.Hide(scope); scope--; $$ = $3;};

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
        cout << "IFPREFIX ONLY: " << $1 << endl;
        patchlabel($1-1, nextQuad()+1);
        patchlabel($1+1, nextQuad()+1);
    }
    | if_prefix stmt else_prefix stmt {
        patchlabel($1-2,$3-1); //if eq if_prefix
        patchlabel($1-1,$3+2); //jmp if_prefix
        patchlabel($3, nextQuad()+1); // jmp to end     
} ;

if_prefix: IF L_PARENTHESIS exp R_PARENTHESIS{
    expr *temp = new expr(constbool_e);
    temp->setboolConst(true);
    if($3->getType()==boolexp_e){
        //////////////////////////////
        expr *temp=new expr(constbool_e);
        temp->setboolConst(false);
        
        /*Backpatching falselist*/
        for(int i=0 ;i<$3->getFalseList().size() ; i++)
            backpatch(nextQuad(),$3->getFalseList().at(i));
        
        emit(assign,temp,NULL,$3,0,yylineno);
        emit(jump,NULL,NULL,NULL,nextQuad()+3,yylineno);
        
        /*Backpatching truelist*/
        for(int i=0 ;i<$3->getTrueList().size() ; i++)
            backpatch(nextQuad(),$3->getTrueList().at(i));

        temp=new expr(constbool_e);
        temp->setboolConst(true);
        emit(assign,temp,NULL,$3,0,yylineno); 
        //////////////////////////////
        emit(if_eq, $3, temp, NULL, 0, yylineno);
        emit(jump,NULL,NULL,NULL,0,yylineno);
    }

    $$ = nextQuad();

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

  //table.printSymbols();
  print_quads();


  return 0; 
}
