/*** ---SYMBOL TABLE HEADER FILE---
     Georgios Zervos AM:3384;
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 ***/
#include <iostream>
#include <list>
#include <string>
#define BUCKETS 1024
using namespace std;

enum SymbolType {
GLOBAL, LOCALV, FORMAL, USERFUNC, LIBFUNC
};

typedef class Variable { 
    private:
        string name; 
        unsigned int scope; 
        unsigned int line;
    public:
        string getName(){return name;}
        unsigned int getScope(){return scope;}
        unsigned int getLine(){return line;}

    Variable(string name,unsigned int scope,unsigned int line){
        this->name=name;
        this->scope=scope;
        this->line=line;
    }
} Variable;

typedef class Function { 
    private:
        string name; //List of arguments unsigned 
        unsigned int scope; 
        unsigned int line;
    public:
        string getName(){return name;}
        unsigned int getScope(){return scope;}
        unsigned int getLine(){return line;}

    Function(string name,unsigned int scope,unsigned int line){
        this->name=name;
        this->scope=scope;
        this->line=line;
    }
} Function;

typedef class SymbolTableEntry { 
    private:
        bool isActive;
        union {
            Variable *varVal;
            Function *funcVal; 
        } value;
        enum SymbolType type; 
    public:
        Function *getFunc(){return this->value.funcVal;}
        void storeFunc(Function *function){value.funcVal=function;}

        Variable *getVar(){return this->value.varVal;}
        void storeVal(Variable *variable){value.varVal=variable;}

        enum SymbolType getType(){return type;}

        bool IsActive(){return isActive;}
        void setInactive(){isActive=false;}

        SymbolTableEntry (SymbolType type){
            this->isActive=true;
            this->type=type;
        }

} SymbolTableEntry;
 

typedef class ScopeLists {
    private:
        unsigned int scope;
    public:
        list <SymbolTableEntry> *S_list;
        unsigned int getScope(){return scope;}
    ScopeLists (unsigned int scope,SymbolTableEntry *symbol){
        S_list = new list<SymbolTableEntry>();
        this->scope=scope;
        S_list->push_back(*symbol);
    }
}ScopeLists;

typedef class SymbolTable {
    private:
        list <SymbolTableEntry> *HashTable[BUCKETS];
        unsigned int hash(string name);
    public:
        list <ScopeLists> scopelists;
        void Insert(string name,enum SymbolType type,unsigned int scope,unsigned int line);
        SymbolTableEntry* Lookup(string name); 
        SymbolTableEntry* LookupScope(string name,unsigned int scope);
        void Hide(unsigned int scope);
    SymbolTable(){
        for (int i=0 ;i<BUCKETS ; ++i)
            HashTable[i] = new list<SymbolTableEntry>();

        this->Insert("print",LIBFUNC,0,0);
        this->Insert("input",LIBFUNC,0,0);
        this->Insert("objectmemberkeys",LIBFUNC,0,0);
        this->Insert("objecttotalmembers",LIBFUNC,0,0);
        this->Insert("objectcopy",LIBFUNC,0,0);
        this->Insert("totalarguments",LIBFUNC,0,0);
        this->Insert("argument",LIBFUNC,0,0);
        this->Insert("typeof",LIBFUNC,0,0);
        this->Insert("strtonum",LIBFUNC,0,0);
        this->Insert("sqrt",LIBFUNC,0,0);
        this->Insert("cos",LIBFUNC,0,0);
        this->Insert("sin",LIBFUNC,0,0);
    }
}SymbolTable;


