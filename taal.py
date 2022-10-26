#!/usr/bin/env python
#
# taal.py
#
# Running this program will print a C template
# for an sqlite3 application manipulating the presscon.db
# Press Conference Database.
#
# Revision History
#
# $Id: taal.py,v 1.3 2006/09/29 01:02:19 robert Exp $
#
# $Log: taal.py,v $
# Revision 1.3  2006/09/29 01:02:19  robert
# Minor corrections made.
#
# Revision 1.2  2006/09/21 17:11:59  robert
# *** empty log message ***
#
#
import string

print "/*"
print " *"
print " * Template for sqlite3 application."
print " *"
print " * Revision History"
print " *"
print " * *********************************************************/"
print "#include <stdio.h>"
print "#include <string.h>"
print "#include <sqlite3.h>"
print ""
print "static int callback(void *NotUsed, int argc, char **argv, char **azColName){"
print "    int i;"
print "      "
print "    for(i=0; i<argc; i++){"
print "        printf(\"%s = %s\\n\", azColName[i], argv[i] ? argv[i] : \"NULL\");"
print "    }"
print "    printf(\"\\n\");"
print "    return 0;"
print "}"
print ""
print ""
print "int main(int argc, char **argv){"
print "    sqlite3 *db;"
print "    char *zErrMsg = 0;"
print "    int rc;"
print ""
print "    rc=sqlite3_open(\"presscon.db\",&db);"
print "    if(rc){"
print "        fprintf(stderr,\"Can't open database:%s\\n\",sqlite3_errmsg(db));"
print "        sqlite3_close(db);"
print "        exit(1);"
print "    }"
print ""
print "    rc=sqlite3_exec(db,argv[2],callback,0,&zErrMsg);"
print "    if (rc!=SQLITE_OK) {"
print "        fprintf(stderr,\"SQLerror:%s\\n\",zErrMsg);"
print "        sqlite3_free(zErrMsg);"
print "    }"
print "    "
print "    sqlite3_close(db);"
print "    return 0;"
print "}"
