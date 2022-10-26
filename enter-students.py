#!/usr/bin/env python
#
# enter-students.py
#
# Enters the pen names of students who joined the contest.
#
# Revision History:
#
# $Id: enter-students.py,v 1.2 2014/09/08 11:25:33 robert Exp $
#
# $Log: enter-students.py,v $
# Revision 1.2  2014/09/08 11:25:33  robert
# Changed size of selection box.
#
# Revision 1.1  2008/09/23 15:59:44  robert
# Renamed it to have the extension .py.
# It's kinda confusing determining which is an ELF file and
# which is a python script.
#
# Revision 1.5  2008/09/23 14:31:19  robert
# New and improved. Uses the TkInter library. We've got GUI!
#
# Revision 1.4  2008/09/23 10:41:28  robert
# Working version. Not thorougly tested yet but it seems to work.
#
# Revision 1.3  2008/09/22 06:52:53  robert
# Start of port to Tkinter.
#
#
import string
import tkFont
import sys
import os
from Tkinter import *

sqldb = os.popen("sqlite3 presscon.db \"select school_code, school_name from school order by school_name;\"", "r")
schools = sqldb.readlines()
sqldb.close()

class Application(Frame):
    def __init__(self, master=None):
	Frame.__init__(self, master)
	self.grid()
	self.semaphore = 0
	self.createWidgets()

    def get_school_name(self):
        # curselection() returns a tuple.
	index = int(self.schoolList.curselection()[0])
	strspl = string.split(schools[index], "|")
	self.school_code = string.strip(strspl[0])
	self.schoolEntry.delete(0,END)
	self.schoolEntry.insert(0, string.strip(strspl[1]))
	
    def get_entries(self):

        # Get the pen names and others
	pen_name = string.strip(self.penNameEntry.get())
	last_name = string.strip(self.lastNameEntry.get())
	first_name = string.strip(self.firstNameEntry.get())
	
	if (len(pen_name) == 0 or len(last_name) == 0 or len(first_name) == 0):
	    return
	# index = int(self.schoolList.curselection()[0])
	# strspl = string.split(schools[index], "|")
	sqlstr = "insert into student "\
	    "(pen_name, last_name, first_name, school_code) "\
	    "values (\'%s\', \'%s\', \'%s\', \'%s\');" % \
	    (pen_name, last_name, first_name, self.school_code)
        print sqlstr	    
	outfile = file("SQL.out", "a")
	outfile.write("%s\n" % sqlstr)
	outfile.close()

        # Clear the entries
	self.penNameEntry.delete(0, END)
	self.lastNameEntry.delete(0, END)
	self.firstNameEntry.delete(0, END)
	self.schoolEntry.delete(0, END)
        self.schoolList.selection_clear(0, END)
	
    def createWidgets(self):
        titlefont = tkFont.Font(family="new century schoolbook", size=14)
        textfont = tkFont.Font(family="new century schoolbook", size=12)
	
	self.fLabel = Label(self, \
	    text="APSTAP Press Conference\n"
	         "Enter pen names of students into the database.",\
		 font=titlefont)
	self.fLabel.grid(columnspan=2)
	
	self.penNameLabel = Label(self, text="Pen Name", font=textfont)
	self.penNameLabel.grid(row=4,column = 0)
	self.penNameEntry = Entry(self, width=20,font=textfont)
	self.penNameEntry.grid(row=4,column = 1)
	
	self.lastNameLabel = Label(self, text="Last Name",font=textfont)
	self.lastNameLabel.grid(row=5,column=0)
	self.lastNameEntry = Entry(self,font=textfont)
	self.lastNameEntry.grid(row=5,column=1)
	
	self.firstNameLabel = Label(self, text="First Name",font=textfont)
	self.firstNameLabel.grid(row=6,column=0)
	self.firstNameEntry = Entry(self,font=textfont)
	self.firstNameEntry.grid(row=6,column=1)

	self.schoolButton = Button(self, \
	     text="School",font=textfont,\
	     command=self.get_school_name)
	self.schoolButton.grid(row=7, column=0)
        self.schoolEntry = Entry(self, width=30,\
	     font=textfont)
        self.schoolEntry.grid(row=7, column=1, columnspan=2)
	
	self.yScroll  =  Scrollbar (self, orient=VERTICAL)
	self.yScroll.grid (row=8, column=2, sticky=N+S)
	self.xScroll  =  Scrollbar (self, orient=HORIZONTAL)
	self.xScroll.grid (row=9, column=0, columnspan=2, sticky=E+W)
	
	self.schoolList = Listbox(self, \
	      selectmode=SINGLE, \
	      height=15,width=40,\
	      xscrollcommand=self.xScroll.set,\
	      yscrollcommand=self.yScroll.set,font=textfont)
	self.schoolList.grid(row=8,column=0,columnspan=2)
        self.xScroll["command"] = self.schoolList.xview
        self.yScroll["command"] = self.schoolList.yview
	self.schoolList.selection_set(0)

        # Note that we don't put parens on the function name. That's
	# because putting parens on them will call the function.
	# Without the parens, we are just telling python the
	# name of the function. Kinda like pointers to functions
	# with C.
        self.OKButton = Button(self, text="Save Entry", \
	     command=self.get_entries,font=textfont)
	self.OKButton.grid(row=14,column=0)
	
        self.ExitButton = Button(self, text="Exit", font=textfont,\
	     command = self.quit)
	self.ExitButton.grid(row=14,column=1)
	
        for i in range(0,len(schools)):
            xstr = string.split(schools[i], "|")
	    schxn = string.replace(xstr[1], "\n", "")
            self.schoolList.insert(END, schxn)


#########################################################################
# Start of Main routine
#########################################################################
app = Application()
app.master.title("APSTAP Press Conference")
# print tkFont.families()
app.mainloop()
sys.exit()

