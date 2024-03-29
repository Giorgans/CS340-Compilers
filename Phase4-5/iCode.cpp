/*** --- INTEMEDIATE CODE C++ FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.3.1 , x86 chip***/

#include "iCode.h"

extern SymbolTable table;
extern int yylineno,scope;

/***   Quad related functions and variables */
unsigned int currQuad = 0;
unsigned int tempQuad = 0;
std::vector <quad> quads;
std::vector <quad> exprquads;
std::queue <unsigned> Mquads;
contbreaklists *BClist = new contbreaklists();
std::stack <unsigned> LoopCounterStack;
std::stack <unsigned> functionLocalStack;

/************************ Quad related functions and variables ************************/
void emit(iopcode op,expr *arg1,expr *arg2,expr *result,unsigned label,unsigned line){
    currQuad++;
    quads.push_back(quad(op,result,arg1,arg2,label,line));
}
void tempemit(iopcode op,expr *arg1,expr *arg2,expr *result,unsigned label,unsigned line){
    tempQuad++;
    exprquads.push_back(quad(op,result,arg1,arg2,label,line));
}

void pop_Tempemit(){
    for (int i=0 ; i<2 ; i++){
        quads.push_back(nextTempemit());
        currQuad++;
    }
}

quad nextTempemit(){
    quad temp=exprquads.at(0);
    exprquads.erase(exprquads.begin());
    return temp;
}

std::vector <quad> getexprquads(){return exprquads;}

unsigned nextMquad(){
    unsigned temp=Mquads.front();
    Mquads.pop();
    return temp;

}

unsigned int getTempQuad(){return tempQuad;}

void resetTempQuad(){tempQuad = 0;}

unsigned int nextQuad(){return currQuad;}

expr *addemits(){
    quad tempifquad = nextTempemit();
    expr *arg = tempifquad.getArg1();

    arg->changeAllTrueLabels(nextQuad());      
    emit(tempifquad.getOp(),tempifquad.getArg1(),tempifquad.getArg2(),NULL,0,yylineno);

    arg->changeAllFalseLabels(nextQuad()-1);      
    quad tempjumpquad = nextTempemit();
    emit(tempjumpquad.getOp(),tempjumpquad.getArg1(),tempjumpquad.getArg2(),NULL,0,yylineno);
    return arg;  
}


void print_quads(){

    int count=1;
    std::cout <<  "-----------------------------------------------------------------------" << std::endl;
    std::cout <<  "-----------------------------------------------------------------------" << std::endl;
    std::cout <<  "quad#\topcode\t\tresult\t\targ1\t\targ2\tlabel" << std::endl;
    std::cout <<  "-----------------------------------------------------------------------" << std::endl;
    for(std::vector <quad>::iterator i=quads.begin() ;i!=quads.end() ;i++){
        std::cout << count << ":\t" << iopcode_to_string(i->getOp()) << "\t\t";
        if(i->getResult()){
            if(!i->getResult()->getstrConst().empty())
                std::cout << i->getResult()->getstrConst() << "\t\t" ; 
            else if(i->getResult()->getType() == constbool_e){
                if(i->getResult()->getboolConst()) std::cout << "'true'\t\t";
                else std::cout << "'false'\t";
            }
            else if(i->getResult()->getType() == costnum_e)
                std::cout << i->getResult()->getnumconst() << "\t\t" ;

            else if(i->getResult()->getSymbol() )
                std::cout << i->getResult()->getSymbol()->getName() << "\t\t" ;
        }
        else std::cout << "\t\t";

        if(i->getArg1()){
            if(!i->getArg1()->getstrConst().empty())
                std::cout << i->getArg1()->getstrConst() << "\t\t" ; 
            else if(i->getArg1()->getType() == constbool_e){
                if(i->getArg1()->getboolConst()) std::cout << "'true'\t\t";
                else std::cout << "'false'\t";
            }
            else if(i->getArg1()->getType() == costnum_e)
                std::cout << i->getArg1()->getnumconst() << "\t\t" ;

            else if(i->getArg1()->getSymbol())
                std::cout << i->getArg1()->getSymbol()->getName() << "\t\t" ;
        }
        else std::cout << "\t\t";

        if(i->getArg2()){
            if(!i->getArg2()->getstrConst().empty())
                std::cout << i->getArg2()->getstrConst() << "\t" ; 
            else if(i->getArg2()->getType() == constbool_e){
                if(i->getArg2()->getboolConst()) std::cout << "'true'\t";
                else std::cout << "'false'\t";
            }
            else if(i->getArg2()->getType() == costnum_e)
                std::cout << i->getArg2()->getnumconst() << "\t" ;

            else if(i->getArg2()->getSymbol())
                std::cout << i->getArg2()->getSymbol()->getName() << "\t" ;
        }
        else std::cout << "\t";

        if(i->getLabel()>0) std::cout << i->getLabel() << "\t";
        std::cout << "\n";
        count++; 
    }
    
    std::cout <<  "-----------------------------------------------------------------------" << std::endl;
}

