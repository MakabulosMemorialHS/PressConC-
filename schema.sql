-- Schema for Private Schools Press Conference Database
--
-- Revision History:
--
-- 23 September 2018:
-- >> Added the changes that were made to the database for
--    the 2016 Private Schools Press Conference.
--
--
-- $Id: schema.sql,v 1.10 2015/09/05 23:58:34 robert Exp $
--
-- $Log: schema.sql,v $
-- Revision 1.10  2015/09/05 23:58:34  robert
-- No comment.
--
-- Revision 1.9  2015/04/08 14:59:23  robert
-- Resolved conflict. Check the SQL database carefully before using
-- in production.
--
-- Revision 1.8  2014/09/08 11:49:02  robert
-- Added a new school and eight new contests for the school year
-- 2014--2015.
--
-- Also modified the names of the contests to use Filipino instead
-- of Pilipino.
--
-- Revision 1.7  2013/09/23 17:55:17  robert
-- Added new school.
--
-- Revision 1.6  2012/09/19 03:21:10  robert
-- Split the Radio Broadcasting and Scripting contests into their
-- separate language groups.
--
-- Revision 1.5  2012/09/13 04:32:50  robert
-- Saw a duplicate code. PUMCLC used by two schools. This
-- bug is now corrected.
--
-- Revision 1.4  2012/09/13 04:17:04  robert
-- Modified names of the events. Aligned it with the names
-- written in the APSTaP Manual.
--
-- Revision 1.3  2010/09/21 05:53:42  robert
-- Changed code of Rosebelle Academy.
--
-- Revision 1.2  2009/08/31 05:30:33  robert
-- Added new entries for San Lorenzo Ruiz Educational Foundation and
-- new event Radio Scriptwriting and Broadcasting.
--
-- Revision 1.1  2008/09/23 14:30:04  robert
-- Initial import into CVS.
-- I just dumped the contents of the database from the
-- 2006 Press Conference, modified the schema a little by
-- widening the widths of some fields, and removed entries
-- which will not be needed. ie names of winners from 2006.
--
--
PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;

CREATE TABLE school (
school_code   character(12),
school_name   character(128),
school_addr   character(128));

CREATE TABLE student (
pen_name    character(32),
last_name   character(64),
first_name  character(64),
school_code character(12));

CREATE TABLE contest (
contest_code    character(12),
contest_name    character(128),
language        character(64),
level           character(16));


-- Table of winners
--
CREATE TABLE wins (
contest_code    character(12),
pen_name        character(32),
place           decimal(3,1),
points          decimal(3,1));

-- The table sclink is supposed to indicate the list of students
-- which joined a contest. Never used it but left here in the schema
-- just in case.
--
CREATE TABLE sclink (
pen_name        character(32),
contest_code    character(12));


--- Some data tables need to be populated before hand.
---

--- Name of schools and their school codes.
---

