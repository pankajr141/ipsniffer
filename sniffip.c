#define _BSD_SOURCE 1
#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <linux/socket.h>
#include <unistd.h>
#include "headers.h"
#include "mac.h"
#include "hashlist.h"

mac_t hwaddr;  /*Contains hw address of local interface*/
hashlist hash_ignorelist;

void parse_ignorelist()
{
    static const char ignorefile_name[] = "ignorelist";
    FILE *file = fopen ( ignorefile_name, "r" );
    hashlist_ops.hashlist_initialize(&hash_ignorelist);
    size_t ln;
    if ( file != NULL )
    {
        char line [ 128 ]; /* or other suitable maximum line size */
        while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
        {
            ln = strlen(line) - 1;
            if (line[ln] == '\n')
                line[ln] = '\0';
            hashlist_ops.hashlist_add(&hash_ignorelist,line,0,0,0);
            //fputs ( line, stdout ); /* write the line */
        }
        fclose ( file );
    }
    else
    {
        perror ( ignorefile_name ); /* why didn't the file open? */
    }
    //hashlist_ops.hashlist_display(&hash_ignorelist);
}

void parse_packet(u_char *dumpfile, const struct pcap_pkthdr *header,const u_char *packet)
{
        /*
        struct tm *ltime;
        char timestr[16];
    
        //convert the timestamp to readable format 
        ltime=(struct tm *)localtime(&header->ts.tv_sec);
        strftime( timestr, sizeof(timestr), "%H:%M:%S", ltime);
        */

	const struct sniff_ethernet *ethernet; /* The ethernet header */
	const struct sniff_ip *ip; /* The IP header */
	const struct sniff_tcp *tcp; /* The TCP header */
	const char *payload; /* Packet payload */
	int i;
	u_int size_ip;
	u_int size_tcp;
        ethernet = (struct sniff_ethernet*)(packet);
	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
	size_ip = IP_HL(ip)*4;
	if (size_ip < 20) {
		//printf("   * Invalid IP header length: %u bytes\n", size_ip);
		return;
	}
	tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
	size_tcp = TH_OFF(tcp)*4;
	if (size_tcp < 20) {
		//printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
		return;
	}
       
        /*Printing EtherHeader*/
        if (ntohs (ethernet->ether_type) == ETHERTYPE_IP)
        {
            //fprintf(stdout, "%-20s: %s\n", "Ether type", "IP");
        }
        else  if (ntohs(ethernet->ether_type) == ETHERTYPE_ARP)
        {
          //  fprintf(stdout, "%-20s: %s\n", "Ether type", "ARP");
        }
        else  if (ntohs(ethernet->ether_type) == ETHERTYPE_REVARP)
        {
           // fprintf(stdout, "%-20s: %s\n", "Ether type", "RARP");
        }
        else {
          //  fprintf(stdout, "%-20s: %s\n", "Ether type", "?");
            return;
            //exit(1);
        }
        mac_t srcEther;
        mac_t dstEther;
        sprintf(srcEther, "%s", ether_ntoa((const struct ether_addr *)&ethernet->ether_shost));
        sprintf(dstEther, "%s", ether_ntoa((const struct ether_addr *)&ethernet->ether_dhost));
       // fprintf(stdout, "%-20s: %s\n", "SrcEther", ether_ntoa((const struct ether_addr *)&ethernet->ether_shost));
       // fprintf(stdout, "%-20s: %s\n", "DstEther", ether_ntoa((const struct ether_addr *)&ethernet->ether_dhost));
        
            
        /*Printing IP header*/
        char srcIp[INET_ADDRSTRLEN];    /*or malloc it later*/
        char dstIp[INET_ADDRSTRLEN];

        //srcIp = inet_ntoa(*(struct in_addr *)&ip->ip_src.s_addr);   /*To convert in_addr_t to in_addr.  Caused overlapping of srcIp and dstIp*/
        inet_ntop(AF_INET, &(ip->ip_src), srcIp,  INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ip->ip_dst), dstIp,  INET_ADDRSTRLEN);
        fprintf(stdout, "%s -> %s\n",srcIp, dstIp);
      
        hash* h1 = hashlist_ops.hashlist_findhash(&hash_ignorelist,srcIp);	
        if (h1 != NULL)
            return;
        
        FILE* fp;
        if ( strcmp(hwaddr, srcEther) == 0 )
        {
            fp = fopen("data/outgoing","a");
            fprintf(fp,"%s\n",dstIp);  
        }
        else 
        {
            fp = fopen("data/incoming","a");
            fprintf(fp,"%s\n",srcIp);  
        }
        fclose(fp);
}