std::string iopcode_to_string(iopcode op)
{
    switch(op){
        case assign: return "assign";
        case add: return "add"; 
        case sub: return "sub";
        case mul: return "mul";
        case divide: return "div";
        case mod: return "mod"; 
        case uminus: return "uminus";
        case andb: return "and";
        case orb: return "or";
        case notb: return "not";
        case if_eq: return "if_eq";
        case if_noteq: return "if_not";
        case if_lesseq: return "if_leq";
        case if_greatereq: return "if_greq";
        case if_less: return "if_less"; 
        case if_greater: return "if_gr"; 
        case call: return "call";
        case param: return "param";
        case ret: return "ret";
        case getretval: return "getretval";
        case funcstart: return "funcstart";
        case funcend: return "funcend";
        case tablecreate: return "tbl_cr";
        case tablegetelem: return "tbl_get"; 
        case tablesetelem: return "tbl_set"; 
        case jump: return "jump";
        default: return "NILL";
    }
}

/*************************************************************************************/

/*************************** Expressions related functions ***************************/
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

expr *emit_iftableitem(expr *e){
    if(e->getType() != tableitem_e)
        return e;
    else{
        expr *result = new expr(var_e);
        result->insertSymbol(newtemp());
        emit(tablegetelem,e,e->getIndex(),result,0,yylineno);
        return result;
    }
}

expr *make_call(expr *lvalue,elists *elist){
    expr *func = emit_iftableitem(lvalue);
    for (std::list <expr>::iterator i = elist->getElist()->begin() ; i!=elist->getElist()->end() ; i++){
        emit(param,NULL,NULL,&*i,0,yylineno);
    }
    emit(call,NULL,NULL,func,0,yylineno);
    expr *result = new expr(var_e);
    result->insertSymbol(newtemp());
    emit(getretval,NULL,NULL,result,0,yylineno);
    return result;
}

expr *member_item(expr *lvalue,std::string name){
    lvalue = emit_iftableitem(lvalue);
    expr *item = new expr(tableitem_e);
    item->insertSymbol(lvalue->getSymbol());
    expr *temp = new expr(conststring_e);
    temp->setstrConst(name);
    item->setIndex(temp);
    return item;
}

void checkuminus(expr *e){
    if(e->getType()==constbool_e   || 
       e->getType()==conststring_e || 
       e->getType()==nil_e         || 
       e->getType()==newtable_e    || 
       e->getType()==programfunc_e || 
       e->getType()==libraryfunc_e || 
       e->getType()==boolexp_e )
       std::cout << "[Intemediate Code] ERROR: Illegal expression to unary - , in line "<< yylineno << std::endl;

       //comperror("Illegal expression to unary -");

}
/*************************************************************************************/

/****************************** Label related functions ******************************/
void backpatch(unsigned savedlabel, unsigned quadlabel){
    if(!exprquads.at(quadlabel).getLabel())quads.at(quadlabel).setLabel(savedlabel+1);
}
void backpatchquads(unsigned savedlabel, unsigned quadlabel){
    if(!quads.at(quadlabel).getLabel())quads.at(quadlabel).setLabel(savedlabel+1);
}

void patchlabel(unsigned quad, unsigned label){
	assert(quad < currQuad);
    quads.at(quad).setLabel(label);
}

void patchlabelBC(std::vector <unsigned> list, unsigned label){
    for(int i=0 ; i<list.size() ; i++)
        quads.at(list.at(i)).setLabel(label);
}

std::vector <unsigned> merge(std::vector <unsigned> a,std::vector <unsigned> b){
    std::vector <unsigned> temp;
    temp.reserve(a.size() + b.size());
    temp.insert(temp.end() , a.begin() , a.end() );
    temp.insert(temp.end() , b.begin() , b.end() );
    return temp;
}
/*************************************************************************************/


/******************* Scope space related functions and variables ********************/
unsigned scopeSpaceCounter = 1;

scopespace_t currscopespace(void){
    if(scopeSpaceCounter == 1) return programvar;
    else if(!(scopeSpaceCounter % 2)) return formalarg;
    else return functionlocal;
}

void enterscopespace(void){++scopeSpaceCounter;}
void exitscopespace(void){assert(scopeSpaceCounter>1);  --scopeSpaceCounter;}
/*************************************************************************************/

/********************** Offset related functions and variables ***********************/
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

void resetformalargoffset(void){formalArgOffset = 0;}
void resetfunctionlocaloffset(void){functionLocalOffset = 0;}

unsigned getprogramVarOffset(){return programVarOffset; }
unsigned getfunctionLocalOffset(){return functionLocalOffset;}
unsigned getformalArgOffset(){return formalArgOffset;}

void setprogramVarOffset(unsigned offset){programVarOffset=offset;}
void setfunctionLocalOffset(unsigned offset){functionLocalOffset=offset;}
void setformalArgOffset(unsigned offset){formalArgOffset=offset;}

/*************************************************************************************/

/********************* Temp hidden variable related functions ***********************/
int tempcounter = 0;

std::string newtempname() { return "@t" + std::to_string(tempcounter); } 

Symbol *newtemp() {
    std::string name = newtempname();
    tempcounter++;
    Symbol *sym = table.LookupScope(name,scope); 
    if (sym == NULL){
        sym = new Symbol(var_s,name,currscopespace(),scope,yylineno,currscopeoffset());
        table.Insert(sym);
        return sym;
    } 
    else return sym;
}
void resettemp() { tempcounter = 0; } 

bool istempname(std::string s){return s.at(0)=='@';}
bool istempexpr(expr *e){return (e->getSymbol()) && (e->getSymbol()->getType() == var_s) && (istempname(e->getSymbol()->getName()));}
/*************************************************************************************/