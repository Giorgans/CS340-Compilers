/*** --- SYMBOL TABLE HEADER FILE ---
     Georgios Zervos AM:3384;
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.2.3 , x86 chip***/
#ifndef _ICODE_H_
#define _ICODE_H_
#include <iostream>
#include <list>
#include <string>
#define BUCKETS 1024
using namespace std;

/*enum SymbolType {GLOBAL, LOCALV, FORMAL, USERFUNC, LIBFUNC}; */
typedef enum symbol_t{var_s,programfunc_s,libraryfunc_s}symbol_t;

typedef enum scopespace_t{programvar,functionlocal,formalarg}scopespace_t;

/* SymbolTableEntry class containing all info for the symbol */
typedef class Symbol { 
    private:
        bool isActive;
        symbol_t type;
        string name;
        scopespace_t space;
        unsigned scope;
        unsigned offset;
        unsigned line;
    public:
        symbol_t getType(){return type;}
        string getName(){return name;}
        scopespace_t getSpace(){return space;}
        unsigned getScope(){return scope;}
        unsigned getLine(){return line;}
        unsigned getOffset(){return offset;}
        bool IsActive(){return isActive;}
        void setInactive(){this->isActive=false;}

        Symbol (symbol_t type,string name,unsigned scope,unsigned line,unsigned offset){
            this->isActive=true;
            this->type=type;
            this->name.assign(name);
            this->scope=scope;
            this->line=line;
        }
} Symbol;
 
 /* Scopelists class containing a list for each scope */
typedef class ScopeLists {
    private:
        unsigned int scope;
        list <Symbol> *S_list;
    public:
        unsigned int getScope(){return scope;}
        list <Symbol> *getList(){return S_list;}
    ScopeLists (unsigned int scope,Symbol *symbol){
        S_list = new list<Symbol>();
        this->scope=scope;
        S_list->push_back(*symbol);
    }
}ScopeLists;

/* Symbol Table class cointaining both the hashtable and the linked lists for each scope */
typedef class SymbolTable {
    private:
        list <Symbol> *HashTable[BUCKETS];
        list <ScopeLists> scopelists;
        unsigned int hash(string name);
    public:
        void Insert(string name,enum symbol_t type,unsigned int scope,unsigned int line);
        Symbol* Lookup(string name); 
        Symbol* LookupScope(string name,unsigned int scope);
        void Hide(unsigned int scope);
        void printSymbols();
    SymbolTable(){
        for (int i=0 ;i<BUCKETS ; ++i)
            this->HashTable[i] = NULL;

        this->Insert("print",libraryfunc_s,0,0);
        this->Insert("input",libraryfunc_s,0,0);
        this->Insert("objectmemberkeys",libraryfunc_s,0,0);
        this->Insert("objecttotalmembers",libraryfunc_s,0,0);
        this->Insert("objectcopy",libraryfunc_s,0,0);
        this->Insert("totalarguments",libraryfunc_s,0,0);
        this->Insert("argument",libraryfunc_s,0,0);
        this->Insert("typeof",libraryfunc_s,0,0);
        this->Insert("strtonum",libraryfunc_s,0,0);
        this->Insert("sqrt",libraryfunc_s,0,0);
        this->Insert("cos",libraryfunc_s,0,0);
        this->Insert("sin",libraryfunc_s,0,0);
    }
}SymbolTable;
#endif
