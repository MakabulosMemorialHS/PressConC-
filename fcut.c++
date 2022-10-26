/* *********************************************************************
 * fcut.c++
 *
 * My version of the cut command. The main difference between this fcut
 * and the standard cut is that this version can reorder columns.
 * Also, this fcut assumes the input is some version of the
 * comma or tab delimited file.
 *
 * *********************************************************************/

#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdlib>

#define MAXLINESIZE    5024
#define SEPARATOR      '|'
#define MAXFIELDS      128


inline char *remove_nl(char *target)
{
    int slen = strlen(target);
    char *p = target + slen-1;
    while (*p != '\n') p--;
    *p = 0x00;
    return target;
}


// Given a string, split the string along
// the given separator.
// You should send a pointer to a buffer long enough to
// hold source in cpybuff.
// The individual strings are indexed by sp_index.
// A copy of source is placed in cpybuff so that
// this function will not change its arguments.

inline int split(char *source, char *sp_index[], char *cpybuff, char sep)
{
    unsigned int cindex {0};      // The current index.
    sp_index[0] = nullptr;

#ifdef DEBUG
    char *src = source;
    char *cpy = cpybuff;
#endif
    if (*source != 0x00) {
        sp_index[cindex++] = cpybuff;
    }

    while ((*cpybuff = *source++) != 0x00) {
        if (*cpybuff == sep) {
            *cpybuff = 0x00;
            sp_index[cindex++] = cpybuff + 1;
        }
        cpybuff++;
    };

#ifdef DEBUG
    printf("Contents of source : %s\n", src);
    printf("The fields of source contain:\n");
    for (int k = 0; k < cindex; ++k) {
        printf("[%d] %s\n", k, sp_index[k]);
    }
#endif
    return cindex;    // Return the number of fields found.
}

int main()
{
    char buffer[MAXLINESIZE];
    char tempb[MAXLINESIZE];
    char *index_to_fields[MAXFIELDS];

    int fpointers[MAXFIELDS] {4,1,3, -1};   // We want these fields in the output. -1 means 'no more'.
    int num_fields_in_output {3};         // There are two fields that we want in the output.

    int numfields {0}; // Don't know if this is even useful?
    char *retval;      // Can we safely ignore these return values?

    while ((retval = fgets(buffer, MAXLINESIZE-1, stdin)) != 0x00) {
        retval = remove_nl(buffer);
        numfields = split(buffer, index_to_fields, tempb, '|');
        // Now print out the fields we want in the order
        // we want.
        for (int k = 0; k < num_fields_in_output; ++k) {
            printf("%s", index_to_fields[fpointers[k]]);
            if (k < num_fields_in_output - 1) putchar('|');
        }
        putchar('\n');
    }


}




