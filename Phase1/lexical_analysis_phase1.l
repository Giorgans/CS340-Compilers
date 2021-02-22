%{
    /** George Zervos 3384
        Stelios Mihelakakis 3524
	Iasonas Ntiagiannis 3540
      
     Prog. Languange used for FLEX: C++
 **/

#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <stack>

#define YY_DECL int alpha_yylex (void* yylval)

using namespace std;

/*Struct of each token found through lexical analysis*/
typedef struct alpha_token_t {
    int line;
    int tokennum;
    string desc;
    string charval;
    string type;
}alpha_token_t;

/*List to store all tokens found through lexical analysis*/
list <alpha_token_t> token_list;
/*List to store all errors and/or warnings found*/
list <string> error_list;

/*Stack to push the older lines*/
stack <int> commline_list;
int commentblock=0;
string lines;

/*Returns a String for each token*/
string toString(string text);

/*Analyses a string to find possible escape characters*/
void StringAnalyse(string text,int strline);

/*Prints all the errors and warnings */
void errors();

/*Prints all tokens found through lexical analysis*/
void print_lexan_results();

/*Creates a new token and adds it into the token list*/
void new_token(int line,string desc,string charval,string type){
    alpha_token_t newtok;
    newtok.line=line;
    newtok.tokennum=token_list.size();
    newtok.desc.assign(desc);
    newtok.charval.assign(charval);
    newtok.type.assign(type);
    token_list.push_back(newtok);
}

%}

keyword     "if"|"else"|"while"|"for"|"function"|"return"|"break"|"continue"|"and"|"not"|"or"|"local"|"true"|"false"|"nil"
operator    "="|"+"|"-"|"*"|"/"|"%"|"++"|"--"|"<"|">"|"<="|">="|"=="|"!="
id          [a-zA-Z][a-zA-Z_0-9]*
integer     (0[xX][0-9A-Fa-f]+)|([0-9]+)
real        [0-9]*\.[0-9](e|E)?[0-9]*
string      \"(\\.|[^"\\])*\"
punctuator  "{"|"}"|"("|")"|"["|"]"|";"|","|":"|"::"|"."|".."
space       [\r\n \t\v]+
comment     "//".*
undef       .
%x BCOMMENT

%option outfile = "lex_an.cpp"
%option yylineno
%option noyywrap


%%
{keyword}        {new_token(yylineno,yytext,"\t<-enumerated","KEYWORD");}
{operator}       {new_token(yylineno,yytext,"\t<-enumerated","OPERATOR");}
{id}             {new_token(yylineno,yytext,"\t<-char*","ID");}
{integer}        {new_token(yylineno,yytext,"\t<-integer","CONST_INT");}
{real}           {new_token(yylineno,yytext,"\t<-real","REALCONST");}
{string}         {new_token(yylineno,yytext,"\t<-char*","STRING");}
{punctuator}     {new_token(yylineno,yytext,"\t<-enumerated","PUNCTUATOR");}
{space}          {}
{comment}        {new_token(yylineno,"","\tLINE_COMMENT\t<-enumerated","COMMENT");}
{undef}          {string text; stringstream temp; temp << yytext; temp >> text;
                  if(!text.compare("\"")){string message = "Error: String is not closed in line " + to_string(yylineno); 
                                          error_list.push_back(message); } 
                  else{string message = "Undefined input \"" + text +  "\" in line " + to_string(yylineno);  
                       error_list.push_back(message);}        }
"/*" {
    commline_list.push(yylineno);
    BEGIN(BCOMMENT);
}


<BCOMMENT>"/*"     {  commline_list.push(yylineno); commentblock++; BEGIN(BCOMMENT); }

<BCOMMENT>"*/" {
        if(commentblock){
            commentblock--;
            if(yylineno!=commline_list.top()) lines = to_string(commline_list.top()) + "-" + to_string(yylineno);
            else lines = to_string(commline_list.top());
            commline_list.pop();
            new_token(yylineno,lines,"\tNEST_COMMENT\t<-enumerated","COMMENT");
        }
        else {
            if(yylineno!=commline_list.top()) lines = to_string(commline_list.top()) + "-" + to_string(yylineno);
            else lines = to_string(commline_list.top());
            commline_list.pop();
            new_token(yylineno,lines,"\tBLOCK_COMMENT\t<-enumerated","COMMENT");
            BEGIN(INITIAL);
        }
    }
    
<BCOMMENT>.    {}
<BCOMMENT>"\n" {}
<BCOMMENT><<EOF>> { string message =  "Error: comment started in line " + to_string(commline_list.top()) + " is not closed"; 
                    error_list.push_back(message);   
                    commline_list.pop();
                    commentblock--; BEGIN(INITIAL); }


%%

