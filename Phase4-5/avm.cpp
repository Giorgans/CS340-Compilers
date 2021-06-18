/*** --- AVM C++ FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.4 , x86 chip ***/
#include <stack>
#include <fstream>
#include "avm.h"
#include "parser.h"

extern FILE *yyin;
extern SymbolTable table;

std::vector <double> numConsts;
std::vector <std::string> stirngConsts;
std::vector <userfunc> *userFuncs;
std::vector <std::string>  namedLibfuncs;

unsigned instructionslabel=0;

extern std::vector <quad> quads;
std::vector <instruction> instructions;
std::vector <incomplete_jump> inc_jumps;
std::stack <userfunc> funcstack;
std::vector <unsigned> returnlist;

unsigned nextinstructionlabel(){
    return instructionslabel;
}

unsigned consts_newstring(std::string s){
    for (unsigned i = 0; i < stirngConsts.size() ; i++)
        if (stirngConsts.at(i) == s) return i;
    
    stirngConsts.push_back(s);
    return stirngConsts.size()-1;
}
unsigned consts_newnumber(double n){
    for (unsigned i = 0 ; i < numConsts.size() ; i++)
        if (numConsts.at(i) == n) return i;

    numConsts.push_back(n);
    return numConsts.size()-1;
}
unsigned libfuncs_newused(std::string s){
    for (unsigned i = 0; i < namedLibfuncs.size() ; i++)
        if (namedLibfuncs.at(i) == s) return i;
    
    namedLibfuncs.push_back(s);
    return namedLibfuncs.size()-1;
}


vmarg *make_operand(expr *e){
    if(e==NULL) return NULL;
    switch (e->getType())
    {
        case var_e:
        case tableitem_e:
        case arithexp_e:
        case boolexp_e:
        case newtable_e:{
            if(e->getSymbol()->getSpace()==programvar) 
                return new vmarg(global_a,e->getSymbol()->getOffset());
            if(e->getSymbol()->getSpace()==functionlocal) 
                return new vmarg(local_a,e->getSymbol()->getOffset());
            if(e->getSymbol()->getSpace()==formalarg) 
                return new vmarg(formal_a,e->getSymbol()->getOffset());

        }
        /*Constants*/
        case constbool_e:{
            return new vmarg(bool_a,e->getboolConst()); break;
        }
        case conststring_e:{
            return new vmarg(string_a,consts_newstring(e->getstrConst())); break;
        }
        case costnum_e:{
            return new vmarg(number_a,consts_newnumber(e->getnumconst())); break;
        }
        case nil_e:{
            return new vmarg(nil_a,0); break;
        }
        /*Functions*/
        case programfunc_e:{
            return new vmarg(userfunc_a,e->getSymbol()->getOffset()); break;
        }
        case libraryfunc_e:{
            return new vmarg(libfunc_a,libfuncs_newused(e->getSymbol()->getName())); break;
        }

        default: return NULL;
    }
}
 
void patch_incoplete_jumps(){
    for (int i = 0 ; i < inc_jumps.size() ; i++){
        if(inc_jumps.at(i).getIaddress() == quads.size())
            instructions.at(inc_jumps.at(i).getInstrNo()).setResult(new vmarg(label_a,nextQuad()));
        else
            instructions.at(inc_jumps.at(i).getInstrNo()).setResult(new vmarg(label_a,quads.at(inc_jumps.at(i).getIaddress()).getLabel()));
    }
}

void emit_instruction(instruction t){
    instructions.push_back(t);
}


void generate(vmopcode op,quad q){
    q.setTaddress(nextinstructionlabel());

    emit_instruction(instruction(op,make_operand(q.getResult()),make_operand(q.getArg1()),make_operand(q.getArg2())));
   std::cout << "DEBUGING TIME" << std::endl;
}

void generate_relational(vmopcode op,quad q){

    if(q.getLabel() <  instructions.size())
        emit_instruction(instruction(op,new vmarg(label_a,quads[q.getLabel()].getTaddress()),make_operand(q.getArg1()),make_operand(q.getArg2())));
    else    
        inc_jumps.push_back(incomplete_jump(nextinstructionlabel(),q.getLabel()));

    q.setTaddress(nextinstructionlabel());

}

void generate_ADD(quad q){ generate(add_v,q); }
void generate_SUB(quad q){ generate(sub_v,q); }
void generate_MUL(quad q){ generate(mul_v,q); }
void generate_UMINUS(quad q){ generate(mul_v,q); }
void generate_DIV(quad q){ generate(div_v,q); }
void generate_MOD(quad q){ generate(mod_v,q); }

void generate_NEWTABLE(quad q){ generate(newtable_v,q); }
void generate_TABLEGETELEM(quad q){ generate(tablegetelem_v,q); }
void generate_TABLESETELEM(quad q){ generate(tablesetelem_v,q); }
void generate_ASSIGN(quad q){ generate(assign_v,q); }
void generate_NOP(quad q){ instruction t; t.setOpcode(nop_v); emit_instruction(t);}