int main(int argc, char *argv[])
{
        pcap_if_t *alldevs, *dev;
        pcap_if_t dev2;
	char errbuf[PCAP_ERRBUF_SIZE];
        int i=0;
        int totaldev = 0;

	struct bpf_program fp;		/* The compiled filter expression */
	char filter_exp[] = "";	/* The filter expression */
	bpf_u_int32 mask;		/* The netmask of our sniffing device */
	bpf_u_int32 net;		/* The IP of our sniffing device */

        int pid;
        /*
        pid = fork();
        if (pid < 0)
        {
             printf("fork failed!\n");
             exit(1);
        }
        else if (pid == 0)
             execl("node", "server.js", (char *) 0);
        */
        pid = fork();
        if (pid < 0)
        {
             printf("fork failed!\n");
             exit(1);
        }
        else if (pid == 0)
        {
             execl("libpy/data_entry.py", (char *) 0);
             exit(1);
        }

        pid = fork();
        if (pid < 0)
        {
             printf("fork failed!\n");
             exit(1);
        }
        else if (pid == 0)
        {
             system("node server.js");
             exit(1);
        } 
        parse_ignorelist();

        if (pcap_findalldevs(&alldevs, errbuf) == -1)
        {
            fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
            exit(1);
        }
        /*
	dev = pcap_lookupdev(errbuf);
	if (dev == NULL) {
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		return(2);
	}
	printf("Device: %s\n", dev);
        */
        for(dev=alldevs; dev; dev=dev->next)
        { 
            totaldev++;
            printf("%d. %s", ++i, dev->name);
            if (dev->description)
                printf(" (%s)\n", dev->description);
            else
                printf(" (No description available)\n");
        }
        printf("Total Devices %d\n", totaldev);
        int no;
        printf("Enter choice\t");
        scanf("%d",&no);
        if(no < 1 || no > totaldev)
        {
            printf("\nInterface number out of range.\n");
            /* Free the device list */
            pcap_freealldevs(alldevs);
            return -1;
        }
            /* Jump to the selected adapter */
        for(dev=alldevs, i=0; i<no-1 ;dev=dev->next, i++);

        printf("%s",dev->name);
        printf("%s",dev->addresses->addr); 
        pcap_t *handle;
        handle = pcap_open_live(dev->name, BUFSIZ, 1, 1000, errbuf);
        if (handle == NULL) {
            fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
            pcap_freealldevs(alldevs);
	    return(2);
	}
         // To get net and mask of the interface
        if (pcap_lookupnet(dev->name, &net, &mask, errbuf) == -1) {
	    fprintf(stderr, "Can't get netmask for device %s\n", dev->name);
	    net = 0;
	    mask = 0;
	}

	if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
	    fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
            pcap_freealldevs(alldevs);
	    return(2);
	}
	if (pcap_setfilter(handle, &fp) == -1) {
	    fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
            pcap_freealldevs(alldevs);
	    return(2);
	}
       
        findmac(dev->name, &hwaddr);

        /* we don't need any more the device list. Free it */
        pcap_freealldevs(alldevs);

        //pcap_loop(handle, 10, parse_packet, NULL);
        while (1==1)
            pcap_loop(handle, 1, parse_packet, NULL);


        /*
        ####To travese a packet using pcap_next
        
	struct pcap_pkthdr header;	// The header that pcap gives us 
	const u_char *packet;		// The actual packet 
	// Grab a packet 
	packet = pcap_next(handle, &header);
	// Print its length
	printf("Jacked a packet with length of [%d]\n", header.len);
	// And close the session 
	pcap_close(handle);
        */

	return(0);
}
