#!/usr/bin/python 
import os
import shutil
import sys
import folium
import time
from objects import ipobj
from database import Database
import traceback

def getstring(string):
    try:
       return str(string)
    except Exception, e:
       return None

def echomsg(msg):
    print 'MSG# %s' % msg

def generatemap():
    path = os.path.dirname(os.path.realpath(__file__))
    path = os.path.join(path , "../")
    livepath = os.path.join(path, 'live.html')
    blivepath = os.path.join(path, 'live_b.html')

    echomsg('Generating map')    
    map_v = folium.Map(location=[28.5700, 77.3200], tiles='Mapbox Control Room', zoom_start=2,max_zoom=8, width="100%", height = "100%")
    ipobj_arr = Database().getobjects()
    for ip_v in ipobj_arr:
        description = "<table border='0'>\
                       <tr>\
                         <td><b>Count:</b></td><td>%s</td>\
                       </tr>\
                       <tr>\
                         <td><b>Ip</b></td><td> %s</td>\
                       </tr>\
                       <tr>\
                         <td><b>Hostname</b></td><td>%s:</td>\
                       </tr>\
                       <tr>\
                         <td><b>City</b></td><td>%s:</td>\
                       </tr>\
                       <tr>\
                         <td><b>Region</b></td><td>%s:</td>\
                       </tr>\
                       <tr>\
                         <td><b>Country</b></td><td>%s:</td>\
                       </tr>\
                       <tr>\
                         <td><b>Phone</b></td><td>%s</td>\
                       </tr>\
                       <tr>\
                         <td><b>Org</b></td><td>%s</td>\
                       </tr>\
                       <tr>\
                         <td><b>Postal</b></td><td>%s</td>\
                       </tr>\
                       </table>" % (getstring(ip_v.extra), getstring(ip_v.ip), getstring(ip_v.hostname), getstring(ip_v.city), 
                          getstring(ip_v.region), getstring(ip_v.country), getstring(ip_v.phone), getstring(ip_v.org), getstring(ip_v.postal))
        split = ip_v.loc.split(',')
        if len(split) < 2:
            continue
        #map_v.simple_marker([split[0], split[1]], popup=description)
        map_v.circle_marker([split[0], split[1]],radius=800, popup=description, line_color='#FF0000', fill_color='#FF0000', fill_opacity=0.2)
    map_v.create_map(path=livepath)
    lines = open(livepath).readlines()
    file = open(blivepath,'w')
    for line in lines:
        if 'Map tiles by' in line:
            continue
        #file.write(line.rstrip())
        file.write(line)
    file.close()
    shutil.copyfile(blivepath, livepath)
    os.remove(blivepath)

'''   
def main():
    while True: 
        try:
            time.sleep(4)
            generatemap()
            time.sleep(20)
        except Exception, e:
            print e
            print traceback.format_exc()
            pass
if __name__ == '__main__':
    sys.exit(main())
'''
