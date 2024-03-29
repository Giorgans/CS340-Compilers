/*** --- INTEMEDIATE CODE HEADER FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.4 , x86 chip ***/
#ifndef _ICODE_H_
#define _ICODE_H_

#include "SymbolTable_v2.h"
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <algorithm>
#include <iterator>


/* Opcode type for Quad class*/
typedef enum iopcode {
    assign, add, sub,
    mul, divide, mod,
    uminus, andb, orb,
    notb, if_eq, if_noteq,
    if_lesseq, if_greatereq, if_less,
    if_greater, call, param,
    ret, getretval, funcstart,
    funcend, tablecreate,
    tablegetelem, tablesetelem,jump
}iopcode;

/* Types for the expression class*/
typedef enum exp_t {
    var_e,
    tableitem_e,

    programfunc_e,
    libraryfunc_e,

    arithexp_e,
    boolexp_e,
    assignexp_e,
    newtable_e,

    costnum_e,
    constbool_e,
    conststring_e,

    nil_e
}exp_t;

/* Expression class being a possible part for a Quad*/
typedef class expr{
    private:
        exp_t type;
        Symbol *sym;
        expr *index;
        double numConst;
        std::string strConst;
        bool boolConst;
        std::vector <unsigned> truelist,falselist;
        bool notGate;
        bool andGate;
    public:
        exp_t getType(){return this->type;}
        Symbol *getSymbol(){return this->sym;}
        void insertSymbol(Symbol *sym){this->sym=sym;}
        double getnumconst(){return numConst;}
        void setnumconst(double numConst){this->numConst=numConst;}
        std::string getstrConst(){return strConst;}
        void setstrConst(std::string strConst){this->strConst=strConst;}
        bool getboolConst(){return boolConst;}
        void setboolConst(bool boolConst){this->boolConst=boolConst;}
        expr *getIndex(){return this->index;}
        void setIndex(expr *index){this->index=index;}
        std::vector <unsigned> getTrueList(){return truelist;}
        void setTrueList(std::vector <unsigned> truelist){this->truelist=truelist;}
        void insertTrueLabel(unsigned label){this->truelist.push_back(label);}
        void changeAllTrueLabels(unsigned value){
            for(int i = 0 ; i!=this->truelist.size() ; i++)
                this->truelist[i]+=value;
        }
        std::vector <unsigned> getFalseList(){return falselist;}
        void setFalseList(std::vector <unsigned> falselist){this->falselist=falselist;}
        void insertFalseLabel(unsigned label){this->falselist.push_back(label);}
        void changeAllFalseLabels(unsigned value){
            for(int i = 0 ; i!=this->falselist.size() ; i++)
                this->falselist[i]+=value;
        }
        
        bool getNotGate(){return this->notGate;}
        void setNotGate(bool notGate){this->notGate=notGate;}
        bool getAndGate(){return this->andGate;}
        void setAndGate(bool andGate){this->andGate=andGate;}
        expr(exp_t type){
            this->type=type;
            this->notGate=false;
            this->andGate=false;


        }
}expr;

/* Quad class cointaining a operation code , two arguments and the result as expressions*/
typedef class quad{
    private:
        iopcode op;
        expr *result;
        expr *arg1;
        expr *arg2;
        unsigned label;
        unsigned line;
        unsigned taddress;
    public:
        iopcode getOp(){return this->op;}
        expr *getResult(){return this->result;}
        expr *getArg1(){return this->arg1;}
        expr *getArg2(){return this->arg2;}
        unsigned getLabel(){return this->label;}
        void setLabel(unsigned label){this->label=label;}
        unsigned getLine(){ return this->line;}
        unsigned getTaddress(){ return this->taddress; }
        void setTaddress(unsigned taddress){ this->taddress=taddress; }
        quad(iopcode op,expr *result,expr *arg1,expr *arg2,unsigned label,unsigned line){
            this->op=op;
            this->result=result;
            this->arg1=arg1;
            this->arg2=arg2;
            this->label=label;
            this->line=line;
        }
}quad;

/* FOR PREFIX class to enter */
typedef class forprefix{
    private:
	    unsigned test;
	    unsigned enter;
    public:
        unsigned getTest(){return this->test;}
        unsigned getEnter(){return this->enter;}
    forprefix(unsigned test,unsigned enter){
        this->test=test;
        this->enter=enter;
    }
}forprefix;

