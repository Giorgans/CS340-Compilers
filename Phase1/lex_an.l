%{
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stack>
#include <cctype>
#define YY_DECL int alpha_yylex (void* yylval)
using namespace std;



/* Token struct for lexixal analizer  */
typedef struct alpha_token_t {
	unsigned int     numline;
	string           content;
	string  		 description;
	string           type; 
}alpha_token_t;

/*For nested multiline comments*/
int bcomment_depth = 0;
stack <int> comment_linespan;

/**/
list <alpha_token_t> tokens;

/*Fix string esc chars from being printed wrong*/
void string_print_fix();

/*All token found from lexical analysis exported in a text file*/
void tokens2file();


ofstream lexical_analysis("Lexical_Analysis.txt");
ofstream Errors("errors.txt");


/*Creating new token and returns it*/
alpha_token_t token(int numline,string content,string description,string type)
{
	alpha_token_t temp_token;
	temp_token.numline=numline;
	temp_token.content.assign(content);
	temp_token.description.assign(description);
	temp_token.type.assign(type);
    return temp_token;
}

string token2string(string content, string type);

void StringAnalyse(string text,int strline);



%}

keyword     "if"|"else"|"while"|"for"|"function"|"return"|"break"|"continue"|"and"|"not"|"or"|"local"|"true"|"false"|"nil"
operator    "="|"+"|"-"|"*"|"/"|"%"|"++"|"--"|"<"|">"|"<="|">="|"=="|"!="
integer     (0[xX][0-9A-Fa-f]+)|([0-9]+)
real        [0-9]*\.[0-9](e|E)?[0-9]*
string      \"(\\.|[^"\\])*\"
punctuator  "{"|"}"|"("|")"|"["|"]"|";"|","|":"|"::"|"."|".."
id          [a-zA-Z][a-zA-Z_0-9]*
delimiter       [\n\r \t\v]+
single_line_comment  "//".*
multi_line_comment_open  "/*" 
multi_line_comment_close "*/"
%x COMMENT



%option outfile = "lexical_analyzer.cpp"
%option yylineno
%option noyywrap



%%
{keyword}		{tokens.push_back(token(yylineno,yytext,"enumerated","KEYWORD"));}
{operator}      {tokens.push_back(token(yylineno,yytext,"enumerated","OPERATOR"));}
{integer}       {tokens.push_back(token(yylineno,yytext,"char*","INTCONST"));}
{real}	        {tokens.push_back(token(yylineno,yytext,"integer","REALCONST"));}
{string}        {tokens.push_back(token(yylineno,yytext,"real","STRING"));}
{punctuator}    {tokens.push_back(token(yylineno,yytext,"enumerated","PUNCTUATOR"));}
{id}	     	{tokens.push_back(token(yylineno,yytext,"enumerated","ID"));}
{delimiter}			{}
. 				{Errors<<" Undefined character "<< yytext <<" Found At Line: "<< yylineno<<endl;}
{single_line_comment} {tokens.push_back(token(yylineno,"","enumerated","LINE COMMENT"));}
{multi_line_comment_open} {
	bcomment_depth++;
	comment_linespan.push(yylineno);
	BEGIN(COMMENT);
} 

<COMMENT>{multi_line_comment_open} {
	bcomment_depth++;
	comment_linespan.push(yylineno); 
	BEGIN(COMMENT); 
}
<COMMENT>{multi_line_comment_close} {
	string lines;
	
	if(comment_linespan.top()==yylineno)  lines= to_string(comment_linespan.top());
	else	lines = to_string(comment_linespan.top()) + "-" + to_string(yylineno);
	comment_linespan.pop();
	
	if(bcomment_depth>1)  {
		bcomment_depth--;
		tokens.push_back(token(yylineno,lines,"enumerated","NESTED COMMENT"));
	}
	else {
		bcomment_depth--;
		tokens.push_back(token(yylineno,lines,"enumerated","MULTILINE COMMENT"));
		BEGIN(INITIAL);
	}

}

<COMMENT>. {}
<COMMENT>{delimiter} {}
<COMMENT><<EOF>> {Errors << "Block comment is not closed " <<endl; BEGIN(INITIAL);}




%%

