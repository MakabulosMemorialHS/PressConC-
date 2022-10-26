/*
 *
 * search.c
 *
 * This application will search for a given strings
 * in the first name, last name and pen_name fields.
 * This application is useful for checking pen names and
 * names of contestants if they are already present in the
 * database.
 *
 * *********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static char str_to_search[128];

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i, len;
    char result_str[1028];
    char copy_of_str[1028];
    
    result_str[0] = 0x00;
    strcat(result_str, argv[0]);
    strcat(result_str, ", ");
    strcat(result_str, argv[1]);
    strcat(result_str, " - ");
    strcat(result_str, argv[2]);

    strcpy(copy_of_str, result_str);
    len = strlen(copy_of_str);
    for (i = 0; i < len; ++i)
	copy_of_str[i] = tolower(copy_of_str[i]);
    
    if (strstr(copy_of_str, str_to_search) != NULL)
	printf("   %s\n", result_str);
    
    return 0;
}

static int school_search(void *NotUsed, int argc, char **argv, char **azColName){
    int i, len;
    char result_str[1028];
    char copy_of_str[1028];
    
    result_str[0] = 0x00;
    strcat(result_str, argv[0]);
    strcat(result_str, " - ");
    strcat(result_str, argv[1]);

    strcpy(copy_of_str, result_str);
    len = strlen(copy_of_str);
    for (i = 0; i < len; ++i)
	copy_of_str[i] = tolower(copy_of_str[i]);
    
    if (strstr(copy_of_str, str_to_search) != NULL)
	printf("   %s\n", result_str);
 
    return 0;
}

static int contest_search(void *NotUsed, int argc, char **argv, char **azColName){
    int i, len;
    char result_str[1028];
    char copy_of_str[1028];
    
    result_str[0] = 0x00;
    strcat(result_str, argv[0]);
    strcat(result_str, " ");
    strcat(result_str, argv[1]);
    strcat(result_str, " ");
    strcat(result_str, argv[2]);
    strcat(result_str, " - ");
    strcat(result_str, argv[3]);
    
    strcpy(copy_of_str, result_str);
    len = strlen(copy_of_str);
    for (i = 0; i < len; ++i)
	copy_of_str[i] = tolower(copy_of_str[i]);
    
    if (strstr(copy_of_str, str_to_search) != NULL)
	printf("   %s\n", result_str);
 
    return 0;
}

int main(int argc, char **argv){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char sql_string[1024];
    char *str_ptr;
    
    printf("search: Search for strings in the data tables\n\n");

    rc=sqlite3_open("presscon.db",&db);
    if(rc){
        fprintf(stderr,"Can't open database:%s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    while (1) {
	// Describe the string to be searched
	// and convert it to lower case.
	printf("Enter string to search: ");
	fgets(str_to_search, 128, stdin);
	putchar('\n');
	str_ptr = str_to_search;
	while (*str_ptr != 0x00) {
	    if (*str_ptr == '\n') *str_ptr = 0x00;
	    *str_ptr = tolower(*str_ptr);
	    ++str_ptr;
	}
	if (strlen(str_to_search) == 0) break;
	
	// Search for the described string in the student table.
	// 
	printf("Individuals found\n");
	rc = sqlite3_exec(db,
		"select last_name, first_name, pen_name "
		"from student order by last_name, first_name;",
		    callback,0,&zErrMsg);
	if (rc != SQLITE_OK) {
	    fprintf(stderr,"SQLerror:%s\n",zErrMsg);
	    sqlite3_free(zErrMsg);
	}
	
	// Then try the search in the school table.
	// 
	printf("\nSchools found\n");
	rc = sqlite3_exec(db,
		"select school_name, school_code "
		"from school order by school_name;",
		    school_search,0,&zErrMsg);
	if (rc != SQLITE_OK) {
	    fprintf(stderr,"SQLerror:%s\n",zErrMsg);
	    sqlite3_free(zErrMsg);
	}

	// And also try the search in the contest table.
	// We've got almost everything covered!
	// 
	printf("\nContest codes\n");
	rc = sqlite3_exec(db,
		"select contest_name, language, level, contest_code "
		"from contest order by contest_name, language, level;",
		    contest_search,0,&zErrMsg);
	if (rc != SQLITE_OK) {
	    fprintf(stderr,"SQLerror:%s\n",zErrMsg);
	    sqlite3_free(zErrMsg);
	}
	printf("\n\n");
    }
    
    printf("\nEnd Search.\n"); 
    sqlite3_close(db);
    
    return 0;
}
