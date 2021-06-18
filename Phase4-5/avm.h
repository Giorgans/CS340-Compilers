/*** --- AVM HEADER FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.4 , x86 chip ***/
#include "iCode.h"

enum vmopcode{
    assign_v,   add_v,  sub_v,
    mul_v,  div_v,  mod_v,
    uminus_v,   and_v,  or_v,
    not_v,  jeq_v,  jne_v,
    jle_v,  jge_v,  jlt_v,
    jgt_v,  call_v, pusharg_v,
    funcenter_v,  funcexit_v,   newtable_v,
    tablegetelem_v, tablesetelem_v, nop_v, jump_v
};

enum vmarg_t{
    label_a = 0,
    global_a = 1,
    formal_a = 2,
    local_a = 3,
    number_a = 4,
    string_a = 5,
    bool_a = 6,
    nil_a = 7,
    userfunc_a = 8,
    libfunc_a = 9,
    retval_a = 10
};

typedef enum avm_memcell_t{
    number_m    = 0,
    string_m    = 1,
    bool_m      = 2,
    table_m     = 3,
    userfunc_m  = 4,
    libfunc_m   = 5,
    nil_m       = 6,
    undef_m     = 7
}avm_memcell_t;

class avm_table;

typedef class avm_memcell{
    private:
        avm_memcell_t type;
        union {
            double numVal;
            char *stringVal;
            bool boolVal;
            avm_table *tableVal;
            unsigned funcVal;
            char *libfuncVal;
        } data;
    public:
        avm_memcell_t getType();
        union data getData();
    avm_memcell(){

    }
}avm_memcell;

#define AVM_STACKSIZE 4096
#define AVM_WIPEOUT(m) memset(&(m), 0, sizeof(m))

avm_memcell stack[AVM_STACKSIZE];

typedef class avm_table_bucket{
    private:
        avm_memcell key;
        avm_memcell value;
    public:
        avm_memcell getKey(){return this->key;}
        avm_memcell getValue(){return this->value;}
    avm_table_bucket();
}avm_table_bucket;

#define AVM_TABLE_HASHSIZE 211

typedef class avm_table{
    private:
        unsigned refCounter;
        avm_table_bucket *strIndexed[AVM_TABLE_HASHSIZE];
        avm_table_bucket *numIndexed[AVM_TABLE_HASHSIZE];
        unsigned total;
    public:
        unsigned getRefCounter(){return this->refCounter;}
        avm_table_bucket *getStrIndexed();
        avm_table_bucket *getNumIndexed();
        unsigned getTotal(){return this->total;}
        void avm_tableincrefcounter(){this->refCounter++;}
        void avm_tablebucketsdestroy(); // TO DO
        void avm_tabledestroy(); // TO DO
    avm_table(){
        refCounter=0;
        for (int i=0 ;i<AVM_TABLE_HASHSIZE ; ++i){
            this->strIndexed[i] = NULL;
            this->numIndexed[i] = NULL;
        }

    }
}avm_table;


typedef class vmarg{
    private:
        vmarg_t type;
        unsigned val;
    public:
        vmarg_t getType(){return this->type;}
        void setType(vmarg_t type){ this->type=type; }
        unsigned getVal(){return this->val;}
        void setVal(unsigned val){ this->val=val; }

    vmarg(vmarg_t type,unsigned val){
        this->type=type;
        this->val=val;
    }

}vmarg;


typedef class instruction{
    private:
        vmopcode opcode;
        vmarg *result;
        vmarg *arg1;
        vmarg *arg2;
        unsigned srcLine;
    public:
        vmopcode getOpcode(){return this->opcode;}
        void setOpcode(vmopcode opcode){this->opcode=opcode;}
        vmarg *getResult(){return this->result;}
        void  setResult(vmarg *result){this->result=result;}
        vmarg *getArg1(){return this->arg1;}
        void  setArg1(vmarg *arg1){this->arg1=arg1;}
        vmarg *getArg2(){return this->arg2;}
        void  setArg2(vmarg *arg2){this->arg2=arg2;}
    instruction(){
    }
    instruction(vmopcode opcode,vmarg *result,vmarg *arg1,vmarg *arg2){
        this->opcode=opcode;
        this->result=result;
        this->arg1=arg1;
        this->arg2=arg2;
    }
    
}instruction;

typedef class userfunc{
    private:
        unsigned address;
        unsigned localSize;
        std::string id;
    public:
        unsigned getAddress(){return this->address;}
        unsigned getLocalSize(){return this->localSize;}
        std::string getId(){return this->id;}
    
    userfunc(unsigned address,unsigned localSize,std::string id){
        this->address=address;
        this->localSize=localSize;
        this->id=id;
    }
}userfunc;

