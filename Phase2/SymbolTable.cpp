/*** --- SYMBOL TABLE C++ FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.2.3 , x86 chip***/
#include "SymbolTable.h"

/*** Creates a new symbol and stores its address 
 *   to the HashTable and the list for each scope **/
void SymbolTable::Insert(string name,enum SymbolType type,
                                        unsigned int scope,
                                        unsigned int line){
    SymbolTableEntry *newSymbol = new SymbolTableEntry(type);
    if(type==USERFUNC || type==LIBFUNC)
        newSymbol->storeFunc(new Function(name,scope,line));
    else newSymbol->storeVal(new Variable(name,scope,line));

    unsigned int index = hash(name);
    if(HashTable[index]==NULL) HashTable[index] =  new list<SymbolTableEntry>();
    HashTable[index]->push_back(*newSymbol);

    list<ScopeLists>::iterator i;
    bool scopefound=false;
    for (i=scopelists.begin() ; i!=scopelists.end() ; i++){
        if(i->getScope()==scope) { 
            i->getList()->push_back(*newSymbol);
            scopefound=true;
        }
    }

    if(!scopefound) {
        list<ScopeLists>::iterator current,next=scopelists.begin();
        if(scopelists.empty())
            scopelists.push_back(*(new ScopeLists(scope,newSymbol)));
        else{
            next++;
            for(current=scopelists.begin(); current!=scopelists.end(); current++){
                if(current->getScope()<scope && next==scopelists.end()){ 
                    scopelists.push_back(*(new ScopeLists(scope,newSymbol)));
                    break;
                }
                else if(current->getScope()<scope && next->getScope()>scope){
                    scopelists.insert(next,*(new ScopeLists(scope,newSymbol)));
                    break;
                }
                next++;
            }       
        }
    }
}

/** Searching through the HashTable and returns the address
  * of the symbol were searching, NULL otherwise.  */
SymbolTableEntry *SymbolTable::Lookup(string name){
    unsigned int index = hash(name);
    list<SymbolTableEntry>::iterator i;
    for (i = HashTable[index]->begin() ; i != HashTable[index]->end() ; ++i){
        if(!name.compare(i->getVar()->getName()) && i->IsActive())
            return &*i;
     }
    return NULL;
}

/* Searching through the Scope Lists and returns, the address
    of the symbol were searching for a specific scope,NULL otherwise. */
SymbolTableEntry *SymbolTable::LookupScope(string name,unsigned int scope){
    list<ScopeLists>::iterator i;
    list<SymbolTableEntry>::iterator j;

    for (i=scopelists.begin() ; i!=scopelists.end() ; i++){
        if(i->getScope()==scope) { 
            for (j = i->getList()->begin() ; j != i->getList()->end() ; j++){
                if(!name.compare(j->getVar()->getName()) && j->IsActive())
                    return &*j;
            }
        }
    }
    return NULL;
}

/** Turning all active symbols on a specific scope , inactive */
void SymbolTable::Hide(unsigned int scope){
    list<ScopeLists>::iterator i;
    list<SymbolTableEntry>::iterator j;
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
unsigned int SymbolTable::hash(string name){ 
    unsigned int num = 0,i;
    for(i=0 ; i<name.length() ; i++)
        num+= (unsigned int)name[i];

    return num % BUCKETS;
}

/* Prints all information for all symbols for all scope */
void SymbolTable::printSymbols(){
    list<ScopeLists>::iterator i;
    list<SymbolTableEntry>::iterator j;

    for (i = scopelists.begin() ; i != scopelists.end() ; i++){
        cout << "\n----------------------------- Scope   #" << i->getScope() << "  -----------------------------" << endl;
        for (j = i->getList()->begin() ; j != i->getList()->end() ; j++){
            cout <<"\"" << j->getVar()->getName()<<"\"" << "\t"  ;
            if(j->getType()==LIBFUNC) cout << "[library function]\t";
            else if(j->getType()==USERFUNC) cout << "[user function]\t";
            else if(j->getType()==GLOBAL) cout << "[global variable]\t";
            else if(j->getType()==LOCALV) cout << "[local variable]\t";
            else cout << "[formal argument]\t";
            cout << "(line " << j->getVar()->getLine() << ")\t" ;
            cout << "(scope " << j->getVar()->getScope() << ")\t"<< endl ;
        }
    }
    cout << "-----------------------------------------------------------------------" << endl;

 }
