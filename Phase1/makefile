CPP= g++
FL=flex
OF= -o
REM=rm -f

all:	lex_an.cpp 
	$(CPP) -std=c++11 $(OF)  lexan_results lex_an.cpp


lex_an.cpp:	lexical_analysis_phase1.l 
	$(FL) lexical_analysis_phase1.l 

clean:	lexan_results lex_an.cpp
	$(REM) lexan_results lex_an.cpp

	
