/*** --- AVM C++ FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.4 , x86 chip ***/

#include "avm.h"
#include <stack>
#include <fstream>

std::vector <double> numConsts;
std::vector <std::string> stirngConsts;
std::vector <userfunc> *userFuncs;
std::vector <std::string>  namedLibfuncs;

unsigned instructionslabel=0;

extern std::vector <quad> quads;
std::vector <instruction> instructions;
std::vector <incomplete_jump> inc_jumps;
std::vector <userfunc> funcstack;

unsigned nextinstructionlabel(){
    return instructionslabel;
}

unsigned consts_newstring(std::string s){
    for (unsigned i = 0; i < stirngConsts.size() ; i++)
        if (stirngConsts.at(i) == s) return i;
    
    stirngConsts.push_back(s);
    return stirngConsts.size();
}
unsigned consts_newnumber(double n){
    for (unsigned i = 0 ; i < numConsts.size() ; i++)
        if (numConsts.at(i) == n) return i;

    numConsts.push_back(n);
    return numConsts.size();
}
unsigned libfuncs_newused(std::string s){
    for (unsigned i = 0; i < namedLibfuncs.size() ; i++)
        if (namedLibfuncs.at(i) == s) return i;
    
    namedLibfuncs.push_back(s);
    return namedLibfuncs.size();
}


void make_operand(expr *e,vmarg *arg){
    switch (e->getType())
    {
    case var_e:
    case tableitem_e:
    case arithexp_e:
    case boolexp_e:
    case newtable_e:{
        vmarg_t temptype;
        switch (e->getSymbol()->getSpace())
        {
            case programvar:    temptype=global_a; break;
            case functionlocal:    temptype=local_a; break;
            case formalarg:    temptype=formal_a; break;        
            default:    assert(0);
        }
        arg = new vmarg(temptype,e->getSymbol()->getOffset());
    }
    /*Constants*/
    case constbool_e:{
        arg = new vmarg(bool_a,e->getboolConst()); break;
    }
    case conststring_e:{
        arg = new vmarg(string_a,consts_newstring(e->getstrConst())); break;
    }
    case costnum_e:{
        arg = new vmarg(string_a,consts_newnumber(e->getnumconst())); break;
    }
    case nil_e:{
        arg = new vmarg(nil_a,0); break;
    }
    /*Functions*/
    case programfunc_e:{
        arg = new vmarg(userfunc_a,e->getSymbol()->getOffset()); break;
    }
    case libraryfunc_e:{
        arg = new vmarg(libfunc_a,libfuncs_newused(e->getSymbol()->getName())); break;
    }

    default: assert(0);
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
    instruction  t;
    t.setOpcode(op);
    make_operand(q.getArg1(),t.getArg1());
    make_operand(q.getArg2(),t.getArg2());
    make_operand(q.getResult(),t.getResult());
    q.setTaddress(nextinstructionlabel());
    emit_instruction(t);
}

void generate_relational(vmopcode op,quad q){
    instruction  t;
    t.setOpcode(op);
    make_operand(q.getArg1(),t.getArg1());
    make_operand(q.getArg2(),t.getArg2());
    t.getResult()->setType(label_a);

    if(q.getLabel() <  instructions.size())
        t.getResult()->setVal(quads[q.getLabel()].getTaddress());
    else    
        inc_jumps.push_back(incomplete_jump(nextinstructionlabel(),q.getLabel()));

    q.setTaddress(nextinstructionlabel());
    emit_instruction(t);
}

void generate_ADD(quad q){ generate(add_v,q); }
void generate_SUB(quad q){ generate(sub_v,q); }
void generate_MUL(quad q){ generate(mul_v,q); }
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
    instruction  t;
    
    t.setOpcode(jeq_v);
    make_operand(q.getArg1(),t.getArg1());
    t.setArg2(new vmarg(bool_a,false));
    t.setResult(new vmarg(label_a,nextinstructionlabel()+3));
    emit_instruction(t);

    t.setOpcode(assign_v);
    t.setArg1(new vmarg(bool_a,false));
    t.setArg2(NULL);//reset_operad(&t.getArg2()); 
    make_operand(q.getResult(),t.getResult());
    emit_instruction(t);
 
    t.setOpcode(jump_v);
    t.setArg1(NULL);//reset_operad(&t.getArg1()); 
    t.setArg2(NULL);//reset_operad(&t.getArg2()); 
    t.setResult(new vmarg(label_a,nextinstructionlabel()+2));
    emit_instruction(t);

    t.setOpcode(assign_v);
    t.setArg1(new vmarg(bool_a,true));
    t.setArg2(NULL);//reset_operad(&t.getArg2()); 
    make_operand(q.getResult(),t.getResult());
    emit_instruction(t);
}

void generate_OR(quad q){
    q.setTaddress(nextinstructionlabel());
    instruction  t;
    
    t.setOpcode(jeq_v);
    make_operand(q.getArg1(),t.getArg1());
    t.setArg2(new vmarg(bool_a,true));
    t.setResult(new vmarg(label_a,nextinstructionlabel()+4));
    emit_instruction(t);


    make_operand(q.getArg2(),t.getArg1());
    t.getResult()->setVal(nextinstructionlabel()+3);
    emit_instruction(t);


    t.setOpcode(assign_v);
    t.setArg1(new vmarg(bool_a,false));
    t.setArg2(NULL);//reset_operad(&t.getArg2()); 
    make_operand(q.getResult(),t.getResult());
    emit_instruction(t);
 
    t.setOpcode(jump_v);
    t.setArg1(NULL);//reset_operad(&t.getArg1()); 
    t.setArg2(NULL);//reset_operad(&t.getArg2()); 
    t.setResult(new vmarg(label_a,nextinstructionlabel()+2));
    emit_instruction(t);

    t.setOpcode(assign_v);
    t.setArg1(new vmarg(bool_a,true));
    t.setArg2(NULL);//reset_operad(&t.getArg2()); 
    make_operand(q.getResult(),t.getResult());
    emit_instruction(t);
}

