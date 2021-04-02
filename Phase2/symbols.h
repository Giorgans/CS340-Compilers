#include <iostream>
#include <list>
#include <string>
#define BUCKETS 1024
using namespace std;

enum SymbolType {
GLOBAL, LOCAL, FORMAL, USERFUNC, LIBFUNC
};

typedef class Variable { 
    private:
        string name; 
        unsigned int scope; 
        unsigned int line;
    public:
        string getName(){return name;}
        int getScope(){return scope;}
        int getLine(){return line;}

    Variable(string name,unsigned int scope,unsigned int line){
        this->name=name;
        this->scope=scope;
        this->line=line;
    }
} Variable;

typedef class Function { 
    private:
        string name; //List of arguments unsigned 
        int scope; 
        unsigned int line;
    public:
        string getName(){return name;}
        int getScope(){return scope;}
        int getLine(){return line;}

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
        this->scope=scope;
        S_list->push_back(*symbol);
    }
}ScopeLists;

typedef class SymbolTable {
    private:
        list <SymbolTableEntry> *HashTable[BUCKETS];
        list <ScopeLists> scopelists;
        unsigned int hash(string name);
    public:
        void Insert(string name,enum SymbolType type,unsigned int scope,unsigned int line);
        SymbolTableEntry* Lookup(string name); 
        SymbolTableEntry *LookupScope(string name,unsigned int scope);
        void Hide();
    SymbolTable(){
        for (int i=0 ; i<BUCKETS ; i++)
            HashTable[i]=NULL;
        
    }
}SymbolTable;



