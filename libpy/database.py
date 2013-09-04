from sqlite3 import OperationalError
import sqlite3 as lite
import sys
import os
#import conf
from objects import ipobj


path = os.path.dirname(os.path.realpath(__file__))
path = os.path.join(path , "../data")
datafilepath = os.path.join(path, 'locationinfo.db')

class Database:
  #-------------------------------------------------------------------------
  # Function to check if the modified version of file is present in database
  # return false if
  # 1. file not present
  # 2. currentmodification time is not same as in database
  #-------------------------------------------------------------------------
  def ispresent(self, ip):
    con = lite.connect(datafilepath)
    with con:
      cur = con.cursor()
      cur.execute("select count from packetinfo where ip='%s'" % ip)
      count = cur.fetchone()
      if not count:
         return False
      return True      

  def updateDB(self, ip_v):
    con = lite.connect(datafilepath)
    with con:
      cur = con.cursor()
      cur.execute("update packetinfo set count=count+%d where ip='%s'" % (ip_v.extra, ip_v.ip))
      con.commit()

  def insertDB(self, ip_v): #, lastaccesstime):
    con = lite.connect(datafilepath)
    with con:
      cur = con.cursor()
      #ip_v.display()
      con.execute("insert into packetinfo values (\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%d\")"%(ip_v.city, 
                      ip_v.loc, ip_v.ip, ip_v.region, ip_v.hostname, ip_v.phone, ip_v.country, ip_v.org, ip_v.postal, ip_v.extra))
      con.commit()

  def checkDB(self):
    con = lite.connect(datafilepath)
    with con:
      cur1 = con.cursor()
      cur2 = con.cursor()

      try:
         cur1.execute("select * from packetinfo")
      except OperationalError:
         cur1.execute("create table packetinfo(city TEXT, loc TEXT, ip TEXT, region TEXT, hostname TEXT, phone TEXT,\
                      country TEXT, org TEXT, postal TEXT, count INTEGER)")
         con.commit()
         cur1.execute("select * from packetinfo")

      try:
         cur2.execute("select * from info")
      except OperationalError:
         cur2.execute("create table info(lineno INTEGER)")
         con.commit()
         cur2.execute("select * from packetinfo")

      exist1 = cur1.fetchone()
      exist2 = cur2.fetchone()
      if not exist1 or not exist2:
          return False
      return True

  def getobjects(self):
    con = lite.connect(datafilepath)
    with con:
      cur = con.cursor()
      cur.execute("select * from packetinfo")
      rows = cur.fetchall()
      ipobj_arr = []
      for row in rows:
          ipobj_arr.append(ipobj(row[0],row[1],row[2],row[3],row[4],row[5],row[6],row[7],row[8],row[9]))
      return ipobj_arr 
 
  def getlastline(self): 
      con = lite.connect(datafilepath)
      with con:
         cur = con.cursor()
         cur.execute("select lineno from info")
         line = cur.fetchone()
         return line

  def updatelineno(self, lineno):
    con = lite.connect(datafilepath)
    with con:
      cur = con.cursor()
      if not self.getlastline():
          cur.execute("insert into info values(%d)" % (int(lineno)))
      else:
          cur.execute("update info set lineno=%d" % (int(lineno)))
      con.commit()

