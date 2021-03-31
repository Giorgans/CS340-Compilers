%{
/*** ---LEXICAL ANALYSIS---
     Georgios Zervos AM:3384;
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 ***/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <stdlib.h>
#include "parser.h"
using namespace std;

/*For multiline/nested comments*/
int bcomment_depth = 0;

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
{string}        { yylval.stringValue.assign(yytext); return STRING;}
{punctuator}    {
    string text = yytext;
    if(!text.compare("{")) return LEFT_BRACE;
    if(!text.compare("}")) return RIGHT_BRACE;
    if(!text.compare("(")) return LEFT_PARENTHESIS;
    if(!text.compare(")")) return RIGHT_PARENTHESIS;
    if(!text.compare("[")) return LEFT_BRACKET;
    if(!text.compare("]")) return RIGHT_BRACKET;
    if(!text.compare(";")) return SEMICOLON;
    if(!text.compare(",")) return COMMA;
    if(!text.compare(":")) return COLON;
    if(!text.compare("::")) return DOUBLE_COLON;
    if(!text.compare(".")) return DOT;
    if(!text.compare("..")) return DOUBLE_DOT;

}
{id}	     	{ yylval.stringValue.assign(yytext);   return ID;}
{delimiter}		{}
{undefined}		{
	string undef; stringstream temp; temp << yytext; temp >> undef;
    if(!undef.compare("\""))
		cout << "Error: String that starts in line " + to_string(yylineno) + " isn't closed!\n"; 
    else 
		cout << "Undefined input \"" + undef +  "\" in line " + to_string(yylineno) + "\n";       
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
<COMMENT><<EOF>> {cout << "Block comment is not closed " <<endl; BEGIN(INITIAL);}

%%