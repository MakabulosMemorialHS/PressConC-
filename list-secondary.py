#!/usr/bin/env python
#
# list-secondary.py
#
# Create LaTeX document for secondary students.
# Invocation:
#    ./getwinners.sh | grep -i secondary | ./list-secondary.py > temp.tex
#

import string

def print_zombie():
    print "\\documentclass[12pt]{article}"
    print "\\usepackage{palatino}"
    print "\\advance\\voffset by -1 in"
    print "\\advance\\hoffset by -0.5 in"
    print "\\advance\\textwidth by 1 in"
    print "\\begin{document}"
    print "\\pagestyle{empty}"
    print "\\fontsize{12}{18}"
    print "\\selectfont"

def print_letterhead():
    print "\\centerline{\\bf\\Large ALLIANCE OF PRIVATE SCHOOLS IN TARLAC PROVINCE}"
    print "\\vspace {0.5 in}"
    print "\\centerline{\\Large DIVISION PRESS CONFERENCE}"
    print "\\centerline{\\Large List of Participants}"
    print "\\centerline{\\Large High School Level}"
    print "\\vspace {0.5 in}"


contest_code = "START"
categories_printed = 0
names_printed = 0


print_zombie()

while True:
    try:
        if categories_printed == 0:
            print_letterhead()
            lines_printed = 5

        x = raw_input()
        xspl = string.split(x, '|')

        rank = int (xspl[5])

        if rank > 6:
            continue;

        if contest_code != xspl[0]:
            if names_printed != 0:
                print "\\end{tabular}"
                print "\\vspace {0.2 in}"
                print ""
                names_printed = 0
                categories_printed = categories_printed + 1
                if categories_printed > 4:
                    print "\\vfill\\eject"
                    categories_printed = 0
                    print_letterhead()

            contest_code = xspl[0]
            contest_name = xspl[1] + " " + xspl[2] + " " + xspl[3]
            print "\\leftline{\\bf %s}" % (contest_name)
            print "\\begin{tabular}[t]{rp{2.5in}l}"
            categories_printed = categories_printed + 1

        last_name = xspl[7]
        first_name = xspl[8]
        school = xspl[10]
        print "%d&\\bf %s, %s\\hfil & %s\\hfil\\\\" % (rank, last_name, first_name, school)
        lines_printed = lines_printed + 1
        names_printed = names_printed + 1

    except EOFError:
        if names_printed != 0:
            print "\\end{tabular}"
        break

print "\\end{document}"

 

