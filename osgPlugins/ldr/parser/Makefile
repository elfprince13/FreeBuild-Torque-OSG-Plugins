RM = rm
CP = cp
FLEX = flex
YACC = bison 

all: lex.yy.cc y.tab.c y.tab.h

lex.yy.cc: ldlite.l y.tab.h
	$(FLEX) -L ldlite.l

y.tab.c: ldlite.y
	$(YACC) -v -d -l ldlite.y

y.tab.h: ldlite.y
	$(YACC) -v -d -l ldlite.y

clean:
	$(RM) -f lex.yy.cc ldlite.tab.c ldlite.tab.h

install:
	$(CP) lex.yy.c ../lex.yy.cpp
	$(CP) ldlite.tab.c ../y.tab.cpp
	$(CP) ldlite.tab.h ../y.tab.h
