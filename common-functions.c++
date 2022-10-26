/*
 *
 * common-functions.c++
 *
 * Functions common to all the utilities of PressConC++.
 *
 * *********************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sqlite3.h>
#include <unistd.h>
#include <getopt.h>
#include "presscon.h"

static FILE *texfile         {stdout};
static sqlite3 *db           {nullptr};

#ifdef OLDVERSION
static char *zErrMsg         {0x00};
static char contest_code[16] {0x00};
static char *str_ptr         {nullptr};
static char outputfile_name[1024]  {"output.tex"};

static int list_only_flag {FALSE};      // Create a complete document.
static int stdout_flag    {FALSE};      // Do not send output to stdout.
static int append_flag    {FALSE};      // Do not append; overwrite whatever is in the output file.
                                        // Obviously, this does not have any effect if the output
                                        // is stdout.
#endif


// Print the LaTeX limbo text.
void print_LaTeX_limbo(void)
{
    fprintf(texfile, "\\documentclass[10pt]{article}\n"
        "\\usepackage{palatino}\n"
        "\\advance\\voffset by -1 in\n"
        "\\advance\\hoffset by -0.5 in\n"
        "\\advance\\textwidth by 1 in\n"
        "\\advance\\textheight by 1 in\n"
        "\\begin{document}\n"
        "\\pagestyle{empty}\n"
        "\\fontsize{10}{15}\n"
        "\\selectfont\n");
}

// LaTeX end text and other clean up for LaTeX.
//
void print_LaTeX_endtext(void)
{
    fprintf(texfile, "\\end{document}\n");
}


// Each page of our output has to have a header.
// This function prints them.
void print_page_headers(void)
{
    fprintf(texfile, 
        "\\centerline{\\bf\\Large %s}\n"
        "\\centerline{\\bf\\Large PRESS CONFERENCE}\n"
        "\\centerline{\\large %s}\n"
        "\\vskip 0.5 in\n"
        "\\centerline{\\Large Winners}\n"
        "\\vskip 0.5 in\n", APSTAP_NAME, CONTEST_DATES);
}


// And each page should also have a footer.
void print_page_footers(void)
{
        fprintf(texfile,
        "\\vfill\\eject\n\n");
}


#ifdef OLDVERSION

void banner(void)
{
    printf("PRINT-ALL_WINNERS: Prints all the winners of all the contests.\n\n"
           "Usage:\n"
           "   print-all-winners <options>\n\n"
           "Options are:\n"
           " -o Print to stdout\n"
           " -f filename\n"
           "    Send to filename instead of the default output.tex\n\n");
}

/* ********************************************************************
 * The CALL BACK FUNCTIONS.
 * ********************************************************************/

int contest_winners_cback(void *NotUsed, int argc, char **argv, char **azColName)
{
    double points;
    
    points = atof(argv[4]);
    fprintf(texfile,
        "%4.1f&\\bf %s, %s&%s&%4.1f %s\\\\\n",
        atof(argv[0]), argv[1], argv[2], argv[3], points,
        (points < 2 ? "pt" : "pts"));
    
    return 0;
}


// The following call back function is invoked for each
// contest code.
int contest_code_cback(void *NotUsed, int argc, char **argv, char **azColName)
{
    char *contest_code {argv[0]};
    print_page_headers();

    fprintf(texfile,
        "\\leftline{\\bf %s %s %s - %s}\n",
        argv[1], argv[2], argv[3], contest_code);
    fprintf(texfile,
        "\\begin{tabular}[t]{rlllr}\n");

    process_contest_winners(contest_code);

    fprintf(texfile,
        "\\end{tabular}\n");
    print_page_footers();

    return 0;
}


/* ********************************************************************
 * The following functions process the contest codes and
 * the contest winners per contest code.
 * ********************************************************************/

void process_contest_winners(char *contest_code)
{
    char sqlstr[1024]   {0x00};
    char *zErrMsg       {0x00};
    int  rc             {SQLITE_ERROR};

    sprintf(sqlstr,
        "select place, last_name, first_name, school_name, "
        "points, contest_code "
        "from wins natural join student natural join school "
        "where contest_code=\'%s\' "
        "order by place asc;",
           contest_code);

    rc = sqlite3_exec(db,sqlstr,contest_winners_cback,0,&zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"SQLerror:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }
}


void process_contest_codes(void)
{
    char sqlstr[1024]   {0x00};
    char *zErrMsg       {0x00};
    int  rc             {SQLITE_ERROR};

    sprintf(sqlstr, 
        "select contest_code, contest_name, level, language "
        "from contest "
        "order by contest_code");

    rc = sqlite3_exec(db,sqlstr,contest_code_cback,0,&zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"SQLerror:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

#endif