void tokens2file(){
   int tokennum=0;
   list <alpha_token_t>::iterator  token;
   for(token = tokens.begin() ; token!=tokens.end() ; token++){
	   if(!token->type.compare("STRING")) {	
			lexical_analysis <<  token->numline << ":\t#"<< ++tokennum <<  "\t" << token->content  << "\t" << token->type  << "\t" ;
 			StringAnalyse(token->content,token->numline);
			lexical_analysis << "\t<---" << token->description << "\n";
	   }
	   else
	   		lexical_analysis <<  token->numline << ":\t#"<< ++tokennum <<  "\t" << "\""  << token->content << "\""  << "\t" << token->type  << "\t" << token2string(token->content,token->type)  << "\t<---"  << token->description << "\n";
   }



}

string token2string(string content, string type){
	
	/*Strings for operators*/
    if(!content.compare("=")) return "ASSIGN";
    else if(!content.compare("+")) return "PLUS";
    else if(!content.compare("-")) return "MINUS";
    else if(!content.compare("*")) return "MULTIPLY";
    else if(!content.compare("/")) return "DIV";
    else if(!content.compare("%")) return "MOD";
    else if(!content.compare("++")) return "PLUS_PLUS";
    else if(!content.compare("--")) return "MINUS_MINUS";
    else if(!content.compare("<")) return "LESS_THAN";
    else if(!content.compare(">")) return "GREATER_THAN";
    else if(!content.compare("<=")) return "LESS_EQUAL";
    else if(!content.compare(">=")) return "GREATER_EQUAL";
    else if(!content.compare("==")) return "EQUAL";
    else if(!content.compare("!=")) return "NOT_EQUAL";
    /*Strings for punctuators*/
    else if(!content.compare("{")) return "LEFT_BRACE";
    else if(!content.compare("}")) return "RIGHT_BRACE";
    else if(!content.compare("(")) return "LEFT_PARENTHESIS";
    else if(!content.compare(")")) return "RIGHT_PARENTHESIS";
    else if(!content.compare("[")) return "LEFT_BRACKET";
    else if(!content.compare("]")) return "RIGHT_BRACKET";
    else if(!content.compare(";")) return "SEMICOLON";
    else if(!content.compare(",")) return "COMMA";
    else if(!content.compare(":")) return "COLON";
    else if(!content.compare("::")) return "DOUBLE_COLON";
    else if(!content.compare(".")) return "DOT";
    else if(!content.compare("..")) return "DOUBLE_DOT";
./
	else if(!type.compare("ID")){
		return content;
	}
	
	else {
		for(int i=0; i<content.size(); i++){
			content.at(i) = toupper(content.at(i));
		}
		return content;
	}
}

/*Analyses a string to find possible escape characters*/
void StringAnalyse(string text,int strline){
    for(int i=0 ; i<text.size() ; i++){
        if(text[i]=='\\' && text[i+1]=='n'){ i++; lexical_analysis << "\n"; }
        else if(text[i]=='\\' && text[i+1]=='t'){ i++; lexical_analysis << "\t"; }
        else if(text[i]=='\\' && text[i+1]=='r'){ i++; lexical_analysis << "\r"; }
        else if(text[i]=='\\' && text[i+1]=='v'){ i++; lexical_analysis << "\v"; }
        else if(text[i]=='\\' && text[i+1]=='r'){ i++; lexical_analysis << "\r"; }
        else if(text[i]=='\\' && text[i+1]=='f'){ i++; lexical_analysis << "\f"; }
        else if(text[i]=='\\' && text[i+1]=='e'){ i++; lexical_analysis << "\e"; }
        else if(text[i]=='\\' && text[i+1]=='b'){ i++; lexical_analysis << "\b"; }
        else if(text[i]=='\\' && text[i+1]=='\"'){ i++; lexical_analysis << "\""; }
        else if(text[i]=='\\' && text[i+1]=='\''){ i++; lexical_analysis << "\'"; }
        else if(text[i]=='\\' && text[i+1]=='\\'){ i++; lexical_analysis << "\\"; }
        else if(text[i]=='\\' && text[i+1]=='\"' && text[i+2]=='\0'){
            Errors << "Error: String is not closed in line " + to_string(strline) + "\n"; 
            i+=2; 
        }
        else if(text[i]=='\\' && text[i+1]!=' '){
            Errors << "Warning: Illegal escape char in line " + to_string(strline) + "\n"; 
            i++; 
        }
        else lexical_analysis << text[i];  
    }
}



int main(int argc, char **argv) 
{
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
	lexical_analysis <<  "\n----------------------    Lexical analysis    ----------------------\n" ;
	Errors <<  "\n----------------  Errors/Warnings  ----------------\n" ;

	alpha_yylex(NULL);
	tokens2file();

	Errors.close();


}
