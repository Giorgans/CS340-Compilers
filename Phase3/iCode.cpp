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
unsigned int tempVar = 0;
vector <quad> quads;
stack <quad> quadStack;
contbreaklists *BClist = new contbreaklists();
stack <unsigned> LoopCounterStack;
stack <unsigned> functionLocalStack;

/************************ Quad related functions and variables ************************/
void emit(iopcode op,expr *arg1,expr *arg2,expr *result,unsigned label,unsigned line){
    currQuad++;
    quads.push_back(quad(op,result,arg1,arg2,label,line));
}

void emit_stack(quad tquad){
    quadStack.push(tquad);
}


unsigned int nextQuad(){return currQuad;}

void print_quads(){

    int count=1;
    cout <<  "-----------------------------------------------------------------------" << endl;
    cout <<  "-----------------------------------------------------------------------" << endl;
    cout <<  "quad#\topcode\t\tresult\t\targ1\t\targ2\tlabel" << endl;
    cout <<  "-----------------------------------------------------------------------" << endl;
    for(vector <quad>::iterator i=quads.begin() ;i!=quads.end() ;i++){
        cout << count << ":\t" << iopcode_to_string(i->getOp()) << "\t\t";
        if(i->getResult()){
            if(!i->getResult()->getstrConst().empty())
                cout << i->getResult()->getstrConst() << "\t\t" ; 
            else if(i->getResult()->getType() == constbool_e){
                if(i->getResult()->getboolConst()) cout << "'true'\t\t";
                else cout << "'false'\t";
            }
            else if(i->getResult()->getType() == costnum_e)
                cout << i->getResult()->getnumconst() << "\t\t" ;

            else if(i->getResult()->getSymbol() )
                cout << i->getResult()->getSymbol()->getName() << "\t\t" ;
        }
        else cout << "\t\t";

        if(i->getArg1()){
            if(!i->getArg1()->getstrConst().empty())
                cout << i->getArg1()->getstrConst() << "\t\t" ; 
            else if(i->getArg1()->getType() == constbool_e){
                if(i->getArg1()->getboolConst()) cout << "'true'\t\t";
                else cout << "'false'\t";
            }
            else if(i->getArg1()->getType() == costnum_e)
                cout << i->getArg1()->getnumconst() << "\t\t" ;

            else if(i->getArg1()->getSymbol())
                cout << i->getArg1()->getSymbol()->getName() << "\t\t" ;
        }
        else cout << "\t\t";

        if(i->getArg2()){
            if(!i->getArg2()->getstrConst().empty())
                cout << i->getArg2()->getstrConst() << "\t" ; 
            else if(i->getArg2()->getType() == constbool_e){
                if(i->getArg2()->getboolConst()) cout << "'true'\t";
                else cout << "'false'\t";
            }
            else if(i->getArg2()->getType() == costnum_e)
                cout << i->getArg2()->getnumconst() << "\t" ;

            else if(i->getArg2()->getSymbol())
                cout << i->getArg2()->getSymbol()->getName() << "\t" ;
        }
        else cout << "\t";

        if(i->getLabel()>0) cout << i->getLabel() << "\t";
        cout << "\n";
        count++; 
    }
    
    cout <<  "-----------------------------------------------------------------------" << endl;
}

string iopcode_to_string(iopcode op)
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
        case tablecreate: return "table_cr";
        case tablegetelem: return "table_get"; 
        case tablesetelem: return "table_set"; 
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
        emit(tablegetelem,e,e->getIndex(),NULL,0,yylineno);
        return result;
    }
}

expr *make_call(expr *lvalue,elists *elist){
    expr *func = emit_iftableitem(lvalue);
    for (list <expr>::iterator i = elist->getElist()->begin() ; i!=elist->getElist()->end() ; i++)
        emit(param,NULL,NULL,&*i,0,yylineno);
    emit(call,NULL,NULL,func,0,yylineno);
    expr *result = new expr(var_e);
    result->insertSymbol(newtemp());
    emit(getretval,NULL,NULL,result,0,yylineno);
    return result;
}

expr *member_item(expr *lvalue,string name){
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
        cout << "[Intemediate Code] ERROR: Illegal expression to unary - , in line "<< yylineno << endl;

       //comperror("Illegal expression to unary -");

}
/*************************************************************************************/

/****************************** Label related functions ******************************/
void backpatch(unsigned savedlabel, unsigned quadlabel){
    if(!quads.at(quadlabel).getLabel())quads.at(quadlabel).setLabel(savedlabel+1);
}

void patchlabel(unsigned quad, unsigned label){
	assert(quad < currQuad);
    quads.at(quad).setLabel(label);
}

void patchlabelBC(vector <unsigned> list, unsigned label){
    for(vector <unsigned>::iterator i=list.begin() ; i!=list.end() ; i++)
        quads.at(*i).setLabel(label);
}

vector <unsigned> merge(vector <unsigned> a,vector <unsigned> b){
    vector <unsigned> temp;
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
/*************************************************************************************/

/********************* Temp hidden variable related functions ***********************/
int tempcounter = 0;

string newtempname() { return "@t" + to_string(tempcounter); } 

Symbol *newtemp() {
    string name = newtempname();
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

bool istempname(string s){return s.at(0)=='@';}
bool istempexpr(expr *e){return (e->getSymbol()) && (e->getSymbol()->getType() == var_s) && (istempname(e->getSymbol()->getName()));}
/*************************************************************************************/