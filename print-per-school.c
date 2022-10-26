/*
 *
 * print-per-school.c
 *
 * Prints list of winners per school. This document is often
 * requested by each school in order to inform them of who
 * their winners are.
 * 
 * Revision History
 *
 * $Id: print-per-school.c,v 1.6 2013/09/26 15:16:03 robert Exp $
 *
 * $Log: print-per-school.c,v $
 * Revision 1.6  2013/09/26 15:16:03  robert
 * By default this program now sends its output to stdout.
 * It also now prints long lists properly.
 *
 * Revision 1.5  2013/09/23 18:00:29  robert
 * Added routine to recognize the -l option which prints a LaTeX
 * formatted list only.
 *
 * Revision 1.4  2009/08/05 03:37:33  robert
 * Made the organization name and the dates of the press con
 * a manifest constant declared and defined in the header
 * file presscon.h
 *
 * Revision 1.3  2008/09/22 12:26:48  robert
 * Modified the date of the press con and the name of the
 * organization.
 *
 * Revision 1.2  2007/09/20 13:37:15  robert
 * *** empty log message ***
 *
 * Revision 1.1  2006/09/29 01:03:24  robert
 * Version as used to print reports during FeTAPS PressCon 2006.
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

/* *****************************************************
   * Switches and Manifest Constants:
   * 
   * USE_STDOUT -- Uses standard output for the output file.
   *****************************************************/
#define USE_STDOUT
#define MAX_NAMES_IN_FIRST_PAGE         20
#define MAX_NAMES_PER_PAGE              30

/*******************************************************
 * Global variables
 * ****************************************************/

static FILE *texfile;

static char SCHOOL_NAME[128];
static char school_code[16];
static sqlite3 *db;
static int list_only_flag;
static int current_page;             // Will hold the current page
                                     // being printed.
static int names_in_page;            // Number of names already in
                                     // this page.

/*******************************************************
 * Function definitions
 * ****************************************************/

void banner(void)
{
    printf("PRINT-PER-SCHOOL: Prints the winners for a given school.\n\n"
	"Usage: print-per-school <options> <school_code>\n\n"
	"Options.\n\n"
	"-l    List and titles only. Print a formatted table with titles\n"
	"      but with no limbo text. This option is used whenever\n"
	"      we intend to use the output of PRINT-PER-SCHOOL to be\n"
	"      incorporated into another LaTeX document.\n\n"
	"      Without this option, PRINT-PER-SCHOOL outputs a LaTeX document\n"
	"      that is standalone and can be compiled into a complete\n"
	"      document.\n\n");
}

// The next callback function gets the complete name of the school
// and saves it into the global variable SCHOOL_NAME.
// 
static int get_school_name(void *NotUsed, int argc, 
	char **argv, char **azColName)
{
    strcpy(SCHOOL_NAME, argv[0]);

    return 0;
}


static int winners_name(void *NotUsed, int argc, char **argv, char **azColName)
{

    names_in_page++;      // Increment the names in current page.

    if ((current_page == 1 && names_in_page > MAX_NAMES_IN_FIRST_PAGE) 
	    || (current_page > 1 && names_in_page > MAX_NAMES_PER_PAGE)) {

         fprintf(texfile,
	     "\\end{tabular}\n"
	     "\\vfill\\eject\n"
	     "\\centerline{\\bf\\large %s, \\textit{continued}}\n"
	     "\\vspace {0.15 in}\n"
             "\\begin{tabular}[t]{lrl}\n"
	     "\\bf Name&\\bf Place&\\bf Category\\\\\n"
             "\\hline\\\\",
             SCHOOL_NAME);
	 names_in_page = 1;
	 current_page++;

    }

    fprintf(texfile,
	    "\\bf %s, %s&%d&%s %s %s\\\\\n",
	        argv[0], argv[1], atoi(argv[2]),
	        argv[3], argv[4], argv[5]);
    
    return 0;
}