INSERT INTO "school" VALUES('ACS','Anao Catholic School',NULL);
INSERT INTO "school" VALUES('AGCA','Amazing Grace Christian Academy',NULL);
INSERT INTO "school" VALUES('ALS','Asian Lexicon School',NULL);
INSERT INTO "school" VALUES('APCS','Asian Pacific Christian School',NULL);
INSERT INTO "school" VALUES('APCSI','Asian Pacific Christian School',NULL);
INSERT INTO "school" VALUES('BCA','Bamban Christian Academy',NULL);
INSERT INTO "school" VALUES('BCCI','Bestcap Career College',NULL);
INSERT INTO "school" VALUES('BCS','Bethany Christian School',NULL);
INSERT INTO "school" VALUES('BFBCS','BFBC Christian School',NULL);
INSERT INTO "school" VALUES('BFLC','Bible Fellowship Learning Center',NULL);
INSERT INTO "school" VALUES('BLSB','Bright Learners School of Bamban','Bamban');
INSERT INTO "school" VALUES('BZA','Bountiful Zion Academy',NULL);
INSERT INTO "school" VALUES('CAEA','Camiling Adventist Elementary School',NULL);
INSERT INTO "school" VALUES('CAMCS','Camiling Catholic School',NULL);
INSERT INTO "school" VALUES('CBBCS','Concepcion Bible Baptist Church School',NULL);
INSERT INTO "school" VALUES('CC','Camiling Colleges',NULL);
INSERT INTO "school" VALUES('CCHS','Capas Christian High School',NULL);
INSERT INTO "school" VALUES('CES','Concepcion Ecumenical School',NULL);
INSERT INTO "school" VALUES('CGSS','Capas Good Samaritan School, Inc','Capas');
INSERT INTO "school" VALUES('CHCC','Concepcion Holy Cross College',NULL);
INSERT INTO "school" VALUES('CICSCS','CICS Christian School',NULL);
INSERT INTO "school" VALUES('CITC','CIT Colleges',NULL);
INSERT INTO "school" VALUES('CLHS','Central Luzon High School',NULL);
INSERT INTO "school" VALUES('CMCS','Concepcion Modern Child School',NULL);
INSERT INTO "school" VALUES('CMS','Carino Multigrade School',NULL);
INSERT INTO "school" VALUES('CONCS','Concepcion Catholic School',NULL);
INSERT INTO "school" VALUES('CSI','Camiling Secondary Institute',NULL);
INSERT INTO "school" VALUES('CUMC','Camiling United Methodist Church',NULL);
INSERT INTO "school" VALUES('DFI','Dallidagum Foundation, Inc.','');
INSERT INTO "school" VALUES('DCT','Dominican College of Tarlac',NULL);
INSERT INTO "school" VALUES('GCS','Gerona Catholic School',NULL);
INSERT INTO "school" VALUES('GDMS','Glory Dei Montessori School',NULL);
INSERT INTO "school" VALUES('GEI','Great Eastern Institute',NULL);
INSERT INTO "school" VALUES('GECSI','Gerona Ecumenical Christian School',NULL);
INSERT INTO "school" VALUES('GGCA','Gods Grace Christian Academy',NULL);
INSERT INTO "school" VALUES('GI','Gerona Institute',NULL);
INSERT INTO "school" VALUES('GJC','Gerona Junior College',NULL);
INSERT INTO "school" VALUES('GSCCA','Good Shepherd Capas Christian Academy',NULL);
INSERT INTO "school" VALUES('GSCSC','Good Shepherd Christian School Concepcion',NULL);
INSERT INTO "school" VALUES('GSHS','Gerona Sacred Heart School',NULL);
INSERT INTO "school" VALUES('GSN','Great Shepherd Nazarene',NULL);
INSERT INTO "school" VALUES('GSSS','Good Shepherd Shekinah School',NULL);
INSERT INTO "school" VALUES('GUMCLC','Gerona United Methodist Church Learning Center',NULL);
INSERT INTO "school" VALUES('GVECS','Guevara Ecumenical Christian School',NULL);
INSERT INTO "school" VALUES('HSA','Headway School Of Achievers',NULL);
INSERT INTO "school" VALUES('HCSG','Holy Child School of Gerona',NULL);
INSERT INTO "school" VALUES('HTCT','Holy Trinity College of Technology',NULL);
INSERT INTO "school" VALUES('IAI','Interworld Asian Institute',NULL);
INSERT INTO "school" VALUES('ICFI','Interworld College Foundation Inc.',NULL);
INSERT INTO "school" VALUES('ICLC','Immaculate Concepcion Learning Center',NULL);
INSERT INTO "school" VALUES('JLC','Jabez Learning Center',NULL);
INSERT INTO "school" VALUES('JMELC','Jose Maria Escriva Learning Center',NULL);
INSERT INTO "school" VALUES('LCA','Lapaz Christian Academy',NULL);
INSERT INTO "school" VALUES('LECS','Lapaz Ecumenical Christian School',NULL);
INSERT INTO "school" VALUES('LGCS','Lightgiver Christian School',NULL);
INSERT INTO "school" VALUES('LLDS','Lords Love Development School',NULL);
INSERT INTO "school" VALUES('LSCA','Living Stones Christian Academy',NULL);
INSERT INTO "school" VALUES('MA','Mayantoc Academy',NULL);
INSERT INTO "school" VALUES('MAMS','Moncada Adventist Multigrade School',NULL);
INSERT INTO "school" VALUES('MAS','Mayantoc Angelicum School',NULL);
INSERT INTO "school" VALUES('MCS','Moncada Catholic School',NULL);
INSERT INTO "school" VALUES('MHS','Mayantoc High School',NULL);
INSERT INTO "school" VALUES('MMHS','Makabulos Memorial High School',NULL);
INSERT INTO "school" VALUES('MRS','Mother Regina School',NULL);
INSERT INTO "school" VALUES('MSGI','Marian School of Gerona',NULL);
INSERT INTO "school" VALUES('MSI','Montessan School',NULL);
INSERT INTO "school" VALUES('MSSN','Montessori School of St Nicholas',NULL);
INSERT INTO "school" VALUES('MUMLC','Mababanaba United Methodist Learning Center',NULL);
INSERT INTO "school" VALUES('NLLSSR','New Life Learning School of San Roque',NULL);
INSERT INTO "school" VALUES('NSUMCLC','New Salem UMC LC',NULL);
INSERT INTO "school" VALUES('OLRA','OLRA College Foundation',NULL);
INSERT INTO "school" VALUES('PA','Pura Academy',NULL);
INSERT INTO "school" VALUES('PAM','Plebian Academy Moncada',NULL);
INSERT INTO "school" VALUES('PAP','Plebeian Academy Paniqui',NULL);
INSERT INTO "school" VALUES('PHAC','Precious High Academy Concepction',NULL);
INSERT INTO "school" VALUES('PRCS','Progress Christian School',NULL);
INSERT INTO "school" VALUES('PCSI','Paniqui Christian School',NULL);
INSERT INTO "school" VALUES('PLC','Padapada Learning Center',NULL);
INSERT INTO "school" VALUES('PODMSI','PO Domingo Montessori School',NULL);
INSERT INTO "school" VALUES('PPES','Pepe and Pilar Ecumenical School',NULL);
INSERT INTO "school" VALUES('PDUMCLC','Padapada UMCLC',NULL);
INSERT INTO "school" VALUES('PRUMCLC','Pura UMCLC',NULL);
INSERT INTO "school" VALUES('QAMS','Quinmardero Adventist Multigrade School',NULL);
INSERT INTO "school" VALUES('RA','Ridgemore Academy',NULL);
INSERT INTO "school" VALUES('RBAT','Rosebelle Academy of Tarlac',NULL);
INSERT INTO "school" VALUES('RFCH','Rose Field Childrens House',NULL);
INSERT INTO "school" VALUES('RFMS','Rodrigo Fernando Memorial School',NULL);
INSERT INTO "school" VALUES('RSDS','Remnest Child Development School',NULL);
INSERT INTO "school" VALUES('RSES','Rose of Sharon Ecumenical School',NULL);
INSERT INTO "school" VALUES('RSLC','Rising Sun Learning Center',NULL);
INSERT INTO "school" VALUES('SACS','Saint Antoninus Christian School','');
INSERT INTO "school" VALUES('SAS','Sulipa Adventist School',NULL);
INSERT INTO "school" VALUES('SCHS','San Clemente High School',NULL);
INSERT INTO "school" VALUES('SDALC','Seventh Day Adventist Learning Center',NULL);
INSERT INTO "school" VALUES('SLC','Shepherds Lamb Credence','');
INSERT INTO "school" VALUES('SIENA','Siena Montessori School of Tarlac','NA');
INSERT INTO "school" VALUES('SIA','Sta Ignacia Academy',NULL);
INSERT INTO "school" VALUES('SIBCCA','SIBC Christian Academy',NULL);
INSERT INTO "school" VALUES('SICST','Sta Ignacia Catholic School of Tarlac','Sta Ignacia');
INSERT INTO "school" VALUES('SIHS','Sta Ignacia High School',NULL);
INSERT INTO "school" VALUES('SJAF','San Jose Academy Foundation',NULL);
INSERT INTO "school" VALUES('SLEF','San Lorenzo Ruiz Education Foundation',NULL);
INSERT INTO "school" VALUES('SNA','Santo Nino Academy',NULL);
INSERT INTO "school" VALUES('SPCT','St Paul College of Technology',NULL);
INSERT INTO "school" VALUES('SRCS','St Rose Catholic School',NULL);
INSERT INTO "school" VALUES('SRLC','St Raymund Learning Center',NULL);
INSERT INTO "school" VALUES('SVSF','St Vincent School Foundation',NULL);
INSERT INTO "school" VALUES('UMCLC','The United Methodist Church Learning Center',NULL);
INSERT INTO "school" VALUES('UMCLCSI','The United Methodist Church Learning Center of Sta Ignacia',NULL);
INSERT INTO "school" VALUES('UMS','Uzziel Montessori School',NULL);
INSERT INTO "school" VALUES('VCS','Victoria Catholic School',NULL);
INSERT INTO "school" VALUES('VUMCLC','Victoria UMCLC',NULL);
INSERT INTO "school" VALUES('WHS','Wellspring High School',NULL);
INSERT INTO "school" VALUES('WTAMS','Western Tarlac Adventist Multigrade School',NULL);
INSERT INTO "school" VALUES('WCS','Wesleyan Christian School',NULL);
INSERT INTO "school" VALUES('SMST','Siena Montessori School Of Tarlac','NA');
INSERT INTO "school" VALUES('CLAS','Concepcion Little Angels School','Concepcion');
INSERT INTO "school" VALUES('CSLR','Colegio De San Lorenzo Ruiz','NA');
INSERT INTO "school" VALUES('VANAV','School Not Available','NA');


