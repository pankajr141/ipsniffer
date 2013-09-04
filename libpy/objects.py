
class ipobj:
    def __init__(self, city=None, loc=None, ip=None, region=None, hostname=None, phone=None, 
                 country=None, org=None, postal=None, extra=None):
        self.city = city
        self.loc = loc 
        self.ip =ip
        self.region = region
        self.hostname = hostname
        self.phone = phone
        self.country = country
        self.org = org
        self.postal = postal
        self.extra = extra
    def display(self):
        if not self.extra: 
            self.extra = 0 
        print "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%d" % (self.city,self.loc,self.ip,self.region,
               self.hostname,self.phone,self.country,self.org,self.postal,self.extra)
