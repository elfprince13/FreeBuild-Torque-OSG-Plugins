#!/usr/bin/env python

import sys,os,re

splitexp = re.compile(r'\r\n?|\n') # split on newlines
recog = ["extern int yylex (void);",  "#define YY_DECL int yylex (void)", '#line 3 "lex.yy.c"']
fix = ["extern int my_yylex (void);", "#define YY_DECL int my_yylex (void)",'']
patch = zip(recog,fix)
handle = open("lex.yy.c",'r')
data = handle.read()
handle.close()
datal = splitexp.split(data)
fixc = 0
for i in xrange(len(datal)):
	line = datal[i]
	for recog,fix in patch:
		if recog in line:
			print "replacing '%s' with '%s'" % (recog, fix)
			datal[i] = fix
			fixc +=1 
data = "\n".join(datal)
handle = open("lex.yy.c", 'w')
handle.write(data)
handle.close()

sys.exit(fixc!=len(patch))