void print_elementary(void)
{
    char *zErrMsg = 0;
    int rc;
    char sqlstr[1024];

    if (list_only_flag == FALSE) {
        fprintf(texfile,
	    "\\documentclass[11pt]{article}\n"
	    "\\usepackage{palatino}\n"
	    "\\advance\\textwidth by 1 in\n"
	    "\\advance\\textheight by 1 in\n"
	    "\\advance\\voffset by -1 in\n"
	    "\\advance\\hoffset by -1 in\n"
	    "\\begin{document}\n"
	    "\\pagestyle{empty}\n"
	    "\\fontsize{11}{16}\n"
	    "\\selectfont\n");
    }
    
    fprintf(texfile,
	"\\centerline{\\bf\\Large %s}\n"
	"\\centerline{\\bf\\Large PRESS CONFERENCE}\n"
	"\\centerline{\\large %s}\n"
	"\\vskip 0.25 in\n"
	"\\centerline{\\Large LIST OF WINNERS PER SCHOOL}\n"
	"\\centerline{\\Large Elementary Level}\n"
	"\\vskip 0.25 in\n"
	"\\centerline{\\bf\\large %s}\n"
	"\\vskip 0.25 in\n"
	"\\begin{tabular}[t]{lrl}\n"
	"\\bf Name&\\bf Place&\\bf Category\\\\\n"
	"\\hline\n", APSTAP_NAME, CONTEST_DATES, SCHOOL_NAME);
    // Now we get the list of winners for this school.
    // 
    sprintf(sqlstr,
	    "select last_name, first_name, place, "
	    "contest_name, language, level "
	    "from student natural join wins natural join contest "
	    "where school_code='%s' "
	    "and level='Elementary' "
	    "order by last_name, first_name;",
	        school_code);
    rc=sqlite3_exec(db, sqlstr, winners_name, 0, &zErrMsg);
    if (rc!=SQLITE_OK) {
        fprintf(stderr,"SQLerror:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }


    fprintf(texfile,
	"\\end{tabular}\n"
	"\\vfill\\eject\n");
    
}


void print_secondary(void)
{
    char *zErrMsg = 0;
    int rc;
    char sqlstr[1024];

    fprintf(texfile,
	"\\centerline{\\bf\\Large %s}\n"
	"\\centerline{\\bf\\Large PRESS CONFERENCE}\n"
	"\\centerline{\\large %s}\n"
	"\\vskip 0.25 in\n"
	"\\centerline{\\Large LIST OF WINNERS PER SCHOOL}\n"
	"\\centerline{\\Large Secondary Level}\n"
	"\\vskip 0.25 in\n"
	"\\centerline{\\bf\\large %s}\n"
	"\\vskip 0.25 in\n"
	"\\begin{tabular}[t]{lrl}\n"
	"\\bf Name&\\bf Place&\\bf Category\\\\\n"
	"\\hline\n", APSTAP_NAME, CONTEST_DATES, SCHOOL_NAME);
    
    // Now we get the list of winners for this school.
    // 
    sprintf(sqlstr,
	    "select last_name, first_name, place, "
	    "contest_name, language, level "
	    "from student natural join wins natural join contest "
	    "where school_code='%s' "
	    "and level='Secondary' "
	    "order by last_name, first_name;",
	        school_code);
    rc=sqlite3_exec(db, sqlstr, winners_name, 0, &zErrMsg);
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
}

// Now for the main function which ties it all together.
// 
int main(int argc, char **argv){
    char *zErrMsg = 0;
    int rc;
    char sqlstr[1024];
    char *str_ptr;
    int retval;

    int stdout_flag;
    int append_flag;
    char outputfile_name[1024];

    /* ***************************************************
       Initialization
       ***************************************************/

    // Open the database file.
    rc=sqlite3_open("presscon.db",&db);
    if(rc){
        fprintf(stderr,"Can't open database:%s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    /* Set the default value for the flags. */
    list_only_flag = FALSE;  // list_only_flag is a global variable.
    stdout_flag = FALSE;      // Do not send output to stdout.
    append_flag = FALSE;      // Do not append; overwrite whatever is in the output file.
                              // Obviously, this does not have any effect if the output
                              // is stdout.
    strcpy(outputfile_name, "output.tex"); // Default output file is output.tex


    /* ***************************************************
       Read the command line options.
       The options can change the value of the flags.
       ***************************************************/

    while ((retval = getopt(argc, argv, "alof:")) != -1) {
	switch (retval) {
	    case 'l':
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

    /* After having read all the options, which start with a dash -, the variable
       optind (from unistd.h) should now point to the school code. */

    if (optind < argc) {
	strcpy(school_code, argv[optind]);
    }
    else {
	banner();
	exit(0);
    }


    // Set the school_code to all uppercase. Just in case.
    // 
    str_ptr = school_code;
    while (*str_ptr != 0x00) {
	if (*str_ptr == '\n') *str_ptr = 0x00;
	*str_ptr = toupper(*str_ptr);
	++str_ptr;
    }


    // Open the TeX file and other initializations.
    //


    if (stdout_flag == TRUE) {
        texfile = stdout;
    }

    if (stdout_flag == FALSE && append_flag == FALSE) {
        texfile = fopen(outputfile_name, "w");
    }

    if (stdout_flag == FALSE && append_flag == TRUE) {
        texfile = fopen(outputfile_name, "a");
    }

    // Now get the fullname of the school.
    // 
    sprintf(sqlstr, "select school_name, school_code "
	       "from school "
	       "where school_code=\'%s';", school_code);
    rc=sqlite3_exec(db,sqlstr,get_school_name,0,&zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr,"SQLerror:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }


    // Print elementary level

    current_page = 1;
    names_in_page = 0;
    print_elementary();


    // Print secondary level

    current_page = 1;
    names_in_page = 0;
    print_secondary();
   

    // Close database

    sqlite3_close(db);


    // Close TeX file. We don't need to close stdout as this is
    // OS generated.

    if (stdout_flag == FALSE) {
        fclose(texfile);
    }

    return 0;
}
