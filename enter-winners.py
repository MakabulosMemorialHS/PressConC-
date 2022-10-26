#!/usr/bin/env python
#
# winners.py
#
# Enter the winners of each event. You will need to enter
# only the contest_code, the pen_name, the place and the number
# of points given.
#
# Revision History:
# $Id: enter-winners.py,v 1.6 2014/09/08 11:26:42 robert Exp $
#
# $Log: enter-winners.py,v $
# Revision 1.6  2014/09/08 11:26:42  robert
# Change point system. Expanded size of selection box.
#
# Revision 1.5  2013/09/23 17:57:29  robert
# Changed formula for computing the number of points.
#
# Revision 1.4  2012/09/09 11:52:48  robert
# The new Press Conference will have 7 winners. So the points system was changed.
#
# Revision 1.3  2008/09/26 11:53:38  robert
# Minor changes only. This program was used during the
# 2008 Press Conference.
#
# Revision 1.2  2008/09/24 09:39:55  robert
# Changed color of background.
#
# Revision 1.1  2008/09/23 16:00:39  robert
# Renamed to have a .py extension. Getting confused between which
# is an ELF file and which is a python script.
#
#
import string
import tkFont
import sys
import os
from Tkinter import *

sqldb = os.popen("sqlite3 presscon.db \"select * from school order by school_name;\"", "r")
schools = sqldb.readlines()
sqldb.close()

sqldb = os.popen("sqlite3 presscon.db \
     \"select contest_code, contest_name, language, level "
     "from contest order by contest_name, language, level;\"", "r")
contest_list = sqldb.readlines()
sqldb.close()

sqldb = os.popen("sqlite3 presscon.db \
     \"select pen_name, first_name, last_name, school_code "
     "from student order by pen_name;\"", "r")
pen_name_list = sqldb.readlines()
sqldb.close()

class Application(Frame):
    def __init__(self, master=None):
	Frame.__init__(self, master)
	self.grid()
	self.createWidgets()

    def get_contest_code(self):
	## Code looks strange below because .curselection returns
	## a tuple. We take only the first entry in the tuple.
	index = int(self.contestListbox.curselection()[0])
	xsdf = string.split(contest_list[index],"|")

        ## The next line will print to the console. It's good
	## for debugging purposes.
	## print xsdf
	self.contest_code = xsdf[0]
	self.text0.delete(0,END)
	self.text0.insert(0,self.contest_code)

    def get_student_name(self):
	index = int(self.pen_nameListbox.curselection()[0])
	xsdf = string.split(pen_name_list[index],"|")
	# print xsdf
	self.pen_name  = xsdf[0]
	full_name = xsdf[1] + " " + xsdf[2]
	self.text1.delete(0,END)
	self.text1.insert(0,full_name)

    def set_points(self):
	# Get the place earned.
	place = int(string.strip(self.placeEntry.get()))
        pernd = 11 - place # points earned
	if pernd < 0:
	    pernd = 0
	self.pointEntry.delete(0,END)
	self.pointEntry.insert(0,"%d" % (pernd))
	
    def save_entries(self):

        # Get entries to be saved.
	pen_name = string.strip(self.pen_name)
	if len(pen_name) == 0:
	    return;
	contest_code = string.strip(self.contest_code)
        place = string.strip(self.placeEntry.get())
        points = string.strip(self.pointEntry.get())
	
        sqlstr = "insert into wins (contest_code, pen_name, place, points) "\
	    "values (\'%s\', \'%s\', %d, %d); " %\
	    (contest_code, pen_name, int(place), int(points))
	print sqlstr
	
	outfile = file("SQL.out", "a")
	outfile.write("%s\n" % sqlstr)
	outfile.close()

        # Clear the entries
	# self.text0.delete(0, END)
	self.text1.delete(0, END)
	self.placeEntry.delete(0, END)
	self.pointEntry.delete(0, END)
	
    def createWidgets(self):
        titlefont = tkFont.Font(family="new century schoolbook", size=14)
        textfont = tkFont.Font(family="new century schoolbook", size=12)

        ## The dialog box banner
	##
	self.fLabel = Label(self, \
	    text="Winners Database.",\
		 font=titlefont)
	self.fLabel.grid(columnspan=2)

        ## List box of Contest Codes.
	##
        # Note that we don't put parens on the function name. That's
	# because putting parens on them will call the function.
	# Without the parens, we are just telling python the
	# name of the function. Kinda like pointers to functions
	# with C.
	self.button0 = Button(self, \
	    text="Contest Code",\
	         command=self.get_contest_code,\
		 font=titlefont)
	self.button0.grid(row=3, column=0)
	self.text0 = Entry(self, width=30, font=textfont)
	self.text0.grid(row=3, column=1, columnspan=2)
	
	self.ccyScroll  =  Scrollbar (self, orient=VERTICAL)
	self.ccyScroll.grid (row=4, column=2, sticky=N+S)
	self.ccxScroll  =  Scrollbar (self, orient=HORIZONTAL)
	self.ccxScroll.grid (row=5, column=0, columnspan=2, sticky=E+W)
	
	self.contestListbox = Listbox(self, \
	      selectmode=SINGLE, \
	      height=5,width=40,\
	      xscrollcommand=self.ccxScroll.set,\
	      yscrollcommand=self.ccyScroll.set,font=textfont)
	self.contestListbox.activate(0)
	self.contestListbox.grid(row=4,column=0,columnspan=2)
        self.ccxScroll["command"] = self.contestListbox.xview
        self.ccyScroll["command"] = self.contestListbox.yview

        for i in range(0,len(contest_list)):
            xstr = string.split(contest_list[i], "|")
	    ## We remove the trailing \\n.
	    nkstr = string.replace(xstr[3], "\n", "")
	    outstr = xstr[1] + " " + xstr[2] + " " + nkstr 
            self.contestListbox.insert(END, outstr)


        ## List box of Pen Names.
	##
	self.button1 = Button(self, \
	    text="Student Name",\
		 command=self.get_student_name,\
		 font=titlefont)
	self.button1.grid(row=10, column=0)
	self.text1 = Entry(self, width=30, font=textfont)
	self.text1.grid(row=10, column=1, columnspan=2)
	
	self.pnyScroll  =  Scrollbar (self, orient=VERTICAL)
	self.pnyScroll.grid (row=12, column=2, sticky=N+S)
	self.pnxScroll  =  Scrollbar (self, orient=HORIZONTAL)
	self.pnxScroll.grid (row=13, column=0, columnspan=2, sticky=E+W)
	
	self.pen_nameListbox = Listbox(self, \
	      selectmode=SINGLE, \
	      height=10,width=40,\
	      xscrollcommand=self.pnxScroll.set,\
	      yscrollcommand=self.pnyScroll.set,font=textfont)
	self.pen_nameListbox.grid(row=12,column=0,columnspan=2)
        self.pnxScroll["command"] = self.pen_nameListbox.xview
        self.pnyScroll["command"] = self.pen_nameListbox.yview
	self.pen_nameListbox.selection_set(0)

        for i in range(0,len(pen_name_list)):
            xstr = string.split(pen_name_list[i], "|")
	    ## We remove the trailing \\n.
	    nkstr = string.replace(xstr[3], "\n", "")
	    outstr = xstr[1] + " " + xstr[2] + " " + nkstr 
            self.pen_nameListbox.insert(END, xstr[0])

	    
	self.placeLabel = Label(self, text="Place", font=textfont)
	self.placeLabel.grid(row=18,column = 0)
	self.placeEntry = Entry(self, width=2,font=textfont)
	self.placeEntry.grid(row=18,column = 1)
	
	self.pointButton = Button(self,\
	       text="Points Earned",\
	       command=self.set_points,\
	       font=textfont)
	self.pointButton.grid(row=19,column = 0)
	self.pointEntry = Entry(self, width=2,font=textfont)
	self.pointEntry.grid(row=19,column = 1)
