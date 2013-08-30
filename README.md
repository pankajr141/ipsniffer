IP SNIFFER
=============
Packages
--------
#### 1. sqlite

   ```shell
   yum install sqlite
   ```
#### 2. node.js

   ```shell
      yum install npm                --Fedora
   ```
   for other distro Please find following link
   https://github.com/joyent/node/wiki/Installing-Node.js-via-package-manager

#### 3. folium
   ```shell
    $ pip install folium 
   ```
#### 4. pcap library
   http://www.linuxfromscratch.org/blfs/view/svn/basicnet/libpcap.html
   
Installation
-------------
   ```shell
      make
   ```
Execution
---------
   ```shell
      sip
   ```
Enter the interface you want to sniff.
#### General interface are
* eth0
* em1

