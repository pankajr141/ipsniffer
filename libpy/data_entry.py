#!/usr/bin/python
import os
import sys
import re
import json
import geoloc
import time
import traceback
from database import Database
from objects import ipobj
from generatemap import generatemap
#from time import gmtime, strftime

def echomsg(msg):
    print 'MSG# %s' % msg 

def data_entry():
    path = os.path.dirname(os.path.realpath(__file__))
    path = os.path.join(path , "../data")
    ifilepath = os.path.join(path, 'incoming')
    ignorefile = os.path.join(path, 'ignorelist')
    echomsg('Entering data')
    if not Database().checkDB():
       echomsg('Creating Db')
    iplist = open(ifilepath).readlines()

    lineno = 0
    if Database().getlastline():
       lineno = Database().getlastline()[0] + 1
    echomsg("lineno: %d" % lineno)
    generate_counter = 0   
 
    len_iplist =  len(iplist)
    hash_visited = {}
    for i in range(lineno, len_iplist):
       ip = iplist[i]
       ip = ip.rstrip()
       #pattern = re.compile(r"^10\.")
       pattern = re.compile(r"/(^127\.)|(^192\.168\.)|(^10\.)|(^172\.1[6-9]\.)|(^172\.2[0-9]\.)|(^172\.3[0-1]\.)|(^::1)$/")
       match = pattern.match(ip)
       if match:
          continue

       generate_counter += 1
       if generate_counter > 50:
          generate_counter=0
          generatemap()
        
       if not hash_visited.has_key(ip):
           ipinfo_json = geoloc.ip2location(ip)
           ipinfo 	= json.loads(ipinfo_json)
           loc	 	= ipinfo['loc']  if ipinfo.has_key('loc') else ' '
           city 	= ipinfo['city']  if ipinfo.has_key('city') else ' '
           ip		= ipinfo['ip']  if ipinfo.has_key('ip') else ' '
           region 	= ipinfo['region']  if ipinfo.has_key('region') else ' '
           hostname	= ipinfo['hostname']  if ipinfo.has_key('hostname') else ' '
           phone	= ipinfo['phone'] if ipinfo.has_key('phone') else ''
           country 	= ipinfo['country']  if ipinfo.has_key('country') else ' '
           org 		= ipinfo['org']  if ipinfo.has_key('org') else ' '
           postal 	= ipinfo['postal'] if ipinfo.has_key('postal') else ' '
           ip_v = ipobj(city, loc, ip, region, hostname, phone, country, org, postal,1)
           hash_visited[ip] = ip_v
       hash_visited[ip].extra += 1
       print i
    for key, ip_v in hash_visited.items():       
       if Database().ispresent(ip_v.ip):
           echomsg('count:%d\tUpdating %s' % (ip_v.extra,ip_v.ip))
           Database().updateDB(ip_v)
       else:
           #accesstime = strftime("%a, %d %b %Y %X +0000", gmtime())
           echomsg('count:%d\tInserting %s' % (ip_v.extra,ip_v.ip))
           Database().insertDB(ip_v)
    Database().updatelineno(len_iplist)
    generatemap()

def main():
    while True:
        try:
            data_entry()
        except IOError:
            pass
        except Exception, e:
            print e
            print traceback.format_exc()
              
if __name__ == '__main__':
    sys.exit(main())
