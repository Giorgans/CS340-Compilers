CPP= g++
FL=flex
OF= -o
REM=rm -f

all:	lex_an_3384.cpp 
	$(CPP) -std=c++11 $(OF)  lexan_results lex_an_3384.cpp


lex_an_3384.cpp:	lexical_analysis_phase1.l 
	$(FL) lexical_analysis_phase1.l 

clean:	lexan_results lex_an_3384.cpp
	$(REM) lexan_results lex_an_3384.cpp

	