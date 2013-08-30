import httplib2
import sys

def ip2location(ipaddress):
    host_address = 'http://ipinfo.io'
    h = httplib2.Http(".cache")
    resp, content = h.request("%s/%s/json/" %(host_address,ipaddress), "GET")
    return content
