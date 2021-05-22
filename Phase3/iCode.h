/*** --- INTEMEDIATE CODE HEADER FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.3.1 , x86 chip ***/

#include "SymbolTable_v2.h"
#include <vector>
#include <list>
#include <stack>
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
        string strConst;
        bool boolConst;
        vector <unsigned> truelist,falselist;
    public:
        exp_t getType(){return this->type;}
        Symbol *getSymbol(){return this->sym;}
        void insertSymbol(Symbol *sym){this->sym=sym;}
        double getnumconst(){return numConst;}
        void setnumconst(double numConst){this->numConst=numConst;}
        string getstrConst(){return strConst;}
        void setstrConst(string strConst){this->strConst=strConst;}
        bool getboolConst(){return boolConst;}
        void setboolConst(bool boolConst){this->boolConst=boolConst;}
        expr *getIndex(){return this->index;}
        void setIndex(expr *index){this->index=index;}
        vector <unsigned> getTrueList(){return truelist;}
        void setTrueList(vector <unsigned> truelist){this->truelist=truelist;}
        void insertTrueLabel(unsigned label){this->truelist.push_back(label);}
        vector <unsigned> getFalseList(){return falselist;}
        void setFalseList(vector <unsigned> falselist){this->falselist=falselist;}
        void insertFalseLabel(unsigned label){this->falselist.push_back(label);}
        expr(exp_t type){
            this->type=type;

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
    public:
        iopcode getOp(){return this->op;}
        expr *getResult(){return this->result;}
        expr *getArg1(){return this->arg1;}
        expr *getArg2(){return this->arg2;}
        unsigned getLabel(){return this->label;}
        void setLabel(unsigned label){this->label=label;}
        unsigned getLine(){return this->line;}
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
        list <expr> *elist;
    public:
        list <expr> *getElist(){return this->elist;}
        void insertElistItem(expr *e){this->elist->push_back(*e);}
        void pushfrontElistItem(expr *e){this->elist->push_front(*e);}
    elists(){this->elist = new list<expr>();}
    elists(expr *e){
        this->elist = new list<expr>();
        this->elist->push_back(*e);
    }
}elists;

typedef class calls{
    private:
        string name;
        bool method;
        elists *elist;
    public:
        string getName(){return this->name;}
        bool getMethod(){return this->method;}
        elists *getElist(){return this->elist;}
    calls(string name,bool method,elists *elist){
        this->name=name;
        this->method=method;
        this->elist=elist;
    }
}calls;

typedef class contbreaklists {
    private:
        vector <unsigned> breaklist;
        vector <unsigned> contlist;
    public:
        vector <unsigned> getBreakList(){return this->breaklist;}
        vector <unsigned> getContList(){return this->contlist;}
        void setBreakList(vector <unsigned> breaklist) {this->breaklist=breaklist;}
        void setContList(vector <unsigned> contlist) {this->contlist=contlist;}
        void insertBreakList(unsigned label) {this->breaklist.push_back(label);}
        void insertContList(unsigned label) {this->contlist.push_back(label);}
    contbreaklists(){};
}contbreaklists;


/* Quad related functions and variables*/
void emit(iopcode op,expr *arg1,expr *arg2,expr *result,unsigned label,unsigned line);
void emit_stack(quad tquad);
void tempemit();
unsigned int getTempQuad();
void resetTempQuad();
unsigned int nextQuad();
string iopcode_to_string(iopcode op);
void print_quads();
/************************************************/

/* Expressions related functions */
expr *lvalue_exp(Symbol *sym);
expr *emit_iftableitem(expr *e);
expr *member_item(expr *lvalue,string name);
expr *make_call(expr *lvalue,elists *elist);
void checkuminus(expr *e);
/************************************************/

/* Label related functions */
void backpatch(unsigned savedlabel, unsigned quadlabel);
void backpatchLabels(vector <unsigned> list, unsigned label);
void patchlabel(unsigned quad, unsigned label);
void patchlabelBC(vector <unsigned> list, unsigned label);
vector <unsigned> merge(vector <unsigned> a,vector <unsigned> b);
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
/************************************************/

/*Temp hidden variable related functions*/
string newtempname();
Symbol *newtemp();
void resettemp();
bool istempname(char* s);
bool istempexpr(expr *e);
/************************************************/

