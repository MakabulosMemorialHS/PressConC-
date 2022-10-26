/*
 *
 * print-winners.c
 *
 * Prints a nicely formatted LaTeX page of the winners of a
 * particular contest. This program only requires that you
 * give the contest code.
 * 
 * Revision History
 *
 * $Id: print-winners.c,v 1.8 2013/09/23 17:56:28 robert Exp $
 *
 * $Log: print-winners.c,v $
 * Revision 1.8  2013/09/23 17:56:28  robert
 * Added routine to recognize the -l option which prints
 * a LaTeX formatted list only.
 *
 * Revision 1.7  2009/08/05 03:42:46  robert
 * Made the organization name and the contest dates into manifest
 * constants.
 *
 * Revision 1.6  2008/09/26 11:58:32  robert
 * Minor change in output of APSTAP letterhead.
 *
 * Revision 1.5  2008/09/23 16:04:47  robert
 * Added an informational banner which is shown whenever no
 * contest_code is given on the command line.
 *
 * Revision 1.4  2008/09/22 12:28:58  robert
 * Just a little bit of modification. The version before this
 * is the one used during the Press Con in SEpt 2006.
 *
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

static FILE *texfile;

static int contest_name(void *NotUsed, int argc, char **argv, char **azColName)
{
    fprintf(texfile, 
	       "\\leftline{\\bf %s %s %s - %s}\n"
               "\\begin{tabular}[t]{rlllr}\n",
	       argv[0], argv[1], argv[2], argv[3]);
    return 0;
}

static int winners_name(void *NotUsed, int argc, char **argv, char **azColName)
{
    double points;
    
    points = atof(argv[4]);
    fprintf(texfile,
	    "%4.1f&\\bf %s, %s&%s&%4.1f %s\\\\\n",
	    atof(argv[0]), argv[1], argv[2], argv[3], points,
	    (points < 2 ? "pt" : "pts"));
    
    return 0;
}

void banner(void)
{
    printf("PRINT-WINNERS: Prints the winners of a particular contest.\n\n"
	"Usage: print-winners <options> <contest_code>\n\n"
	"Options.\n\n"
	"-l    List and titles only. Print a formatted table with titles\n"
	"      but with no limbo text. This option is used whenever\n"
	"      we intend to use the output of PRINT-WINNERS to be\n"
	"      incorporated into another LaTeX document.\n\n"
	"      Without this option, PRINT-WINNERS outputs a LaTeX document\n"
	"      that is standalone and can be compiled into a complete\n"
	"      document.\n\n");
}

int main(int argc, char **argv){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char sqlstr[1024];
    char contest_code[16];
    char *str_ptr;

    int list_only_flag;
    int stdout_flag;
    int append_flag;
    char outputfile_name[1024];

    int retval;

    /* Check if database is present. */
    rc=sqlite3_open("presscon.db",&db);
    if(rc){
        fprintf(stderr,"Can't open database:%s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }


#ifdef DEBUG
    printf("argc = %d\n", argc);
    for (retval = 0; retval < argc; ++retval) {
	printf("%d %s\n", retval, argv[retval]);
    }
#endif

    /* Initialize important variables and flags. */
    list_only_flag = FALSE;   // Create a complete document.
    stdout_flag = FALSE;      // Do not send output to stdout.
    append_flag = FALSE;      // Do not append; overwrite whatever is in the output file.
                              // Obviously, this does not have any effect if the output
                              // is stdout.
    strcpy(outputfile_name, "output.tex"); // Default output file is output.tex


    /* Read the options. Some options  */
    while ((retval = getopt(argc, argv, "alof:")) != -1) {
	switch (retval) {
	    case 'l': // List-only. Document shall consist of many pages of output.
                      // Using this option means this filter shall not output zombie text
                      // and the LaTeX headers and footers.
		list_only_flag = TRUE;
		break;
            case 'o':  // Print to stdout rather than a file.
                stdout_flag = TRUE;
                break;
            case 'a':  // Append flag. Since, by default, the output file is called output.tex,
                       // this flag says to not overwrite whatever is already in that file.
                append_flag = TRUE;
                break;
            case 'f':  // Name of output file. Call the output file by this name.
                strcpy(outputfile_name, optarg);
                break;
	    default:
		break;
	}
    }



    if (optind < argc) {

#ifdef DEBUG
	printf("optind = %d, argv[optind] = %s\n",
		optind, argv[optind]);
#endif
	strcpy(contest_code, argv[optind]);
    }
    else {
	banner();
    }

    
    str_ptr = contest_code;
    while (*str_ptr != 0x00) {
	if (*str_ptr == '\n') *str_ptr = 0x00;
	*str_ptr = toupper(*str_ptr);
	++str_ptr;
    }
    

    if (stdout_flag == TRUE) {
        texfile = stdout;
    }

    if (stdout_flag == FALSE && append_flag == FALSE) {
        texfile = fopen(outputfile_name, "w");
    }

    if (stdout_flag == FALSE && append_flag == TRUE) {
        texfile = fopen(outputfile_name, "a");
    }

    if (list_only_flag == FALSE) { 
	fprintf(texfile, "\\documentclass[11pt]{article}\n"
	    "\\usepackage{palatino}\n"
	    "\\advance\\voffset by -1 in\n"
	    "\\advance\\hoffset by -0.5 in\n"
	    "\\advance\\textwidth by 1 in\n"
	    "\\advance\\textheight by 1 in\n"
	    "\\begin{document}\n"
	    "\\pagestyle{empty}\n"
	    "\\fontsize{11}{16}\n"
	    "\\selectfont\n");
    }
    fprintf(texfile, 
        "\\centerline{\\bf\\Large %s}\n"
	"\\centerline{\\bf\\Large PRESS CONFERENCE}\n"
	"\\centerline{\\large %s}\n"
	"\\vskip 0.5 in\n"
	"\\centerline{\\Large Winners}\n"
	"\\vskip 0.5 in\n", APSTAP_NAME, CONTEST_DATES);

    // First we get and print the contest name
    // 
    sprintf(sqlstr, 
	    "select contest_name, language, level, contest_code "
	    "from contest where contest_code=\'%s\';", contest_code);
	    
    rc=sqlite3_exec(db,sqlstr,contest_name,0,&zErrMsg);
    if (rc!=SQLITE_OK) {
        fprintf(stderr,"SQLerror:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }

    // Then we get a list of the winners for this particular
    // contest code.
    // 
    sprintf(sqlstr,
	    "select place, last_name, first_name, school_name, "
	    "points, contest_code "
	    "from wins natural join student natural join school "
	    "where contest_code=\'%s\' "
	    "order by place asc;",
	       contest_code);

    rc=sqlite3_exec(db,sqlstr,winners_name,0,&zErrMsg);
    if (rc!=SQLITE_OK) {
        fprintf(stderr,"SQLerror:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }

    if (list_only_flag == TRUE) {    
        fprintf(texfile,
	    "\\end{tabular}\n\\vfill\\eject\n");
    }
    else {
	fprintf(texfile,
		"\\end{tabular}\n\\end{document}\n");
    }
    fclose(texfile);
    sqlite3_close(db);
    return 0;
}
