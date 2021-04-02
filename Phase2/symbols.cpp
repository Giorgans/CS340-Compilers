#include "symbols.h"


void SymbolTable::Insert(string name,enum SymbolType type,
                                        unsigned int scope,
                                        unsigned int line){
    SymbolTableEntry *newSymbol = new SymbolTableEntry(type);

    if(type==USERFUNC || type==LIBFUNC)
        newSymbol->storeFunc(new Function(name,scope,line));
    else newSymbol->storeVal(new Variable(name,scope,line));

    unsigned int index = hash(name);
    HashTable[index]->push_back(*newSymbol);

    list<ScopeLists>::iterator i;
    bool scopefound=false;
    for (i=scopelists.begin() ; i!=scopelists.end() ; i++){
        if(i->getScope()==scope) { 
            i->S_list->push_back(*newSymbol);
            scopefound=true;
        }
    }

    if(!scopefound) 
        scopelists.push_back(*(new ScopeLists(scope,newSymbol)));
}

SymbolTableEntry *SymbolTable::Lookup(string name){
    unsigned int index = hash(name);
    list<SymbolTableEntry>::iterator i;
    for (i = HashTable[index]->begin() ; i != HashTable[index]->end() ; ++i){
        if(!name.compare(i->getVar()->getName()))
            return &*i;
     }
    return NULL;
}

SymbolTableEntry *SymbolTable::LookupScope(string name,unsigned int scope){
    list<ScopeLists>::iterator i;
    list<SymbolTableEntry>::iterator j;

    for (i=scopelists.begin() ; i!=scopelists.end() ; i++){
        if(i->getScope()==scope) { 
            for (j = i->S_list->begin() ; j != i->S_list->end() ; j++){
                if(!name.compare(j->getVar()->getName()))
                    return &*j;
            }
        }
    }
        kmf fvsm ;
REUJTN



    return NULL;
}


unsigned int SymbolTable::hash(string name){ 
    unsigned int num = 0,i;
    for(i=0 ; i<name.length() ; i++)
        num+= (unsigned int)name[i];
    num=num % BUCKETS;
    return (unsigned int)num;
}

int main(){
    return 0;
}
