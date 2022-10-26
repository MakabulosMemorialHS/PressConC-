# Makefile for the utilities in this package.
#
# Revision History:
#
# $Id: Makefile,v 1.7 2015/09/05 23:14:04 robert Exp $
#
# $Log: Makefile,v $
# Revision 1.7  2015/09/05 23:14:04  robert
# Fixed typo in one of the rules.
#
# Revision 1.6  2012/09/13 04:27:18  robert
# Added entry for distclean.
#
#

FLAGS = `pkg-config --libs --cflags sqlite3`

.phony : clean print commit setup clean-db
	
search : search.c
	gcc -o search search.c ${FLAGS}

clean-db :
	rm presscon.db
	sqlite3 presscon.db ".read schema.sql"

setup: print-winners print-rankings print-per-school


common-functions.o : common-functions.c++ presscon.h
	g++ -c common-functions.c++


report-maker : book.l
	lex book.l
	gcc -o report-maker lex.yy.c -ll

print-winners : print-winners.c presscon.h
	gcc -o print-winners print-winners.c -Wall ${FLAGS}


print-all-winners : print-all-winners.c++ presscon.h
	g++ -Wall -o print-all-winners print-all-winners.c++ ${FLAGS}


print-all-schools : print-all-schools.c++ common-functions.o presscon.h
	g++ -o print-all-schools print-all-schools.c++ common-functions.o ${FLAGS}


print-rankings : print-rankings.c presscon.h
	gcc -o print-rankings print-rankings.c -Wall ${FLAGS}

commit :
	cat SQL.out | sqlite3 presscon.db
	rm SQL.out

print-per-school : print-per-school.c presscon.h
	gcc -o print-per-school print-per-school.c -Wall `pkg-config --libs sqlite3`


print :
	latex output
	dvips -t letter output
	lpr -PColor output.ps


clean :
	-rm *.out *.dvi *.tex *.ps *.pdf
        

distclean :
	-rm presscon.db print-winners print-rankings print-per-school


