#!/bin/bash
#
# Dump the winners ordered by category.
#
sqlite3 presscon.db "select * from contest natural join wins natural join student natural join school order by contest.contest_code, wins.place;"