typedef class elists{
    private:
        std::list <expr> *elist;
    public:
        std::list <expr> *getElist(){return this->elist;}
        void insertElistItem(expr *e){this->elist->push_back(*e);}
        void pushfrontElistItem(expr *e){this->elist->push_front(*e);}
    elists(){this->elist = new std::list<expr>();}
    elists(expr *e){
        this->elist = new std::list<expr>();
        this->elist->push_back(*e);
    }
}elists;

typedef class calls{
    private:
        std::string name;
        bool method;
        elists *elist;
    public:
        std::string getName(){return this->name;}
        bool getMethod(){return this->method;}
        elists *getElist(){return this->elist;}
    calls(std::string name,bool method,elists *elist){
        this->name=name;
        this->method=method;
        this->elist=elist;
    }
}calls;

typedef class contbreaklists {
    private:
        std::vector <unsigned> breaklist;
        std::vector <unsigned> contlist;
    public:
        std::vector <unsigned> getBreakList(){return this->breaklist;}
        std::vector <unsigned> getContList(){return this->contlist;}
        void setBreakList(std::vector <unsigned> breaklist) {this->breaklist=breaklist;}
        void setContList(std::vector <unsigned> contlist) {this->contlist=contlist;}
        void insertBreakList(unsigned label) {this->breaklist.push_back(label);}
        void insertContList(unsigned label) {this->contlist.push_back(label);}
    contbreaklists(){};
}contbreaklists;

typedef class indexedelements{
    private:
        expr *index;
        expr *value;
    public:
        expr *getIndexElement(){return this->index;}
        expr *getValueElement(){return this->value;}
    indexedelements(expr *index,expr *value){
        this->index=index;
        this->value=value;
    }
}indexedelements;

typedef class indexedlist{
    private:
        std::list <indexedelements> *indexlist;
    public:
        std::list <indexedelements> *getIndexedList(){return this->indexlist;}
        void insertElements(indexedelements *elements){this->indexlist->push_back(*elements);}
    indexedlist(){
        this->indexlist = new std::list<indexedelements>();
    }
}indexedlist;



/* Quad related functions and variables*/
void emit(iopcode op,expr *arg1,expr *arg2,expr *result,unsigned label,unsigned line);
void emit_stack(quad tquad);
void tempemit(iopcode op,expr *arg1,expr *arg2,expr *result,unsigned label,unsigned line);
expr *addemits();
quad nextTempemit();
void pop_Tempemit();
unsigned nextMquad();
unsigned int getTempQuad();
void resetTempQuad();
std::vector <quad> getexprquads();
unsigned int nextQuad();
std::string iopcode_to_string(iopcode op);
void print_quads();
/************************************************/

/* Expressions related functions */
expr *lvalue_exp(Symbol *sym);
expr *emit_iftableitem(expr *e);
expr *member_item(expr *lvalue,std::string name);
expr *make_call(expr *lvalue,elists *elist);
void checkuminus(expr *e);
/************************************************/

/* Label related functions */
void backpatch(unsigned savedlabel, unsigned quadlabel);
void backpatchquads(unsigned savedlabel, unsigned quadlabel);
void patchlabel(unsigned quad, unsigned label);
void patchlabelBC(std::vector <unsigned> list, unsigned label);
std::vector <unsigned> merge(std::vector <unsigned> a,std::vector <unsigned> b);
/************************************************/
/* Scope space related functions */
scopespace_t currscopespace(void);
void enterscopespace(void);
void exitscopespace(void);
/************************************************/

/****   Offset related functions               */
unsigned currscopeoffset(void);
void inccurrscopeoffset(void);
void resetformalargoffset(void);
void resetfunctionlocaloffset(void);
unsigned getprogramVarOffset();
unsigned getfunctionLocalOffset();
unsigned getformalArgOffset();
void setprogramVarOffset(unsigned offset);
void setfunctionLocalOffset(unsigned offset);
void setformalArgOffset(unsigned offset);
/************************************************/

/*Temp hidden variable related functions*/
std::string newtempname();
Symbol *newtemp();
void resettemp();
bool istempname(char* s);
bool istempexpr(expr *e);
/************************************************/

#endif