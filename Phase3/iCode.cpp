/*** --- INTEMEDIATE CODE C++ FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.2.3 , x86 chip***/
#include "iCode.h"

void emit(iopcode op,exp *arg1,exp *arg2,exp *result,unsigned label,unsigned line){
    if(currQuad==total) expand();
   
    quad *p = quads+currQuad++;
    p = new quad(op,arg1,arg2,result,label,line);
}

exp *emit_iftableitem(exp *e){
    if(e->getType() != tableitem_e)
        return e;
    else{
        exp *result = new exp(var_e);
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

scopespace_t currscopespace(void){
    if(scopeSpaceCounter == 1) return programvar;
    else if(!(scopeSpaceCounter % 2)) return formalarg;
    else functionlocal;
}

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

void enterscopespace(void){++scopeSpaceCounter;}
void exitscopespace(void){assert(scopeSpaceCounter>1);  --scopeSpaceCounter;}
