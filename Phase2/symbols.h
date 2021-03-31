#include <iostream>
#include <list>
#include <string>
#define BUCKET 1024
using namespace std;



enum SymbolType {
GLOBAL, LOCAL, FORMAL, USERFUNC, LIBFUNC
};

typedef struct Variable { 
    string name; 
    unsigned int scope; 
    unsigned int line;
} Variable;

typedef struct Function { 
    string name; //List of arguments unsigned 
    int scope; 
    unsigned int line;
} Function;

typedef struct SymbolTableEntry { 
    bool isActive;
    union {
        Variable *varVal;
        Function *funcVal; 
    } value;
enum SymbolType type; 
} SymbolTableEntry;

typedef struct token {
    SymbolTableEntry symbol;
    list <SymbolTableEntry> scopelist;
} token;

/*Insert a token in the symbol table*/
void Insert(string name,SymbolType type,int scope,int line);

/*Returns a token from the symbol table*/
SymbolTableEntry Lookup(); 

/*Turn a token into non active*/
void Hide();

/*Returns a hashnumber as index to the symbol table*/
unsigned int hash();