void generate_JUMP(quad q){ generate_relational(jump_v,q); }
void generate_IF_EQ(quad q){ generate_relational(jeq_v,q); }
void generate_IF_NOTEQ(quad q){ generate_relational(jne_v,q); }
void generate_IF_GREATER(quad q){ generate_relational(jgt_v,q); }
void generate_IF_GREATEREQ(quad q){ generate_relational(jge_v,q); }
void generate_IF_LESS(quad q){ generate_relational(jlt_v,q); }
void generate_IF_LESSEQ(quad q){ generate_relational(jle_v,q); }

void generate_NOT(quad q){
    q.setTaddress(nextinstructionlabel());
    
    emit_instruction(instruction(jeq_v,new vmarg(label_a,nextinstructionlabel()+3),make_operand(q.getArg1()),new vmarg(bool_a,false)));

    emit_instruction(instruction(assign_v,make_operand(q.getResult()),new vmarg(bool_a,false),NULL));

    emit_instruction(instruction(jump_v,new vmarg(label_a,nextinstructionlabel()+2),NULL,NULL));

    emit_instruction(instruction(assign_v,make_operand(q.getResult()),new vmarg(bool_a,true),NULL));
}

void generate_OR(quad q){
    q.setTaddress(nextinstructionlabel());

    emit_instruction(instruction(jeq_v,new vmarg(label_a,nextinstructionlabel()+4),make_operand(q.getArg1()),new vmarg(bool_a,true)));

    emit_instruction(instruction(assign_v,make_operand(q.getResult()),new vmarg(bool_a,false),NULL));
 
    emit_instruction(instruction(jump_v,new vmarg(label_a,nextinstructionlabel()+2),NULL,NULL));

    emit_instruction(instruction(assign_v,make_operand(q.getResult()),new vmarg(bool_a,true),NULL));

}

void generate_AND(quad q){
    q.setTaddress(nextinstructionlabel());

    emit_instruction(instruction(jeq_v,new vmarg(label_a,nextinstructionlabel()+4),make_operand(q.getArg1()),new vmarg(bool_a,true)));

    emit_instruction(instruction(assign_v,make_operand(q.getResult()),new vmarg(bool_a,false),NULL));
 
    emit_instruction(instruction(jump_v,new vmarg(label_a,nextinstructionlabel()+2),NULL,NULL));

    emit_instruction(instruction(assign_v,make_operand(q.getResult()),new vmarg(bool_a,true),NULL));
}

void generate_PARAM(quad q){
    q.setTaddress(nextinstructionlabel());

    emit_instruction(instruction(pusharg_v,NULL,make_operand(q.getArg1()),NULL));
}

void generate_CALL(quad q){
    q.setTaddress(nextinstructionlabel());

    emit_instruction(instruction(call_v,NULL,make_operand(q.getArg1()),NULL));
}

void generate_GETRETVAL(quad q){
    q.setTaddress(nextinstructionlabel());

    emit_instruction(instruction(assign_v,NULL,make_operand(q.getArg1()),NULL));
}

void generate_FUNCSTART(quad q){
    q.setTaddress(nextinstructionlabel());
    funcstack.push(userfunc(nextinstructionlabel(),q.getResult()->getSymbol()->getTotalLocals(),q.getResult()->getSymbol()->getName()));

    emit_instruction(instruction(funcenter_v,make_operand(q.getResult()),NULL,NULL));
}

void generate_RETURN(quad q){
    q.setTaddress(nextinstructionlabel());

    emit_instruction(instruction(assign_v,new vmarg(retval_a,0),make_operand(q.getArg1()),NULL));

    userfunc f = funcstack.top();
    
    returnlist.push_back(nextinstructionlabel()); //append

    emit_instruction(instruction(assign_v,new vmarg(label_a,0),NULL,NULL));
}

void generate_FUNCEND(quad q){
    q.setTaddress(nextinstructionlabel());
    
    funcstack.pop();
    
    backpatchRet(nextinstructionlabel());

    emit_instruction(instruction(funcexit_v,make_operand(q.getResult()),NULL,NULL));
}

void backpatchRet(unsigned label){
    for (int i = 0 ; i < returnlist.size() ; i++ )
        instructions[returnlist.at(i)].setResult(new vmarg(label_a,label));
}



void generates(void){
    for(instructionslabel = 0 ; instructionslabel < quads.size() ; instructionslabel++ ){
        std::cout << "INNSTRLB " << instructionslabel << "\t" << iopcode_to_string(quads.at(instructionslabel).getOp()) << std::endl;
        (*generators[quads.at(instructionslabel).getOp()])(quads.at(instructionslabel));
    }
    patch_incoplete_jumps();
}   


std::ofstream binary("binary.abc");



void magicnumber(){
    binary << "\n magic number: " << 777 << std::endl;
}

void strings(){
    if(stirngConsts.size()){
        binary << "\n strings: \n" << std::endl;
        for(unsigned i = 0 ; i < stirngConsts.size() ; i++)
            binary << stirngConsts.at(i) << "\t" ; 
    }
    binary << std::endl;
}

