/*** --- INTEMEDIATE CODE HEADER FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.2.3 , x86 chip***/
#include "SymbolTable_v2.h"
#define EXPAND_SIZE 1024
#define CURR_SIZE (total*sizeof(quad))
#define NEW_SIZE (EXPAND_SIZE*sizeof(quad)+CURR_SIZE)
quad *quads = (quad*) 0;
unsigned total = 0;
unsigned int currQuad=0;

typedef enum iopcode {
    assign, add, sub,
    mul, div, mod,
    uminus, andb, orb,
    notb, if_eq, if_noteq,
    if_lesseq, if_greatereq, if_less,
    if_greater, call, param,
    ret, getretval, funcstart,
    funcend, tablecreate,
    tablegetelem, tablesetelem
}iopcode;

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

typedef class exp{
    private:
        exp_t type;
        Symbol *sym;
        exp *index;
        double numConst;
        string strConst;
        unsigned char boolConst;
    public:
        exp_t getType(){return this->type;}
        Symbol *getSymbol(){return this->sym;}
        exp *getIndex(){return this->index;}
        exp(exp_t type){this->type=type;}
}exp;

typedef class quad{
    private:
        iopcode op;
        exp *result;
        exp *arg1;
        exp *arg2;
        unsigned label;
        unsigned line;
    public:
        iopcode getOp(){return this->op;}
        exp *getResult(){return this->result;}
        exp *getArg1(){return this->arg1;}
        exp *getArg2(){return this->arg2;}
        unsigned getLabel(){return this->label;}
        unsigned getLine(){return this->line;}
        quad(iopcode op,exp *result,exp *arg1,exp *arg2,unsigned label,unsigned line){
            this->op=op;
            this->result=result;
            this->arg1=arg1;
            this->arg2=arg2;
            this->label=label;
            this->line=line;
        }
}quad;


typedef enum scopespace_t{programvar,functionlocal,formalarg}scopespace_t;

typedef enum symbol_t{var_s,programfunc_s,libraryfunc_s}symbol_t;

unsigned programVarOffset = 0;
unsigned functionLocalOffset = 0;
unsigned formalArgOffset = 0;
unsigned scopeSpaceCounter = 1;

scopespace_t currscopespace(void);
unsigned currscopeoffset(void);
void inccurrscopeoffset(void);
void enterscopespace(void);
void exitscopespace(void);

void emit(iopcode op,exp *arg1,exp *arg2,exp *result,unsigned label,unsigned line);

exp *emit_iftableitem(exp *e);

void expand(void);
