CPP= g++
FL=flex
OF= -o
REM=rm -f

all: lexical_analyzer.cpp 
	$(CPP) $(OF) lexical_analyzer lexical_analyzer.cpp


lexical_analyzer.cpp: lex_an.l 
	$(FL) lex_an.l 

clean:	lexical_analyzer lexical_analyzer.cpp errors.txt Lexical_Analysis.txt
	$(REM) lexical_analyzer lexical_analyzer.cpp errors.txt Lexical_Analysis.txt

	