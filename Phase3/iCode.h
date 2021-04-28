/*** --- INTEMEDIATE CODE HEADER FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.2.3 , x86 chip***/
#include "SymbolTable_v2.h"

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

typedef class expr{
    private:
        exp_t type;
        Symbol *sym;
        expr *index;
        double numConst;
        string strConst;
        bool boolConst;
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
        
        expr(exp_t type){
            this->type=type;
        }
}expr;

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

scopespace_t currscopespace(void);
unsigned currscopeoffset(void);
void inccurrscopeoffset(void);
void enterscopespace(void);
void exitscopespace(void);

expr *lvalue_exp(Symbol *sym);

void emit(iopcode op,expr *arg1,expr *arg2,expr *result,unsigned label,unsigned line);

expr *emit_iftableitem(expr *e);

void expand(void);

bool istempname(char* s);
bool istempexpr(expr *e);

