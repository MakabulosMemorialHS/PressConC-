#!/usr/bin/env python
#
# menu.py
#
# Top Level Menu. Run this program to gain access to all utilities
# in this package.
#
# Revision History:
#
# $Id: menu.py,v 1.10 2014/09/08 13:15:19 robert Exp $
#
# $Log: menu.py,v $
# Revision 1.10  2014/09/08 13:15:19  robert
# Removed routine that will print to the Samsung ML-1610 since I
# no longer have that printer. Instead of that, this version
# uses evince to view and print the output file.
#
# Revision 1.9  2013/09/23 17:59:12  robert
# Changed commands for printing the reports.
#
# Revision 1.8  2012/09/13 04:15:36  robert
# Readied for PressCon of 14 Sept 2012.
#
# Revision 1.7  2011/09/22 09:37:01  robert
# Version of 21 Sept 2011. Will be used for the Press Competition of
# 23 Sept 2011.
#
# Revision 1.6  2009/08/31 05:33:28  robert
# Modified call to command dvips to reflect behavior of newer version.
#
# Revision 1.5  2008/09/27 14:11:37  robert
# With the printing problem with the hpijs and hp3325 printer
# resolved, this version now sports a "Print Last Viewed Report"
# button; replacing the "Create PostScript File" button.
#
# Revision 1.4  2008/09/26 11:55:03  robert
# This is the version used during the 2008 APSTAP Press Conference.
#
# Revision 1.3  2008/09/24 14:52:49  robert
# Added the actions associated with the various buttons.
#
# Revision 1.2  2008/09/24 09:38:19  robert
# Changed call to programs to include relative pathnames.
#
# Revision 1.1  2008/09/23 16:03:28  robert
# This is the old menu renamed to menu.py
#
# Revision 1.2  2008/09/22 12:24:26  robert
# Added a new entry.
#
# Revision 1.1  2008/09/22 06:53:22  robert
# Initial import into CVS.
#
#
import string
import sys,os
import tkFont
from Tkinter import *

def call_enter_students():
    os.system("./enter-students.py")
    
def call_enter_winners():
    os.system("./enter-winners.py")

def call_pwr():
    os.system("./pwr.py")

def call_psr():
    os.system("./psr.py")

def prep_rankings():
    os.system("./print-rankings")
    os.system("pdflatex output && evince output.pdf")

def call_commit():
    os.system("sqlite3 presscon.db \".read SQL.out\" && rm SQL.out")
    print "SQL.out commited to database."

def create_postscript():
    os.system("dvips -t letter -o output.ps output"
	    + "&& evince output.ps")
    
class Application(Frame):
    def __init__(self, master=None):
	Frame.__init__(self, master)
	self.grid()
	self.createWidgets()

    def createWidgets(self):
        titlefont = tkFont.Font(family="new century schoolbook", size=14)
        textfont = tkFont.Font(family="new century schoolbook", size=12)
	
	self.fLabel = Label(self, \
	    text="APSTAP Press Conference\n"
	         "Main Menu\n", font=titlefont)
	self.fLabel.grid()

        self.penNamesButton = Button(self, text="Enter Pen Names",\
	          font=textfont,\
		  command=call_enter_students)
	self.penNamesButton.grid()
        self.winnersButton = Button(self, text="Enter Winners",\
	          command=call_enter_winners,
		  font=textfont)
	self.winnersButton.grid()
        self.separatorLabel0 = Label(self, text="-----------------------------")
	self.separatorLabel0.grid()

        self.commitButton = Button(self,\
	     text="Commit",\
	     command=call_commit,\
	     font=textfont)
	self.commitButton.grid()
	
        self.separatorLabel1 = Label(self, text="-----------------------------")
	self.separatorLabel1.grid()
	
        self.winPrintButton = Button(self, \
	     text="Prepare Winners Report",\
             command=call_pwr,\
             font=textfont)
	self.winPrintButton.grid()
        self.schoolprintButton = Button(self,\
	     text="Prepare Schools Report",\
	     command=call_psr,\
	     font=textfont)
	self.schoolprintButton.grid()
        self.rankprintButton = Button(self, \
	     text="Prepare Rankings\nFor Printing",\
	     command=prep_rankings,\
	     font=textfont)
	self.rankprintButton.grid()
	
        self.psprintButton = Button(self, \
	     text="Print Last\nViewed Report",\
	     command=create_postscript,\
	     font=textfont)
	self.psprintButton.grid()
	
        self.separatorLabel2 = Label(self, text="-----------------------------")
	self.separatorLabel2.grid()

	self.quitButton = Button(self, text="Exit", command=self.quit,font=textfont)
	self.quitButton.grid()
	

app = Application()
app.master.title("APSTAP Press Conference")
app.mainloop()
sys.exit()


