%{
/*** ---LEXICAL ANALYSIS LEX/FLEX FILE---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540
     Compiled and run in Mac OS Big Sur 11.2.3 , x86 chip***/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <cctype>
#include <stdlib.h>
#include "parser.h"
using namespace std;

/*For multiline/nested comments*/
int bcomment_depth = 0;

/*Analyses a string to find possible errors with escape characters*/
void String_Esc_Fix(string content,int line);

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
{keyword}		{
	string text = yytext;
    if(!text.compare("if")) return IF;
	if(!text.compare("else")) return ELSE;
    if(!text.compare("while")) return WHILE;
    if(!text.compare("for")) return FOR;
    if(!text.compare("function")) return FUNCTION;
    if(!text.compare("return")) return RETURN;
    if(!text.compare("break")) return BREAK;
    if(!text.compare("continue")) return CONTINUE;
    if(!text.compare("and")) return AND;
    if(!text.compare("not")) return NOT;
    if(!text.compare("or")) return OR;
    if(!text.compare("local")) return LOCAL;
    if(!text.compare("true")) return TRUE;
    if(!text.compare("false")) return FALSE;
    if(!text.compare("nil")) return NIL;
}
{operator}      {
    string text = yytext;
    if(!text.compare("=")) return ASSIGN;
    if(!text.compare("+")) return PLUS;
    if(!text.compare("-")) return MINUS;
    if(!text.compare("*")) return MULTIPLY;
    if(!text.compare("/")) return DIV;
    if(!text.compare("%")) return MOD;
    if(!text.compare("++")) return PLUS_PLUS;
    if(!text.compare("--")) return MINUS_MINUS;
    if(!text.compare("<")) return LESS_THAN;
    if(!text.compare(">")) return GREATER_THAN;
    if(!text.compare("<=")) return LESS_EQUAL;
    if(!text.compare(">=")) return GREATER_EQUAL;
    if(!text.compare("==")) return EQUAL;
    if(!text.compare("!=")) return NOT_EQUAL;
}
{integer}       { yylval.intValue = atoi(yytext);    return INTCONST;}
{real}	        { yylval.realValue = atof(yytext);   return REALCONST;}
{string}        { String_Esc_Fix(yytext,yylineno); yylval.stringValue =strdup(yytext);  return STRING;}
{punctuator}    {
    string text = yytext;
    if(!text.compare("{")) return L_BRACE;
    if(!text.compare("}")) return R_BRACE;
    if(!text.compare("(")) return L_PARENTHESIS;
    if(!text.compare(")")) return R_PARENTHESIS;
    if(!text.compare("[")) return L_BRACKET;
    if(!text.compare("]")) return R_BRACKET;
    if(!text.compare(";")) return SEMICOLON;
    if(!text.compare(",")) return COMMA;
    if(!text.compare(":")) return COLON;
    if(!text.compare("::")) return DOUBLE_COLON;
    if(!text.compare(".")) return DOT;
    if(!text.compare("..")) return DOUBLE_DOT;

}
{id}	     	{ yylval.stringValue =strdup(yytext);  return ID;}
{delimiter}		{}
{undefined}		{
	string undef; stringstream temp; temp << yytext; temp >> undef;
    if(!undef.compare("\""))
		cout << "[Lexical Analysis] Error: String that starts in line " + to_string(yylineno) + " isn't closed!\n"; 
    else 
		cout << "[Lexical Analysis] Undefined input \"" + undef +  "\" in line " + to_string(yylineno) + "\n";       
}
{single_line_comment} {}
{multi_line_comment_open} {
	bcomment_depth++;
	BEGIN(COMMENT);
} 

<COMMENT>{multi_line_comment_open} { bcomment_depth++;  BEGIN(COMMENT); }
<COMMENT>{multi_line_comment_close} {
	if(bcomment_depth>1)  bcomment_depth--;
	else {
		bcomment_depth--;
		BEGIN(INITIAL);
	}
}
<COMMENT>. {}
<COMMENT>{delimiter} {}
<COMMENT><<EOF>> {cout << "[Lexical Analysis] ERROR: Block comment is not closed " <<endl; BEGIN(INITIAL);}

%%

/*Analyses a string to find possible errors with escape characters*/
void String_Esc_Fix(string content,int line){ 
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
			cout << "[Lexical Analysis] ERROR: String that starts in line " + to_string(yylineno) + " isn't closed!\n"; i+=2; }
        else if(content[i]=='\\' && content[i+1]!=' '){
        	cout << "[Lexical Analysis] WARNING: Illegal escape char " << content[i+1] << " in line " + to_string(line) + "\n"; i++; }
        else temp += content[i];  
    }
}