--- Table of the Contests which are in competition
---

INSERT INTO "contest" VALUES('CLEE','Column Writing','English','Elementary');
INSERT INTO "contest" VALUES('CLES','Column Writing','English','Secondary');
INSERT INTO "contest" VALUES('CLPE','Column Writing','Filipino','Elementary');
INSERT INTO "contest" VALUES('CLPS','Column Writing','Filipino','Secondary');
INSERT INTO "contest" VALUES('CREE','Copyreading and Headline Writing','English','Elementary');
INSERT INTO "contest" VALUES('CRES','Copyreading and Headline Writing','English','Secondary');
INSERT INTO "contest" VALUES('CRPE','Copyreading and Headline Writing','Filipino','Elementary');
INSERT INTO "contest" VALUES('CRPS','Copyreading and Headline Writing','Filipino','Secondary');
INSERT INTO "contest" VALUES('DPPE','Desktop Publishing','Pilipino','Elementary');
INSERT INTO "contest" VALUES('DPPS','Desktop Publishing','Pilipino','Secondary');
INSERT INTO "contest" VALUES('DPES','Desktop Publishing','English','Secondary');
INSERT INTO "contest" VALUES('DPEE','Desktop Publishing','English','Elementary');
INSERT INTO "contest" VALUES('ECEE','Editorial Cartooning','English','Elementary');
INSERT INTO "contest" VALUES('ECES','Editorial Cartooning','English','Secondary');
INSERT INTO "contest" VALUES('ECPE','Editorial Cartooning','Filipino','Elementary');
INSERT INTO "contest" VALUES('ECPS','Editorial Cartooning','Filipino','Secondary');
INSERT INTO "contest" VALUES('EWEE','Editorial Writing','English','Elementary');
INSERT INTO "contest" VALUES('EWES','Editorial Writing','English','Secondary');
INSERT INTO "contest" VALUES('EWPE','Editorial Writing','Filipino','Elementary');
INSERT INTO "contest" VALUES('EWPS','Editorial Writing','Filipino','Secondary');
INSERT INTO "contest" VALUES('FWEE','Feature Writing','English','Elementary');
INSERT INTO "contest" VALUES('FWES','Feature Writing','English','Secondary');
INSERT INTO "contest" VALUES('FWPE','Feature Writing','Filipino','Elementary');
INSERT INTO "contest" VALUES('FWPS','Feature Writing','Filipino','Secondary');
INSERT INTO "contest" VALUES('NWEE','News Writing','English','Elementary');
INSERT INTO "contest" VALUES('NWES','News Writing','English','Secondary');
INSERT INTO "contest" VALUES('NWPE','News Writing','Filipino','Elementary');
INSERT INTO "contest" VALUES('NWPS','News Writing','Filipino','Secondary');
INSERT INTO "contest" VALUES('PJEE','Photo Journalism','English','Elementary');
INSERT INTO "contest" VALUES('PJES','Photo Journalism','English','Secondary');
INSERT INTO "contest" VALUES('PJPE','Photo Journalism','Filipino','Elementary');
INSERT INTO "contest" VALUES('PJPS','Photo Journalism','Filipino','Secondary');
INSERT INTO "contest" VALUES('RBPE','Radio Scripting and Broadcasting','Filipino','Elementary');
INSERT INTO "contest" VALUES('RBEE','Radio Scripting and Broadcasting','English','Elementary');
INSERT INTO "contest" VALUES('RBES','Radio Scripting and Broadcasting','English','Secondary');
INSERT INTO "contest" VALUES('RBPS','Radio Scripting and Broadcasting','Filipino','Secondary');
INSERT INTO "contest" VALUES('SCEE','Sports Writing','English','Elementary');
INSERT INTO "contest" VALUES('SCES','Sports Writing','English','Secondary');
INSERT INTO "contest" VALUES('SCPE','Sports Writing','Filipino','Elementary');
INSERT INTO "contest" VALUES('SCPS','Sports Writing','Filipino','Secondary');
INSERT INTO "contest" VALUES('SWFE','Science and Technology Writing','Filipino','Elementary');
INSERT INTO "contest" VALUES('SWEE','Science and Technology Writing','English','Elementary');
INSERT INTO "contest" VALUES('SWES','Science and Technology Writing','English','Secondary');
INSERT INTO "contest" VALUES('SWFS','Science and Technology Writing','Filipino','Secondary');
INSERT INTO "contest" VALUES('TVPS','TV News Production','Pilipino','Secondary');
INSERT INTO "contest" VALUES('TVES','TV News Production','English','Secondary');

COMMIT;
