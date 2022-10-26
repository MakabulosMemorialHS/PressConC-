/*
 *
 * print-all-schools.c++
 *
 * Prints list of winners per school. This document is often
 * requested by each school in order to inform them of who
 * their winners are. 
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

static FILE *texfile          {stdout};

static char SCHOOL_NAME[1024] {"Uninitialized"};
static char school_code[16]   {"NONE"};
static sqlite3 *db            {nullptr};
static int list_only_flag     {false};
static int current_page       {1};    // Will hold the current page
                                     // being printed.
static int names_in_page      {0};    // Number of names already in
                                     // this page.

static bool semaphore         {false};   // Used by has_winners() and its call back function.
static float total_points     {0.0};     // Total number of points won by each school.

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

    return 0;
}

static int elementary_winners_name_cback(void *NotUsed, int argc, char **argv, char **azColName)
{
    char *last_name  {argv[0]};      // The last name will be here.

    if (strlen(last_name) == 0) return 0;    // If empty then do not print.

    names_in_page++;      // Increment the names in current page.

    if ((current_page == 1 && names_in_page > MAX_NAMES_IN_FIRST_PAGE) 
	    || (current_page > 1 && names_in_page > MAX_NAMES_PER_PAGE)) {

         fprintf(texfile,
	     "\\end{tabular}\n"
	     "\\vfill\\eject\n"
	     "\\centerline{\\bf\\large %s, \\textit{continued}}\n"
	     "\\vspace {0.15 in}\n"
             "\\begin{tabular}[t]{lrlr}\n"
	     "\\bf Name&\\bf Rank&\\bf Category&\\bf Pts\\\\\n"
             "\\hline\\\\",
             SCHOOL_NAME);
	 names_in_page = 1;
	 current_page++;

    }

    fprintf(texfile,
	    "\\bf %s, %s&%3.1f&%s %s %s&%3.1f\\\\\n",
	        argv[0], argv[1], atof(argv[2]),
	        argv[3], argv[4], argv[5], atof(argv[6]));
    total_points += atof(argv[6]);     // argv[6] has the number of points earned by this data row. 
    return 0;
}


static int secondary_winners_name_cback(void *NotUsed, int argc, char **argv, char **azColName)
{
    names_in_page++;      // Increment the names in current page.

    if ((current_page == 1 && names_in_page > MAX_NAMES_IN_FIRST_PAGE) 
	    || (current_page > 1 && names_in_page > MAX_NAMES_PER_PAGE)) {

         fprintf(texfile,
	     "\\end{tabular}\n"
	     "\\vfill\\eject\n"
	     "\\centerline{\\bf\\large %s, \\textit{continued}}\n"
	     "\\vspace {0.15 in}\n"
             "\\begin{tabular}[t]{lrlr}\n"
	     "\\bf Name&\\bf Rank&\\bf Category&\\bf Pts\\\\\n"
             "\\hline\\\\",
             SCHOOL_NAME);
	 names_in_page = 1;
	 current_page++;

    }

    fprintf(texfile,
	    "\\bf %s, %s&%3.1f&%s %s %s&%3.1f\\\\\n",
	        argv[0], argv[1], atof(argv[2]),
	        argv[3], argv[4], argv[5], atof(argv[6]));
    total_points += atof(argv[6]);     // argv[6] has the number of points earned by this data row. 
    
    return 0;
}


// Call back function for has_winners()
static int has_winners_cback(void *NotUsed, int argc, char **argv, char **azColName)
{
    char *pen_name     {argv[0]};             // The pen name should be in argv[0].
    size_t slen  {strlen(pen_name)};    // Checks if pen_name is empty.

    if (slen == 0)
        semaphore = false;
    else semaphore = true;
    return 0;
}


// This function checks if a school has a winner.

bool has_winners(char *school_code, char *level)
{
    char sqlstr[1024]   {0x00};
    char *zErrMsg         {nullptr};
    int rc              {SQLITE_ERROR};

    semaphore = false;    // Make sure to always set this false before doing anything else.

    sprintf(sqlstr,
            "select pen_name, school_code, contest_code "
            "from school natural join student "
            "natural join wins natural join contest "
            "where school_code = '%s' and level = '%s';",
            school_code, level);

    rc=sqlite3_exec(db, sqlstr, has_winners_cback, 0, &zErrMsg);
    if (rc!=SQLITE_OK) {
        fprintf(stderr,"SQLerror:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }

    // The callback function will set the semaphore to true or false
    // depending on whether the given school code has a winner
    // or not.
    return semaphore;
}

void print_elementary(char *school_code, char *school_name)
{
    char *zErrMsg         {nullptr};
    int rc                {SQLITE_ERROR};
    char sqlstr[1024]     {0x00};

    strcpy(SCHOOL_NAME, school_name);      // Needed by the callback function if it needs to
                                           // to create a new page.

    if (!has_winners(school_code, (char *) "Elementary")) return;     // If no winner then don't print this page.

    current_page  = 1;
    names_in_page = 0;
    total_points  = 0;                     // Initialize to zero. The callback function will change
                                           // this to the correct value.

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
	"\\begin{tabular}[t]{lrlr}\n"
	"\\bf Name&\\bf Rank&\\bf Category&\\bf Pts\\\\\n"
	"\\hline\n", APSTAP_NAME, CONTEST_DATES, school_name);
    
    // Now we get the list of winners for this school.
    // 
    sprintf(sqlstr,
	    "select last_name, first_name, place, "
	    "contest_name, language, level, points "
	    "from student natural join wins natural join contest "
	    "where school_code='%s' "
	    "and level='Elementary' "
	    "order by last_name, first_name;",
	        school_code);

    rc = sqlite3_exec(db, sqlstr, elementary_winners_name_cback, 0, &zErrMsg);

    if (rc!=SQLITE_OK) {
        fprintf(stderr,"SQLerror:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }

    fprintf(texfile,
        "\\\\\n"
        "& &\\hfill\\bf Total Points&%3.1f\\\\\n"
        "\\end{tabular}\n\\vfill\\eject\n", total_points);
    
}


void print_secondary(char *school_code, char *school_name)
{
    char *zErrMsg         {nullptr};
    int rc                {SQLITE_ERROR};
    char sqlstr[1024]     {0x00};

    strcpy(SCHOOL_NAME, school_name);      // Needed by the callback function if it needs to
                                           // to create a new page.

    if (!has_winners(school_code, (char *) "Secondary")) return;     // If no winner then don't print this page.
                                           
    current_page  = 1;    // Currently on this page.
    names_in_page = 0;    // Number of names already in
                          // this page.
    total_points  = 0;                     // Initialize to zero. The callback function will change
                                           // this to the correct value.


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
	"\\begin{tabular}[t]{lrlr}\n"
	"\\bf Name&\\bf Rank&\\bf Category&\\bf Pts\\\\\n"
	"\\hline\n", APSTAP_NAME, CONTEST_DATES, school_name);
    
    // Now we get the list of winners for this school.
    // 
    sprintf(sqlstr,
	    "select last_name, first_name, place, "
	    "contest_name, language, level, points "
	    "from student natural join wins natural join contest "
	    "where school_code='%s' "
	    "and level='Secondary' "
	    "order by last_name, first_name;",
	        school_code);

    rc = sqlite3_exec(db, sqlstr, secondary_winners_name_cback, 0, &zErrMsg);

    if (rc!=SQLITE_OK) {
        fprintf(stderr,"SQLerror:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }

    fprintf(texfile,
        "\\\\\n"
        "& &\\hfill\\bf Total Points&%3.1f\\\\\n"
        "\\end{tabular}\n\\vfill\\eject\n", total_points);
    
    
}


int school_codes_cback(void *NotUsed, int argc, char **argv, char **azColName)
{
    char *school_code   {argv[0]};
    char *school_name   {argv[1]};
    
    print_secondary(school_code, school_name);
    print_elementary(school_code, school_name);
    return 0;
}


// Get a list of all schools in the database and process
// them.
int process_school_codes(void)
{
    char sqlstr[1024] {0x00};
    int  rc           {SQLITE_ERROR};
    char *zErrMsg     {nullptr};

    sprintf(sqlstr, 
               "select school_code, school_name "
	       "from school "
               "order by school_name;");

    rc = sqlite3_exec(db,sqlstr,school_codes_cback,0,&zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"SQLerror:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }

    return rc;
}

// Now for the main function which ties it all together.
// 


int open_presscon_database(void)
{

    int rc             {SQLITE_ERROR};

    /* Check if database is present. */
    rc = sqlite3_open("presscon.db",&db);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"Can't open database:%s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    return rc;
}


int close_presscon_database(void)
{
    sqlite3_close(db);
    return 0;
}

int main(int argc, char **argv){
    int rc              {SQLITE_ERROR};
    char sqlstr[1024]   {0x00};
    char *str_ptr       {nullptr};
    int retval          {SQLITE_ERROR};

    int stdout_flag             {true};
    int append_flag             {false};
    char outputfile_name[1024]  {"output.tex"};


    open_presscon_database();

    // Process all the schools.
    print_LaTeX_limbo();
    process_school_codes();
    print_LaTeX_endtext();

    close_presscon_database();

#ifdef OLDVERSION

    /* Set the default value for the flags. */
    list_only_flag = FALSE;  // list_only_flag is a global variable.
    stdout_flag = FALSE;      // Do not send output to stdout.
    append_flag = FALSE;      // Do not append; overwrite whatever is in the output file.
                              // Obviously, this does not have any effect if the output
                              // is stdout.


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

#endif  // OLDVERSION

    return 0;
}
