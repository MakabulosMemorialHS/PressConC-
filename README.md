README
This is a rewrite of the PressCon Program to use more of
C++ and Microservices.

Software for the APSTAP Press Conference

MANIFEST
--------------------------------------------

README        --   This file.
schema.dia    --   Diagram of the ERD (use dia to read)
schema.sql    --   SQL commands to create and populate the presscon.db
                   database.
presscon.db   --   The sqlite database. Could be populated
                   already. Check the contents of this db if
		   you have just checked out this package from
		   the CVS repository. The contents could be obsolete.
Makefile      --   Used to compile the various utilities.
*.c *.py      --   The various source codes in C and Python.

HOWTOS
---------------------------------------------------------------------
Q: I have just checked out the package from the repository. What
   should I do next?

A: You need to do the following:
   1. Edit the file presscon.h to reflect the correct contest dates.
   2. Check the presscon database with
         $ sqlite3 presscon
      to determine its current state.
   3. If need be, create a new presscon database and populate
      it with the default data. (See below)
   4. Give the following commands:
         $ make distclean
         $ make clean
         # make setup

---------------------------------------------------------------------
Q: How do I create and populate a new presscon database?

A: Use the program sqlite3 and the schema.sql file. 

        $ sqlite3 presscon.db ".read schema.sql"

    will both create the database and populate it with
    some standard data. Check the file schema.sql for
    information about the schema and the standard data.
---------------------------------------------------------------------

Q: How do I use the program during the Press Conference?

A: Just run "menu"
 
      $ ./menu.py

---------------------------------------------------------------------

Q: How do I port this software package to, say, Windows?

A: I haven't made any attempt to make this program portable
   to non-Unix systems but if you want to, you can try.

   This package requires the following libraries so you need them
   installed in order to make an attempt at porting.

   a. sqlite3 SQL database library.
   b. Tex and Latex for the printing.
   c. GNU Make and GCC.
   d. Python

   