#	
#	self.lastNameLabel = Label(self, text="Last Name",font=textfont)
#	self.lastNameLabel.grid(row=5,column=0)
#	self.lastNameEntry = Entry(self,font=textfont)
#	self.lastNameEntry.grid(row=5,column=1)
#	
#	self.firstNameLabel = Label(self, text="First Name",font=textfont)
#	self.firstNameLabel.grid(row=6,column=0)
#	self.firstNameEntry = Entry(self,font=textfont)
#	self.firstNameEntry.grid(row=6,column=1)
#
#	self.schoolLabel = Label(self, text="School",font=textfont)
#	self.schoolLabel.grid(row=7, column=0, columnspan=2)
#

	## self.separatorX = Label(self, \
	##    text="------------------------",\
	##	 font=textfont)
	## self.separatorX.grid(column=0, columnspan=2)

        self.OKButton = Button(self, text="Save Entry", \
	     command=self.save_entries,font=textfont)
	self.OKButton.grid(row=22,column=0)
	
        self.ExitButton = Button(self, text="Cancel", font=textfont,\
	     command = self.quit)
	self.ExitButton.grid(row=22, column=1)
	


#########################################################################
# Start of Main routine
#########################################################################
app = Application()
app.master.title("APSTAP Winners")
# print tkFont.families()
app.mainloop()
sys.exit()









outfile = file("SQL.out", "a")

rpstr = raw_input("Enter the contest code: ")
contest_code = string.upper(rpstr)
inum = int (raw_input("How many winners to enter? "))
print ""
print "Enter each winner separately"
for i in range(0,inum):
    rpstr = raw_input("   Enter the pen name: ")
    pen_name = string.lower(rpstr)
    place = int (raw_input("   Enter place: "))
    points = int (raw_input("   Enter points earned: "))

    sqlstr = "insert into wins " + \
	     "(contest_code, pen_name, place, points) " + \
	     "values(\'%s\', \'%s\', %d, %d);\n" % \
	     (contest_code, pen_name, place, points)
    ans = raw_input('OK to save? (y/n) ')
    if ans[0] == 'y':
	outfile.write(sqlstr)
    else:
	print 'Data entry cancelled!'
    print ""

outfile.close()


