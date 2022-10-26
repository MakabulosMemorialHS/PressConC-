/*
 *
 * print-rankings.c
 *
 * Prints a nicely formatted LaTeX page of the overall rankings
 * in the Press Conference. The rankings are divided into the two
 * levels of Elementary and Secondary.
 *
 * Running this program will result in a LaTeX file output.tex
 * on the present directory.
 * 
 * Revision History
 *
 * $Id: print-rankings.c,v 1.8 2009/08/31 05:24:28 robert Exp $
 *
 * $Log: print-rankings.c,v $
 * Revision 1.8  2009/08/31 05:24:28  robert
 * print-rankings now gives the correct rank when there are
 * ties in the ranking. Previously, print-rankings prints ties
 * in alphabetical order.
 *
 * Revision 1.7  2009/08/05 03:40:41  robert
 * Made the organization name and contest dates into manifest
 * constants.
 *
 * Revision 1.6  2008/09/26 11:57:37  robert
 * Found an error in the comparison of strings. Previously, the function
 * used during comparison was strstr() but this led to problems with
 * CAMCS and MCS because both school codes have a common substring.
 *
 * The use of strcmp() instead of strstr() eliminated this common
 * substring problem.
 *
 * Revision 1.5  2008/09/23 10:40:33  robert
 * Repaired SQL error dues to ambiguous call to select school_code.
 * Made it school.school_code.
 *
 * Revision 1.4  2008/09/22 12:27:32  robert
 * Changed name of organization and the date of the press con.
 *
 * Revision 1.3  2006/09/29 01:00:16  robert
 * This is the version that was actually used during FeTAPS.
 * I can'r remember what changes were made between the last
 * version and this version.
 *
 * Revision 1.2  2006/09/21 23:18:01  robert
 * Corrected the display of the time of print. The correct
 * time is now displayed in the output file.
 *
 * Revision 1.1  2006/09/21 19:38:09  robert
 * Initial import into CVS. Working version but date string at
 * bottom of output LaTeX file is arbitrary. Need to change that
 * or perhaps delete it while it doesn't work correctly yet.
 *
 * Except for the date, the output is already correct.
 *
 *
 * *********************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sqlite3.h>
#include "presscon.h"

#define  ARRAY_SIZE   1024

struct rank_atom {
    float school_rank;        // What's my rank?
    char school_code[16];
    int total_points;
};


/* ***********************************************************
 * Declare the global variables we will be using a lot.
 *
 * ***********************************************************/

static struct rank_atom rank_array[ARRAY_SIZE];
static FILE *texfile;
static sqlite3 *db;
char CURRENT_TIME[128];


/* ***********************************************************
 * We will need the following variables to
 * pass int values from
 * one function to another. Akin to errno.
 * We need this kludge because the nature
 * of callback functions makes it difficult
 * to pass values from the calling function
 * to the callback without resorting to global
 * variables.
 * Be careful with this one. Make sure that
 * the value of temp_val is indeed as one expects
 * it to be.
 * ***********************************************************/

static int temp_val;
static float temp_val2;


/* ***********************************************************
 * start of function definitions
 * ***********************************************************/

/* ***********************************************************
 * Function to test sorting.
 * ***********************************************************/
void print_rank_array(void)
{
    int i;

    for (i = 0; i < ARRAY_SIZE && (rank_array[i].school_code)[0] != 0x00; ++i) {
	printf("Code = %10s, points = %3d\n",
		rank_array[i].school_code, rank_array[i].total_points);
    }
}


/* **********************************************************
 * Adjust the rankings within the rank array to take care
 * of ties. This function must be called after sorting
 * rank_array[].
 * **********************************************************/
void adjust_rank_array(void)
{
    int trailing_index, forward_index;
    int tie_count;
    float current_rank, rank_sum;


    trailing_index = 0;
    current_rank = 1.0;

    while (trailing_index < ARRAY_SIZE && 
	      rank_array[trailing_index].school_code[0] != 0x00) {

	forward_index = trailing_index + 1;

	// We search for the next item with a different number of
	// points from the current item.
        while(rank_array[forward_index].school_code[0] != 0x00 &&
	      rank_array[forward_index].total_points ==
		  rank_array[trailing_index].total_points)
	    ++forward_index;

	// Found the next different score. How far away is it
	// from the current index? We call that difference the
	// tie_count.
	tie_count = forward_index - trailing_index;
        
	// The ranks follow an arithmetic progression
	// with a0 = current_rank and an = current_rank + tie_count - 1
	rank_sum = (2*current_rank + tie_count - 1)/2;

        while (trailing_index < forward_index) 
	    rank_array[trailing_index++].school_rank = rank_sum;

	current_rank = forward_index+1;
   }
	  
}

