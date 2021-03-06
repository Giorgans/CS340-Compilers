%{
#include <iostream>
#include <string>
#include <stack>
#define YY_DECL int alpha_yylex (void* yylval)
using namespace std;



/* Token struct for lexixal analizer  */
typedef struct alpha_token_t {
  unsigned int     numline;
  string  		   description;
  string           content;
  string           type; 
}alpha_token_t;

/*Creating new token and returns it*/
alpha_token_t push_token(int numline,string description,string content,string type)
{
	alpha_token_t temp_token;
	temp_token.numline=numline;
	temp_token.description.assign(description);
	temp_token.content.assign(content);
	temp_token.type.assign(type);
    return temp_token;
}



/*For nested multiline comments*/
int nested_comm_count = 0;
stack <int> lines_from_comments;

/**/
stack <alpha_token_t> tokens;

/*Fix string esc chars from being printed wrong*/
void string_print_fix();




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
{keyword}		{tokens.push(push_token(yylineno,yytext,"EMPTY","KEYWORD"));}
{operator}      {tokens.push(push_token(yylineno,yytext,"EMPTY","OPERATOR"));}
{integer}       {tokens.push(push_token(yylineno,yytext,"EMPTY","INTCONST"));}
{real}	        {tokens.push(push_token(yylineno,yytext,"EMPTY","REALCONST"));}
{string}        {tokens.push(push_token(yylineno,yytext,"EMPTY","STRING"));}
{punctuator}    {tokens.push(push_token(yylineno,yytext,"EMPTY","PUNCTUATOR"));}
{id}	     	{tokens.push(push_token(yylineno,yytext,"EMPTY","ID"));}
{delimiter}			{}
. 				{cout<<" Undefined character "<< yytext <<" Found At Line: "<< yylineno<<endl;}
{single_line_comment} {cout << " Line Comment "<< yytext << " Found at Line" << yylineno<<endl;}
{multi_line_comment_open}         {nested_comm_count++;   cout << "NESTED COMMENT DEPTH : " << nested_comm_count<< endl;BEGIN(COMMENT); } 

<COMMENT>{multi_line_comment_open} { 
	nested_comm_count++; 
	lines_from_comments.push(yylineno); 
	cout << nested_comm_count<< endl;
	BEGIN(COMMENT); 
}
<COMMENT>{multi_line_comment_close} {
	nested_comm_count--; 
	if(!nested_comm_count)BEGIN(INITIAL);
    else{
	   if(lines_from_comments.top()==yylineno) {
		cout<<" Comment Opened and Closed At line : "<< yylineno<<endl;
	   }
	   else {
		   cout<< "Comment Opened At Line : "<< lines_from_comments.top() <<" And Closed At Line : " << yylineno<<" "<< endl;
		   }
        lines_from_comments.pop();
    }
}
	

<COMMENT>. {}
<COMMENT>{delimiter} {}
<COMMENT><<EOF>> {cout << "TO COMMENT DEN KLEINEI!!!!" <<endl; BEGIN(INITIAL);}




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
	alpha_yylex(NULL);


}