void numbers(){
    if(numConsts.size()){
        binary << "\n numbers: \n" << std::endl;
        for(unsigned i = 0 ; i < numConsts.size() ; i++)
            binary << numConsts.at(i) << "\t" ; 
    }
    binary << std::endl;
}

void libfunctions(){
    if(namedLibfuncs.size()){
        binary << "\n lib functions: \n" << std::endl;
        for(unsigned i = 0 ; i < namedLibfuncs.size() ; i++)
            binary << namedLibfuncs.at(i) << "\t" ; 
    }
    binary << std::endl;
}
void arrays(){
    strings();
    numbers();
    libfunctions();
}

void instruc(int i){
    binary <<  vm_iopcode_to_string(instructions.at(i).getOpcode()) << "\t\t";
    if(instructions.at(i).getResult()!=NULL)
        binary << instructions.at(i).getResult()->getType() << vm_vmarg_t_to_string(instructions.at(i).getResult()->getType()) << " , " << instructions.at(i).getResult()->getVal() << "\t";
    if(instructions.at(i).getArg1()!=NULL)
        binary << instructions.at(i).getArg1()->getType() <<  vm_vmarg_t_to_string(instructions.at(i).getArg1()->getType()) << " , " << instructions.at(i).getArg1()->getVal()  << "\t";
    if(instructions.at(i).getArg2()!=NULL)
        binary << instructions.at(i).getArg2()->getType() <<  vm_vmarg_t_to_string(instructions.at(i).getArg2()->getType())<< " , " << instructions.at(i).getArg2()->getVal() << "\t";
    binary << std::endl;
}

void code(){
    binary << "\nopcode\t\tresult\t\targ1\t\targ2\n";
    for (int i = 0 ; i < instructions.size() ; i++)
        instruc(i);
}


void binaryf(){
    magicnumber();
    arrays();
    code();
}

std::string vm_iopcode_to_string(vmopcode op){
    switch(op){
        case assign_v: return "assign";
        case add_v: return "add"; 
        case sub_v: return "sub";
        case mul_v: return "mul";
        case div_v: return "div";
        case mod_v: return "mod"; 
        case uminus_v: return "uminus";
        case and_v: return "and";
        case or_v: return "or";
        case not_v: return "not";
        case jeq_v: return "jeq";
        case jne_v: return "jne";
        case jle_v: return "jle";
        case jge_v: return "jge";
        case jlt_v: return "jlt"; 
        case jgt_v: return "jgt"; 
        case call_v: return "call";
        case pusharg_v: return "param";
        case funcenter_v: return "funcenter";
        case funcexit_v: return "funcexit";
        case newtable_v: return "newtable";
        case tablegetelem_v: return "tbl_get"; 
        case tablesetelem_v: return "tbl_set"; 
        case jump_v: return "jump";
        case nop_v: return "";
        default: return "";
    }

}

std::string vm_vmarg_t_to_string(vmarg_t type){
    switch(type){
        case label_a: return "(label)";
        case global_a: return "(global)"; 
        case formal_a: return "(formal)";
        case local_a: return "(local)";
        case number_a: return "(number)";
        case string_a: return "(string)"; 
        case bool_a: return "(bool)";
        case nil_a: return "(nil)";
        case userfunc_a: return "(userfunc)";
        case libfunc_a: return "(libfunc)";
        case retval_a: return "(retval)";
        default: return "";
    }
}

/************ VM ************/

avm_memcell *avm_translate_operand(vmarg *arg){
    if (arg==NULL) return NULL;

    switch (arg->getType())
    {
    case global_a:
    case local_a:
    case formal_a:
    case retval_a:
    break;
    
    default:
        break;
    }
}


void execute_assign(instruction t){

}
void execute_add(instruction t){}
void execute_sub(instruction t){}
void execute_mul(instruction t){}
void execute_div(instruction t){}
void execute_mod(instruction t){}
void execute_uminus(instruction t){}
void execute_and(instruction t){}
void execute_or(instruction t){}
void execute_not(instruction t){}
void execute_jeq(instruction t){}
void execute_jne(instruction t){}
void execute_jle(instruction t){}
void execute_jge(instruction t){}
void execute_jlt(instruction t){}
void execute_jgt(instruction t){}
void execute_call(instruction t){}
void execute_pusharg(instruction t){}
void execute_funcenter(instruction t){}
void execute_funcexit(instruction t){}
void execute_newtable(instruction t){}
void execute_tablegetelem(instruction t){}
void execute_tablesetelem(instruction t){}
void execute_nop(instruction t){}
void execute_jump(instruction t){}


int main(int argc, char **argv) { 
    if(argc>1){
        if(!(yyin = fopen(argv[1],"r"))){
        fprintf(stderr, "Cannot read file %s\n",argv[1]);
        return 1;
        }
    }
    else yyin = stdin;    
    std::cout << "\n--------------------------- Errors/Warnings ---------------------------" << std::endl;
    parser();
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    table.printSymbols();
    print_quads();
    generates();
    std::cout << "DEBUGING TIME" << std::endl;
    binaryf();

    return 0; 
}

//  std::cout << "DEBUGING TIME" << std::endl;