// This function sets the current time.
// 
void get_current_time(void)
{
    struct tm *p_time;
    time_t x_time;

    x_time = time(NULL);
    p_time = localtime(&x_time);
    strcpy(CURRENT_TIME, asctime(p_time));
}



// This function clears the rank array by setting
// each total_points member to zero and blanking the
// first character of each school_code character string.
// 
int clear_rank_array(void)
{
    int i;
    
    for (i = 0; i < ARRAY_SIZE; ++i) {
	(rank_array[i].school_code)[0] = 0x00;
	rank_array[i].total_points = 0;
	rank_array[i].school_rank = 0;
    }

    return 0;
}

// This next function searches the rank array for a particular
// school_code. The formal parameter s_code is a pointer to
// the school_code.
// 
// This function returns the index of the element that holds
// the atom for the searched school_code. It returns -1 on failure.
// 
int search_rank_array(char *s_code)
{
    int i;
    
    for (i = 0; i < ARRAY_SIZE && (rank_array[i].school_code)[0] != 0x00;
	    ++i) 
	if (strstr(rank_array[i].school_code, s_code) != NULL) 
	    return i;

    return -1;
}

// This function increments the number of points of a particular
// school by the given amount. It returns the total points that
// the school has after the increment.
// 
int increment_points(char *s_code, int t_points)
{
    int i;
    
    for (i = 0; i < ARRAY_SIZE && (rank_array[i].school_code)[0] != 0x00;
	    ++i) 
	if (strcmp(rank_array[i].school_code, s_code) == 0) {
	    rank_array[i].total_points += t_points;
	    return rank_array[i].total_points;
	}

    // If we reach this point then no entry for the s_code has
    // been found. i is then the index of the first empty element
    // in rank_array. We 
    
    strcpy(rank_array[i].school_code, s_code);
    rank_array[i].total_points = t_points;

    return rank_array[i].total_points;
}

// Sort rank_array in descending order of total_points.
// The sorting algorithm is a simple selection sort which should
// work fine in this application of less than a dozen elements!
// 

int sort_rank_array(void)
{
    int i, j, max;
    char temp_str[16];
    int temp_points;
   
    for (i = 0; i < ARRAY_SIZE && (rank_array[i].school_code)[0] != 0x00;
	    ++i) {
	max = i;
	for (j = i+1; j < ARRAY_SIZE && (rank_array[j].school_code)[0] != 0x00; 
		++j) 
	    if (rank_array[j].total_points > rank_array[max].total_points) max = j;

	// Swap the total points
	temp_points = rank_array[i].total_points;
	rank_array[i].total_points = rank_array[max].total_points;
	rank_array[max].total_points = temp_points;

	// Swap the school_codes
	strcpy(temp_str, rank_array[i].school_code);
	strcpy(rank_array[i].school_code, rank_array[max].school_code);
	strcpy(rank_array[max].school_code, temp_str);
    }

    return 0;
}


// The following callback will be used by the function
// print_array_contents().
// This callback uses the temp_val and temp_val2 global passing variables.
// Oooh! Playing with global values eh? Keep yer fingers crossed
// no gamma ray changes the value of temp_val b'tween this callback
// and the caller! 
// 
static int helper_0(void *NotUsed, int argc, char **argv, char **azColName)
{
    fprintf(texfile, "%3.1f&\\bf %s&%d pts\\\\\n", temp_val2, argv[0], temp_val);
    
    return 0;
}

