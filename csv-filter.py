#!/usr/bin/env python
#
# Read a CSV file of values and then print out SQL commands to insert information into the
# presscon.db database.
#
# The CSV File should have the following fields.
#
# 1. contest code
# 2. rank
# 3. points
# 4. pen name
# 5. first name
# 6. last name
# 7. school code
#
# The input should be redirected into stdin.
#
# This filter will output the following:
#
# a) SQL commands compatible with the schema of table student in presscon.db
# b) SQL commands compatible with the schema of the table sclink.
# c) SQL commands compatible with table wins.
#
# The output will be sent to stdout.
#
import string

while True:
    try:
        x = raw_input()
        xspl = string.split(x, '|')
        contest_code = xspl[0]
        rank = xspl[1]
        points = xspl[2]
        pen_name = string.strip(xspl[3])
        first_name = xspl[4]
        last_name = xspl[5]
        school_code = xspl[6]

        # SQL command to insert into table student
        print "insert into student values ('%s', '%s', '%s', '%s');" % (pen_name, last_name, first_name, school_code)

        # For sclink table.
        print "insert into sclink values ('%s', '%s');" % (pen_name, contest_code)

        # For wins table.
        print "insert into wins values ('%s', '%s', %s, %s);" % (contest_code, pen_name, rank, points)

    except EOFError:
        break

 