typedef class incomplete_jump {
    private:
        unsigned instrNo;
        unsigned iaddress;
    public:
        unsigned getInstrNo(){return this->instrNo;}
        unsigned getIaddress(){return this->iaddress;}
    
    incomplete_jump(unsigned instrNo,unsigned iaddress){
        this->instrNo=instrNo;
        this->iaddress=iaddress;
    }    
}incomplete_jump;

unsigned nextinstructionlabel();

unsigned consts_newstring(std::string s);
unsigned consts_newnumber(double n);
unsigned libfuncs_newused(std::string s);

vmarg *make_operand(expr *e,vmarg *arg);
void patch_incoplete_jumps();

void emit_instruction(instruction t);
void generate(vmopcode op,quad q);
void generate_relational(vmopcode op,quad q);
void generate_ADD(quad q);
void generate_SUB(quad q);
void generate_MUL(quad q);
void generate_DIV(quad q);
void generate_MOD(quad q);
void generate_UMINUS(quad q);
void generate_NEWTABLE(quad q);
void generate_TABLEGETELEM(quad q);
void generate_TABLESETELEM(quad q);
void generate_ASSIGN(quad q);
void generate_NOP(quad q);

void generate_JUMP(quad q);
void generate_IF_EQ(quad q);
void generate_IF_NOTEQ(quad q);
void generate_IF_GREATER(quad q);
void generate_IF_GREATEREQ(quad q);
void generate_IF_LESS(quad q);
void generate_IF_LESSEQ(quad q);

void generate_NOT(quad q);
void generate_OR(quad q);
void generate_AND(quad q);

void generate_PARAM(quad q);
void generate_CALL(quad q);
void generate_GETRETVAL(quad q);

void generate_FUNCSTART(quad q);
void generate_RETURN(quad q);
void generate_FUNCEND(quad q);


typedef void (*generator_func_t)(quad);

generator_func_t generators[] = {
    generate_ASSIGN,
    generate_ADD, 
    generate_SUB,
    generate_MUL,
    generate_DIV,
    generate_MOD,
    generate_UMINUS,
    generate_AND,
    generate_OR,
    generate_NOT,
    generate_IF_EQ,
    generate_IF_NOTEQ,
    generate_IF_LESSEQ,
    generate_IF_GREATEREQ,
    generate_IF_LESS,
    generate_IF_GREATER,
    generate_CALL,
    generate_PARAM,
    generate_RETURN,
    generate_GETRETVAL,
    generate_FUNCSTART,
    generate_FUNCEND,
    generate_NEWTABLE,
    generate_TABLEGETELEM,
    generate_TABLESETELEM,
    generate_JUMP,
    generate_NOP,
};
void generates(void);
void backpatchRet(unsigned label);


void magicnumber();
void strings();
void numbers();
void libfunctions();
void arrays();
void instruc(int i);
void code();
void binaryf();
std::string vm_iopcode_to_string(vmopcode op);
std::string vm_vmarg_t_to_string(vmarg_t type);

void execute_assign(instruction t); 
void execute_add(instruction t);
void execute_sub(instruction t);
void execute_mul(instruction t);
void execute_div(instruction t); 
void execute_mod(instruction t);
void execute_uminus(instruction t);  
void execute_and(instruction t);
void execute_or(instruction t);
void execute_not(instruction t);
void execute_jeq(instruction t);
void execute_jne(instruction t);
void execute_jle(instruction t);
void execute_jge(instruction t);
void execute_jlt(instruction t);
void execute_jgt(instruction t); 
void execute_call(instruction t);
void execute_pusharg(instruction t);
void execute_funcenter(instruction t); 
void execute_funcexit(instruction t);  
void execute_newtable(instruction t);
void execute_tablegetelem(instruction t); 
void execute_tablesetelem(instruction t);
void execute_nop(instruction t);
void execute_jump(instruction t);

typedef void (*execute_func_t)(instruction t);

execute_func_t executeFuncs[]{
    execute_assign,   
    execute_add,  
    execute_sub,
    execute_mul,  
    execute_div,  
    execute_mod,
    execute_uminus,   
    execute_and,  
    execute_or,
    execute_not,  
    execute_jeq,  
    execute_jne,
    execute_jle,  
    execute_jge,  
    execute_jlt,
    execute_jgt,  
    execute_call, 
    execute_pusharg,
    execute_funcenter,  
    execute_funcexit,   
    execute_newtable,
    execute_tablegetelem, 
    execute_tablesetelem, 
    execute_nop, 
    execute_jump
};