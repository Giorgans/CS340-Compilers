/*** --- INTEMEDIATE CODE C++ FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.2.3 , x86 chip***/
#include "iCode.h"
#define EXPAND_SIZE 1024
#define CURR_SIZE (total*sizeof(quad))
#define NEW_SIZE (EXPAND_SIZE*sizeof(quad)+CURR_SIZE)
extern SymbolTable table;
extern int yylineno,scope;

/* Quad related functions and variables*/
unsigned total = 0;
unsigned int currQuad = 0;
unsigned int tempVar = 0;
quad *quads = (quad*) 0;

void emit(iopcode op,expr *arg1,expr *arg2,expr *result,unsigned label,unsigned line){
    if(currQuad==total) expand();
   
    quad *p = quads+currQuad++;
    p = new quad(op,arg1,arg2,result,label,line);
}

expr *emit_iftableitem(expr *e){
    if(e->getType() != tableitem_e)
        return e;
    else{
        expr *result = NULL;//new expr(var_e);
        //emit(tablegetelem,e,e->getIndex(),label,line);
        return result;
    }
}

void expand(void){
    assert(total==currQuad);
    quad *p = (quad*) malloc(NEW_SIZE);
    if(quads){
        memcpy(p,quads,CURR_SIZE);
        free(quads);
    }
    quads = p;
    total += EXPAND_SIZE;
}
/************************************************/

/* Expression related functions */
expr *lvalue_exp(Symbol *sym){
    assert(sym);
    expr *e;
    switch(sym->getType()){
        case var_s :   e = new expr(var_e); break;
        case programfunc_s :   e = new expr(programfunc_e); break;
        case libraryfunc_s :   e = new expr(libraryfunc_e); break;
        default: assert(0);
    }
    e->insertSymbol(sym);
    return e;
}
/************************************************/

/* Scope space related functions and variables */
unsigned scopeSpaceCounter = 1;

scopespace_t currscopespace(void){
    if(scopeSpaceCounter == 1) return programvar;
    else if(!(scopeSpaceCounter % 2)) return formalarg;
    else return functionlocal;
}

void enterscopespace(void){++scopeSpaceCounter;}
void exitscopespace(void){assert(scopeSpaceCounter>1);  --scopeSpaceCounter;}
/************************************************/

/*   Offset related functions and variables     */
unsigned programVarOffset = 0;
unsigned functionLocalOffset = 0;
unsigned formalArgOffset = 0;

unsigned currscopeoffset(void){
    switch(currscopespace()){
        case programvar : return programVarOffset;
        case functionlocal : return functionLocalOffset;
        case formalarg : return formalArgOffset;
        default : assert(0);
    }
}

void inccurrscopeoffset(void){
        switch(currscopespace()){
        case programvar : ++programVarOffset; break;
        case functionlocal : ++functionLocalOffset; break;
        case formalarg : ++formalArgOffset; break;
        default : assert(0);
    }
}
/************************************************/


/*Temp hidden variable related functions*/
int tempcounter = 0;

string newtempname() { return "@t" + to_string(tempcounter); } 

void resettemp() { tempcounter = 0; } 

Symbol *newtemp() {
    string name = newtempname();
    Symbol *sym = table.LookupScope(name,scope); 
    if (sym == NULL) return new Symbol(var_s,name,currscopespace(),scope,yylineno,currscopeoffset());
    else return sym;
}

bool istempname(string s){return s.at(0)=='@';}
bool istempexpr(expr *e){return (e->getSymbol()) && (e->getSymbol()->getType() == var_s) && (istempname(e->getSymbol()->getName()));}


/************************************************/

