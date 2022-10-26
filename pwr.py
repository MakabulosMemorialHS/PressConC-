#!/usr/bin/env python
#
# pwr.py
#
# Prepare winners report user interface.
# Unbelievable but true, this program is more complicated
# and longer than the program it calls which does the real job!
#
# Revision History:
# $Id: pwr.py,v 1.5 2014/09/08 11:33:23 robert Exp $
#
# $Log: pwr.py,v $
# Revision 1.5  2014/09/08 11:33:23  robert
# Changed commands for compiling the TeX file output.tex to
# use commands readily available with Ubuntu. eg evince, pdflatex, etc.
#
# Revision 1.4  2013/09/23 18:10:40  robert
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
# Revision 1.3  2011/09/22 09:37:01  robert
# Version of 21 Sept 2011. Will be used for the Press Competition of
# 23 Sept 2011.
#
# Revision 1.2  2008/09/26 11:59:27  robert
# Minor changes only. This version used during the 2008 APSTAP Press Conference.
#
# Revision 1.1  2008/09/24 14:54:57  robert
# Initial import into CVS. Works as expected.
#
#
import string
import tkFont
import sys
import os
from Tkinter import *

sqldb = os.popen("sqlite3 presscon.db \
     \"select contest_code, contest_name, language, level "
     "from contest order by contest_name, language, level;\"", "r")
contest_list = sqldb.readlines()
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
	print xsdf
	self.contest_code = xsdf[0]
	self.text0.delete(0,END)
	self.text0.insert(0,self.contest_code)

    def prep_rept(self):
        # Get the contest code.
	
	contest_code = string.strip(self.contest_code)
        os.system("rm output.tex")
	os.system("./print-winners %s" % (contest_code))
	os.system("pdflatex output && evince output.pdf")
	## os.system("latex output && dvips -t letter -o output.ps output\\
        ##     && evince output.ps")
        self.quit()	
        # Clear the entries
	self.text0.delete(0, END)
	
    def createWidgets(self):
        titlefont = tkFont.Font(family="new century schoolbook", size=14)
        textfont = tkFont.Font(family="new century schoolbook", size=12)

        ## The dialog box banner
	##
	self.fLabel = Label(self, \
	    text="User interface for Winners Report.\n",\
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
	      height=18,width=40,\
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

        self.OKButton = Button(self, text="OK", \
	     command=self.prep_rept,font=textfont)
	self.OKButton.grid(row=22,column=0)
	
        self.ExitButton = Button(self, text="Exit", font=textfont,\
	     command = self.quit)
	self.ExitButton.grid(row=22, column=1)
	


#########################################################################
# Start of Main routine
#########################################################################
app = Application()
app.master.title("Winners Report")
app.mainloop()
