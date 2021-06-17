/*** --- SYMBOL TABLE C++ FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.4 , x86 chip***/
#include "SymbolTable_v2.h"
#include "iCode.h"

/*** Creates a new symbol and stores its address 
 *   to the HashTable and the list for each scope **/
void SymbolTable::Insert(Symbol *sym){
    Symbol *newSymbol = sym;
    inccurrscopeoffset();
    unsigned int index = hash(sym->getName());
    if(HashTable[index]==NULL) HashTable[index] =  new std::list<Symbol>();
    HashTable[index]->push_back(*newSymbol);

    std::list<ScopeLists>::iterator i;
    bool scopefound=false;
    for (i=scopelists.begin() ; i!=scopelists.end() ; i++){
        if(i->getScope()==sym->getScope()) { 
            i->getList()->push_back(*newSymbol);
            scopefound=true;
        }
    }

    if(!scopefound) {
        std::list<ScopeLists>::iterator current,next=scopelists.begin();
        if(scopelists.empty())
            scopelists.push_back(*(new ScopeLists(sym->getScope(),newSymbol)));
        else{
            next++;
            for(current=scopelists.begin(); current!=scopelists.end(); current++){
                if(current->getScope()<sym->getScope() && next==scopelists.end()){ 
                    scopelists.push_back(*(new ScopeLists(sym->getScope() ,newSymbol)));
                    break;
                }
                else if(current->getScope()<sym->getScope()  && next->getScope()>sym->getScope() ){
                    scopelists.insert(next,*(new ScopeLists(sym->getScope() ,newSymbol)));
                    break;
                }
                next++;
            }       
        }
    }
}

/** Searching through the HashTable and returns the address
  * of the symbol were searching, NULL otherwise.  */
Symbol *SymbolTable::Lookup(std::string name){
    unsigned int index = hash(name);
    std::list<Symbol>::iterator i;
    for (i = HashTable[index]->begin() ; i != HashTable[index]->end() ; ++i){
        if(!name.compare(i->getName()) && i->IsActive())
            return &*i;
     }
    return NULL;
}

/* Searching through the Scope Lists and returns, the address
    of the symbol were searching for a specific scope,NULL otherwise. */
Symbol *SymbolTable::LookupScope(std::string name,unsigned int scope){
    std::list<ScopeLists>::iterator i;
    std::list<Symbol>::iterator j;

    for (i=scopelists.begin() ; i!=scopelists.end() ; i++){
        if(i->getScope()==scope) { 
            for (j = i->getList()->begin() ; j != i->getList()->end() ; j++){
                if(!name.compare(j->getName()) && j->IsActive())
                    return &*j;
            }
        }
    }
    return NULL;
}

/** Turning all active symbols on a specific scope , inactive */
void SymbolTable::Hide(unsigned int scope){
    std::list<ScopeLists>::iterator i;
    std::list<Symbol>::iterator j;
    for (i=scopelists.begin() ; i!=scopelists.end() ; i++){
        if(i->getScope()==scope) { 
            for (j = i->getList()->begin() ; j != i->getList()->end() ; j++)
                if(j->IsActive()){
                     j->setInactive();
                }
        }
    }
}

/* Retuns the index for the Hash with name as the key */
unsigned int SymbolTable::hash(std::string name){ 
    unsigned int num = 0,i;
    for(i=0 ; i<name.length() ; i++)
        num+= (unsigned int)name[i];

    return num % BUCKETS;
}

/* Prints all information for all symbols for all scope */
void SymbolTable::printSymbols(){
    std::list<ScopeLists>::iterator i;
    std::list<Symbol>::iterator j;

    for (i = scopelists.begin() ; i != scopelists.end() ; i++){
        std::cout << "\n----------------------------- Scope   #" << i->getScope() << "  -----------------------------" << std::endl;
        for (j = i->getList()->begin() ; j != i->getList()->end() ; j++){
            std::cout <<"\"" << j->getName()<<"\"" << "\t"  ;
            if(j->getType()==libraryfunc_s) std::cout << "[library function]\t";
            else if(j->getType()==programfunc_s) std::cout << "[user function]\t";
            else std::cout << "[variable]\t";
            std::cout << "(line " << j->getLine() << ")\t" ;
            std::cout << "(scope " << j->getScope() << ")\t"<< std::endl ;
        }
    }
    std::cout << "-----------------------------------------------------------------------" << std::endl;

 }

//------------------------------------------------------------------------------------//