void generate_AND(quad q){
    q.setTaddress(nextinstructionlabel());
    instruction  t;
    
    t.setOpcode(jeq_v);
    make_operand(q.getArg1(),t.getArg1());
    t.setArg2(new vmarg(bool_a,true));
    t.setResult(new vmarg(label_a,nextinstructionlabel()+4));
    emit_instruction(t);


    make_operand(q.getArg2(),t.getArg1());
    t.getResult()->setVal(nextinstructionlabel()+3);
    emit_instruction(t);


    t.setOpcode(assign_v);
    t.setArg1(new vmarg(bool_a,false));
    t.setArg2(NULL);//reset_operad(&t.getArg2()); 
    make_operand(q.getResult(),t.getResult());
    emit_instruction(t);
 
    t.setOpcode(jump_v);
    t.setArg1(NULL);//reset_operad(&t.getArg1()); 
    t.setArg2(NULL);//reset_operad(&t.getArg2()); 
    t.setResult(new vmarg(label_a,nextinstructionlabel()+2));
    emit_instruction(t);

    t.setOpcode(assign_v);
    t.setArg1(new vmarg(bool_a,true));
    t.setArg2(NULL);//reset_operad(&t.getArg2()); 
    make_operand(q.getResult(),t.getResult());
    emit_instruction(t);
}

void generate_PARAM(quad q){
    q.setTaddress(nextinstructionlabel());
    instruction  t;
    t.setOpcode(pusharg_v);
    make_operand(q.getArg1(),t.getArg1());
    emit_instruction(t);
}

void generate_CALL(quad q){
    q.setTaddress(nextinstructionlabel());
    instruction  t;
    t.setOpcode(call_v);
    make_operand(q.getArg1(),t.getArg1());
    emit_instruction(t);
}

void generate_GETRETVAL(quad q){
    q.setTaddress(nextinstructionlabel());
    instruction  t;
    t.setOpcode(assign_v);
    make_operand(q.getArg1(),t.getArg1());
    emit_instruction(t);
}

void generate_FUNCSTART(quad q){
    q.setTaddress(nextinstructionlabel());
    funcstack.insert(funcstack.begin(), userfunc(nextinstructionlabel(),q.getResult()->getSymbol()->getTotalLocals(),q.getResult()->getSymbol()->getName()));

    instruction t;
    t.setOpcode(funcenter_v);
    make_operand(q.getResult(),t.getResult());
    emit_instruction(t);
}

void generate_RETURN(quad q){
    q.setTaddress(nextinstructionlabel());
    instruction t;
    t.setOpcode(assign_v);
    t.setResult(new vmarg(retval_a,0));
    make_operand(q.getArg1(),t.getArg1());
    emit_instruction(t);


    userfunc f = funcstack.at(0);
    //append()
    t.setOpcode(jump_v);
    t.setArg1(NULL);//reset_operad(&t.getArg1()); 
    t.setArg2(NULL);//reset_operad(&t.getArg2()); 
    t.setResult(new vmarg(label_a,0));
    emit_instruction(t);

}

void generate_FUNCEND(quad q){

}



void generate(void){
    for(instructionslabel = 0 ; instructionslabel < quads.size() ; instructionslabel++ )
        (*generators[quads.at(instructionslabel).getOp()])(quads.at(instructionslabel));

    patch_incoplete_jumps();
}   


std::ofstream binary("binary.abc");



void magicnumber(){
    binary << "\n magic number " << 777 << std::endl;
}

void strings(){
    if(!stirngConsts.size()){
        binary << "\n strings \n" << std::endl;
        for(unsigned i = 0 ; i < stirngConsts.size() ; i++)
            binary << stirngConsts.at(i) << "\t" ; 
    }
}

void numbers(){
    if(!numConsts.size()){
        binary << "\n numbers: \n" << std::endl;
        for(unsigned i = 0 ; i < numConsts.size() ; i++)
            binary << numConsts.at(i) << "\t" ; 
    }
}

void libfunctions(){
    if(!namedLibfuncs.size()){
        binary << "\n lib functions: \n" << std::endl;
        for(unsigned i = 0 ; i < namedLibfuncs.size() ; i++)
            binary << namedLibfuncs.at(i) << "\t" ; 
    }

}
void arrays(){
    strings();
    numbers();
    libfunctions();
}

void instruc(int i){
        binary <<  vm_iopcode_to_string(instructions.at(i).getOpcode()) << "\t";
        if(!instructions.at(i).getResult())
            binary << instructions.at(i).getResult()->getType() << vm_vmarg_t_to_string(instructions.at(i).getResult()->getType());
        
        if(!instructions.at(i).getArg1())
            binary << instructions.at(i).getArg1()->getType() <<  vm_vmarg_t_to_string(instructions.at(i).getArg1()->getType());
        
        if(!instructions.at(i).getArg2())
            binary << instructions.at(i).getArg2()->getType() <<  vm_vmarg_t_to_string(instructions.at(i).getArg2()->getType());
    
}

void code(){
    binary << "\nopcode\tresult\targ1\targ2\n";
    for (int i = 0 ; i < instructions.size() ; i++)
        instruc(i);
}


void binaryfile(){
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