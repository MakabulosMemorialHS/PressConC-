/* *************************************************************
 * presscon.h
 *
 * Contains identifiers and constants common to all
 * utilities in this package.
 *
 * Revision History:
 * $Id: presscon.h,v 1.7 2015/09/06 00:40:50 robert Exp $
 *
 * $Log: presscon.h,v $
 * Revision 1.7  2015/09/06 00:40:50  robert
 * No comment.
 *
 * Revision 1.6  2014/09/08 13:17:17  robert
 * Modified for the PSPC 2014.
 *
 * Revision 1.5  2013/09/23 17:59:45  robert
 * Added #define for TRUE and FALSE.
 *
 * Revision 1.4  2012/09/13 04:18:20  robert
 * Changed competition dates for Sept 2012.
 *
 * Revision 1.3  2011/09/22 09:38:12  robert
 * Readied for Press Competition of 23 September 2011.
 *
 * Revision 1.2  2010/09/21 05:02:16  robert
 * Changed dates.
 *
 * Revision 1.1  2009/08/05 03:43:20  robert
 * Initial import into CVS.
 *
 *
 * *************************************************************/

#define APSTAP_NAME     "ALLIANCE OF PRIVATE SCHOOLS IN TARLAC PROVINCE"
#define CONTEST_DATES   "October 19, 2022"


// The following defines is superfluous in C++ since C++
// already defines bool true and bool false.
//
#define TRUE            1
#define FALSE           0

/* **************************************************************************
 * FUNCTION DECLARATIONS
 * **************************************************************************/

void banner(void);

void print_LaTeX_limbo(void);
void print_LaTeX_endtext(void);
void print_page_headers(void);
void print_page_footers(void);

void process_contest_winners(char *);
void process_contest_codes(void);
int process_school_codes(void);


int school_codes_cback(void *NotUsed, int argc, char **argv, char **azColName);
int contest_winners_cback(void *NotUsed, int argc, char **argv, char **azColName);
int contest_code_cback(void *NotUsed, int argc, char **argv, char **azColName);


