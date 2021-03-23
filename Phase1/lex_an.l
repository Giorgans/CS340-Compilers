%{
/*** Georgios Zervos AM:3384;
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 ***/
#include <iostream>
#include <fstream>
#include <sstream>
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

/*List of all tokens*/
list <alpha_token_t> tokens;

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

/*For multiline/nested comments*/
int bcomment_depth = 0;
stack <int> comment_linespan;

/*All token found from lexical analysis exported in a text file*/
void tokens2file();

/*Saving the results of tokens and errors in text files*/
ofstream lexical_analysis("Lexical_Analysis.txt");
ofstream Errors("errors.txt");

/*Saving all tokens in a text file*/
string token2string(string content,int line,string type);

/*Fixing the strings with escape characters*/
string String_Esc_Fix(string content,int line);

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
undefined .
%x COMMENT

%option outfile = "lexical_analyzer.cpp"
%option yylineno
%option noyywrap

%%
{keyword}		{tokens.push_back(token(yylineno,yytext,"\t<---enumerated","KEYWORD"));}
{operator}      {tokens.push_back(token(yylineno,yytext,"\t<---enumerated","OPERATOR"));}
{integer}       {tokens.push_back(token(yylineno,yytext,"\t<---integer","INTCONST"));}
{real}	        {tokens.push_back(token(yylineno,yytext,"\t<---real","REALCONST"));}
{string}        {tokens.push_back(token(yylineno,yytext,"\t<---char*","STRING"));}
{punctuator}    {tokens.push_back(token(yylineno,yytext,"\t<---enumerated","PUNCTUATOR"));}
{id}	     	{tokens.push_back(token(yylineno,yytext,"\t<---char*","ID"));}
{delimiter}			{}
{undefined}		{
	string undef; stringstream temp; temp << yytext; temp >> undef;
    if(!undef.compare("\""))
		Errors << "Error: String that starts in line " + to_string(yylineno) + " isn't closed!\n"; 
    else 
		Errors << "Undefined input \"" + undef +  "\" in line " + to_string(yylineno) + "\n";       
}
{single_line_comment} {tokens.push_back(token(yylineno,"","\tLINE_COMMENT\t<---enumerated","COMMENT"));}
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
		tokens.push_back(token(yylineno,lines,"\tNESTED_COMMENT\t<---enumerated","COMMENT"));
	}
	else {
		bcomment_depth--;
		tokens.push_back(token(yylineno,lines,"\tMULTILINE_COMMENT\t<---enumerated","COMMENT"));
		BEGIN(INITIAL);
	}
}
<COMMENT>. {}
<COMMENT>{delimiter} {}
<COMMENT><<EOF>> {Errors << "Block comment that started in line: " << to_string(comment_linespan.top()) << " is not closed " <<endl; BEGIN(INITIAL);}

%%
/*Saving all tokens in a text file*/
void tokens2file(){
   int tokennum=0;
   list <alpha_token_t>::iterator  token;
   for(token = tokens.begin() ; token!=tokens.end() ; token++){
	   	lexical_analysis <<  token->numline << ":\t#"<< ++tokennum <<"\t" ;
		if(token->type.compare("STRING")) lexical_analysis << "\""  ;
    	lexical_analysis << token->content ;
		if(token->type.compare("STRING")) lexical_analysis << "\""  ;
		lexical_analysis << "\t" << token->type  << "\t" << token2string(token->content,token->numline,token->type) << "\t" << token->description << "\n";
   }
}

/*Returns the string description of some type of tokens*/
string token2string(string content,int line,string type){
	/*Strings for IDs stay indentical*/
	if(!type.compare("ID")) return content;
	/*Strings with Escape characters needs to be fixed*/
	else if(!type.compare("STRING")) return String_Esc_Fix(content,line); 
	else if(!type.compare("COMMENT")) return "";
	/*Strings for operators*/
    else if(!content.compare("=")) return "ASSIGN";
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
	/*Other stuff like keywords getting in Capital Letters*/
	else {
		for(int i=0; i<content.size(); i++)
			content.at(i) = toupper(content.at(i));
		return content;
	}
}

/*Analyses a string to find possible escape characters*/
string String_Esc_Fix(string content,int line){ 
	string temp;
	for(int i=0 ; i<content.size() ; i++){
        if(content[i]=='\\' && content[i+1]=='n'){ i++; temp +=  "\n"; }
        else if(content[i]=='\\' && content[i+1]=='t'){ i++; temp +=  "\t"; }
        else if(content[i]=='\\' && content[i+1]=='r'){ i++; temp +=  "\r"; }
        else if(content[i]=='\\' && content[i+1]=='v'){ i++; temp +=  "\v"; }
        else if(content[i]=='\\' && content[i+1]=='r'){ i++; temp +=  "\r"; }
        else if(content[i]=='\\' && content[i+1]=='f'){ i++; temp +=  "\f"; }
        else if(content[i]=='\\' && content[i+1]=='e'){ i++; temp +=  "\e"; }
        else if(content[i]=='\\' && content[i+1]=='b'){ i++; temp +=  "\b"; }
        else if(content[i]=='\\' && content[i+1]=='\"'){ i++; temp +=  "\""; }
        else if(content[i]=='\\' && content[i+1]=='\''){ i++; temp +=  "\'"; }
        else if(content[i]=='\\' && content[i+1]=='\\'){ i++; temp +=  "\\"; }
        else if(content[i]=='\\' && content[i+1]=='\"' && content[i+2]=='\0'){
			Errors << "Error: String that starts in line " + to_string(yylineno) + " isn't closed!\n"; i+=2; }
        else if(content[i]=='\\' && content[i+1]!=' '){
        	Errors << "Warning: Illegal escape char " << content[i+1] << " in line " + to_string(line) + "\n"; i++; }
        else temp += content[i];  
    }
	return temp;
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
	lexical_analysis <<  "\n-----------------------    Lexical Analysis    -----------------------\n" ;
	Errors <<  "\n----------------  Errors|Warnings|Undefined  ----------------\n" ;
	alpha_yylex(NULL);
	tokens2file();
	lexical_analysis <<  "----------------------------------------------------------------------" ;
	Errors <<  "-------------------------------------------------------------" ;

	Errors.close();
	lexical_analysis.close();
}