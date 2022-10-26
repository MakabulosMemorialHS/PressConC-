#!/usr/bin/env python
#
# psr.py
#
# GUI front-end for the print-per-school utility.
# Front ends are glamorous, but they don't much work!

# Revision History:
#
# $Id: psr.py,v 1.4 2014/09/08 13:13:42 robert Exp $
#
# $Log: psr.py,v $
# Revision 1.4  2014/09/08 13:13:42  robert
# Modified so that clicking on "Print Schools Report" will invoke
# pdflatex and evince.
#
# Revision 1.3  2013/09/23 18:10:40  robert
# Modified so that this program deletes output.tex before
# calling the program which creates the new output.tex.
#
# This modification was needed because the formatting utilities
# have been modified to not delete the old output.tex before
# formatting a new document.
#
# Admittedly, this is a kludge and this package must be
# modified.
#
# Revision 1.2  2011/09/22 09:37:01  robert
# Version of 21 Sept 2011. Will be used for the Press Competition of
# 23 Sept 2011.
#
# Revision 1.1  2008/09/24 14:54:04  robert
# Initial import to CVS.
# Works as expected.
#
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
        os.system("rm output.tex")
        os.system("./print-per-school %s > output.tex" % (self.school_code))
	os.system("pdflatex output && evince output.pdf")
	
	self.schoolEntry.delete(0, END)
        self.schoolList.selection_clear(0, END)
	
    def createWidgets(self):
        titlefont = tkFont.Font(family="new century schoolbook", size=14)
        textfont = tkFont.Font(family="new century schoolbook", size=12)
	
	self.fLabel = Label(self, \
	    text="Print Report Per School.",\
		 font=titlefont)
	self.fLabel.grid(columnspan=2)
	
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
	      height=10,width=40,\
	      xscrollcommand=self.xScroll.set,\
	      yscrollcommand=self.yScroll.set,font=textfont)
	self.schoolList.grid(row=8,column=0,columnspan=2)
        self.xScroll["command"] = self.schoolList.xview
        self.yScroll["command"] = self.schoolList.yview

        for i in range(0,len(schools)):
            xstr = string.split(schools[i], "|")
	    schxn = string.replace(xstr[1], "\n", "")
            self.schoolList.insert(END, schxn)

	self.schoolList.selection_set(0)

        # Note that we don't put parens on the function name. That's
	# because putting parens on them will call the function.
	# Without the parens, we are just telling python the
	# name of the function. Kinda like pointers to functions
	# with C.
        self.OKButton = Button(self, text="Prepare Report", \
	     command=self.get_entries,font=textfont)
	self.OKButton.grid(row=14,column=0)
	
        self.ExitButton = Button(self, text="Exit", font=textfont,\
	     command = self.quit)
	self.ExitButton.grid(row=14,column=1)
	

#########################################################################
# Start of Main routine
#########################################################################
app = Application()
app.master.title("Print Per School")
app.mainloop()
sys.exit()