/*Returns a String for each token*/
string toString(string text){
    /*Strings for keywords*/
    if(!text.compare("if")) return "IF";
    if(!text.compare("else")) return "ELSE";
    if(!text.compare("while")) return "WHILE";
    if(!text.compare("for")) return "FOR";
    if(!text.compare("function")) return "FUNCTION";
    if(!text.compare("return")) return "RETURN";
    if(!text.compare("break")) return "BREAK";
    if(!text.compare("continue")) return "CONTINUE";
    if(!text.compare("and")) return "AND";
    if(!text.compare("not")) return "NOT";
    if(!text.compare("or")) return "OR";
    if(!text.compare("local")) return "LOCAL";
    if(!text.compare("true")) return "TRUE";
    if(!text.compare("false")) return "FALSE";
    if(!text.compare("nil")) return "NIL";
    /*Strings for operators*/
    if(!text.compare("=")) return "ASSIGN";
    if(!text.compare("+")) return "PLUS";
    if(!text.compare("-")) return "MINUS";
    if(!text.compare("*")) return "MULTIPLY";
    if(!text.compare("/")) return "DIV";
    if(!text.compare("%")) return "MOD";
    if(!text.compare("++")) return "PLUS_PLUS";
    if(!text.compare("--")) return "MINUS_MINUS";
    if(!text.compare("<")) return "LESS_THAN";
    if(!text.compare(">")) return "GREATER_THAN";
    if(!text.compare("<=")) return "LESS_EQUAL";
    if(!text.compare(">=")) return "GREATER_EQUAL";
    if(!text.compare("==")) return "EQUAL";
    if(!text.compare("!=")) return "NOT_EQUAL";
    /*Strings for punctuators*/
    if(!text.compare("{")) return "LEFT_BRACE";
    if(!text.compare("}")) return "RIGHT_BRACE";
    if(!text.compare("(")) return "LEFT_PARENTHESIS";
    if(!text.compare(")")) return "RIGHT_PARENTHESIS";
    if(!text.compare("[")) return "LEFT_BRACKET";
    if(!text.compare("]")) return "RIGHT_BRACKET";
    if(!text.compare(";")) return "SEMICOLON";
    if(!text.compare(",")) return "COMMA";
    if(!text.compare(":")) return "COLON";
    if(!text.compare("::")) return "DOUBLE_COLON";
    if(!text.compare(".")) return "DOT";
    if(!text.compare("..")) return "DOUBLE_DOT";
    return "";
}

/*Analyses a string to find possible escape characters*/
void StringAnalyse(string text,int strline){
    for(int i=0 ; i<text.size() ; i++){
        if(text[i]=='\\' && text[i+1]=='n'){ i++; cout << "\n"; }
        else if(text[i]=='\\' && text[i+1]=='t'){ i++; cout << "\t"; }
        else if(text[i]=='\\' && text[i+1]=='r'){ i++; cout << "\r"; }
        else if(text[i]=='\\' && text[i+1]=='v'){ i++; cout << "\v"; }
        else if(text[i]=='\\' && text[i+1]=='r'){ i++; cout << "\r"; }
        else if(text[i]=='\\' && text[i+1]=='f'){ i++; cout << "\f"; }
        else if(text[i]=='\\' && text[i+1]=='e'){ i++; cout << "\e"; }
        else if(text[i]=='\\' && text[i+1]=='b'){ i++; cout << "\b"; }
        else if(text[i]=='\\' && text[i+1]=='\"'){ i++; cout << "\""; }
        else if(text[i]=='\\' && text[i+1]=='\''){ i++; cout << "\'"; }
        else if(text[i]=='\\' && text[i+1]=='\\'){ i++; cout << "\\"; }
        else if(text[i]=='\\' && text[i+1]=='\"' && text[i+2]=='\0'){
            string error = "Error: String is not closed in line "; 
            string message= error + to_string(strline);
            i+=2; error_list.push_back(message); 
        }
        else if(text[i]=='\\' && text[i+1]!=' '){
            string error = "Warning: Illegal escape char in line "; 
            string message= error + to_string(strline);
            i++; error_list.push_back(message); 
        }
        else cout << text[i];  
    }
}

/*Prints all tokens found through lexical analysis*/
void print_lexan_results(){
    
    cout << "\n----------------------    Lexical analysis    ----------------------" << endl;
    list <alpha_token_t>::iterator i;
    int count=0;
    for(i=token_list.begin(); i!=token_list.end(); i++){
        if(i->type.compare("STRING")==0){
            cout << i->line << ":\t#" << ++count << "\t" <<i->desc << "\t" << i->type << "\t"; StringAnalyse(i->desc,i->line);  cout << i->charval << endl;      }
        else if(i->type.compare("ID")==0)
            cout << i->line << ":\t#" << ++count << "\t"<< "\"" << i->desc << "\"\t" << i->type << "\t"<< "\"" << i->desc << "\"" <<i->charval << endl;
        else
            cout << i->line << ":\t#" << ++count << "\t" << "\"" <<i->desc << "\"\t" << i->type << "\t" << toString(i->desc) <<i->charval << endl;
    }
    cout << "--------------------------------------------------------------------" << endl;

}

/*Prints all the errors and warnings */
void  errors(){
    list <string>::iterator i;
    if(!error_list.empty()) cout << "\n\t----------------  Errors/Warnings  ----------------" << endl;
    for(i=error_list.begin(); i!=error_list.end(); i++)
       cout << "\t " << *i << endl; 
    if(!error_list.empty()) cout << "\t---------------------------------------------------\n" << endl;
}

int main(int argc, char **argv) {
	if(argc > 1){
		if(!(yyin = fopen(argv[1],"r"))){
			fprintf(stderr, "Cannot read file: %s\n", argv[1]);
			return 1;
		}
	}
	else{
		printf("Enter text here:\n");
		yyin = stdin;
	}

	alpha_yylex(NULL);
    print_lexan_results();
    errors();
    return 0;
}
