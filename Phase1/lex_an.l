%{
#include <iostream.h>
#include <string.h>
#include <stack.h>

using namespace std;

int alpha_yylex (void* ylval)

/**** TO DO :
       NA GINOYN OI DILOSEIS SINARTISEON , METAVLITON KAI TOY
       KAI TOY FKNG ALPHA_TOKEN_T */

%}

keyword     "if"|"else"|"while"|"for"|"function"|"return"|"break"|"continue"|"and"|"not"|"or"|"local"|"true"|"false"|"nil"
operator    "="|"+"|"-"|"*"|"/"|"%"|"++"|"--"|"<"|">"|"<="|">="|"=="|"!="
integer     (0[xX][0-9A-Fa-f]+)|([0-9]+)
real        [0-9]*\.[0-9](e|E)?[0-9]*
string      \"(\\.|[^"\\])*\"
punctuator  "{"|"}"|"("|")"|"["|"]"|";"|","|":"|"::"|"."|".."
id          [a-zA-Z][a-zA-Z_0-9]*
space       [\r \t\v]+
new_line    \n
line_comm   "//".*

%x COMMENT

%option outfile = "lexical_analyzer.cpp"
%option yylineno
%option noyywrap




%%

!KANONES!


%%
/*
TO DO 
EXTRA KODIKAS !

*/

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


}
