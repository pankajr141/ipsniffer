#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include "mac.h"

void findmac(char* devicename, mac_t* hwaddr)
{
  char* ptr = * hwaddr;
  struct ifreq s;
  int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

  strcpy(s.ifr_name, devicename);
  if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) {
    int i;
    for (i = 0; i < 6; ++i)
    {
      if (i == 5 )
          ptr += sprintf(ptr,"%02x", (unsigned char) s.ifr_addr.sa_data[i]);
      else
          ptr += sprintf(ptr,"%02x:", (unsigned char) s.ifr_addr.sa_data[i]);
    }
  }
}

/*
int main()
{
    mac_t hwaddr;
    findmac("em1", &hwaddr);
    printf("%s\n", hwaddr);
}
*/