static int print_array_contents(void)
{
    char *zErrMsg = 0;
    int rc;
    int i;
    char sql_query[1024];

    for (i = 0; i < ARRAY_SIZE && (rank_array[i].school_code)[0] != 0x00; ++i) {
	sprintf(sql_query, 
		"select school_name, school_code "
		"from school "
		"where school_code=\'%s\';", rank_array[i].school_code);

	// Pass total number of points and the rank to
	// the helper callback helper_0()
	// 
	temp_val = rank_array[i].total_points;
        temp_val2 = rank_array[i].school_rank;

	// Execute the query and hoping that the callback gets
	// the rank and the total number of points.
	// 
        rc=sqlite3_exec(db,sql_query,helper_0,0,&zErrMsg);
	if (rc!=SQLITE_OK) {
	    fprintf(stderr,"SQLerror:%s\n",zErrMsg);
	    sqlite3_free(zErrMsg);
	}
    }

    return 0;
}

static int fill_rank_array(void *NotUsed, int argc, char **argv, char **azColName)
{
    increment_points(argv[0], atoi(argv[1]));
	    
    return 0;
}


// The next function will rank the schools on the elementary level.
// Before calling this function, make sure that the database
// has already been opened.
static int rank_elementary_level(void)
{
    char *zErrMsg = 0;
    int rc;
    char *sql_elem_str = "select school.school_code, points "
	"from school natural join student natural join wins natural join contest "
	"where level='Elementary';";

    clear_rank_array();   // Clears the array first
    
    rc=sqlite3_exec(db,sql_elem_str,fill_rank_array,0,&zErrMsg);
    if (rc!=SQLITE_OK) {
        fprintf(stderr,"SQLerror:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sort_rank_array();
    adjust_rank_array();

    fprintf(texfile, "\\documentclass[12pt]{article}\n"
	"\\usepackage{palatino}\n"
	"\\advance\\voffset by -1 in\n"
	"\\advance\\hoffset by -0.5 in\n"
	"\\advance\\textwidth by 1 in\n"
	"\\begin{document}\n"
	"\\pagestyle{empty}\n"
	"\\fontsize{12}{18}\n"
	"\\selectfont\n"
	"\\centerline{\\bf\\Large %s}\n"
	"\\centerline{\\bf\\Large PRESS CONFERENCE}\n"
	"\\centerline{\\large %s}\n"
	"\\vskip 0.5 in\n"
	"\\centerline{\\Large Ranking -- Elementary Level}\n"
	"\\vskip 0.5 in\n"
	"\\begin{tabular}[t]{rlr}\n", APSTAP_NAME, CONTEST_DATES);

    print_array_contents();

    fprintf(texfile, "\\end{tabular}\n"
	"\\vskip 1 in\n"
	"\\centerline{Ranking based on results as of}\n"
	"\\centerline{%s}\n"
	"\\vfill\\eject\n", CURRENT_TIME);

    return 0;
}


// The following function ranks the secondary level.
//
static int rank_secondary_level(void)
{
    char *zErrMsg = 0;
    int rc;
    char *sql_elem_str = "select school.school_code, points "
	"from school natural join student natural join wins natural join contest "
	"where level='Secondary' order by school.school_code;";

    clear_rank_array();   // Clears the array first
    
    rc=sqlite3_exec(db,sql_elem_str,fill_rank_array,0,&zErrMsg);
    if (rc!=SQLITE_OK) {
        fprintf(stderr,"SQLerror:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sort_rank_array();
    adjust_rank_array();

    fprintf(texfile, 
	"\\centerline{\\bf\\Large %s}\n"
	"\\centerline{\\bf\\Large PRESS CONFERENCE}\n"
	"\\centerline{\\large %s}\n"
	"\\vskip 0.5 in\n"
	"\\centerline{\\Large Ranking -- Secondary Level}\n"
	"\\vskip 0.5 in\n"
	"\\begin{tabular}[t]{rlr}\n", APSTAP_NAME, CONTEST_DATES);

    print_array_contents();

    fprintf(texfile, "\\end{tabular}\n"
	"\\vskip 1 in\n"
	"\\centerline{Ranking based on results as of}\n"
	"\\centerline{%s}\n"
	"\\end{document}\n", CURRENT_TIME);

    return 0;
}


int main(int argc, char **argv){
    int rc;

    get_current_time();
    rc=sqlite3_open("presscon.db",&db);
    if(rc){
        fprintf(stderr,"Can't open database:%s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    texfile = fopen("output.tex", "w");

    rank_elementary_level();
    rank_secondary_level();

    sqlite3_close(db);
    fclose(texfile);
    
    return 0;
}
