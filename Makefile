sip: sniffip.o mac.o hashlist.o hashalgo.o 
	gcc -o sip sniffip.o mac.o hashlist.o hashalgo.o -lpcap
	chmod +x libpy/data_entry.py
	chmod +x libpy/generatemap.py
sniffip.o: sniffip.c headers.h
	gcc -c sniffip.c headers.h
mac.o: mac.c mac.h
	gcc -c mac.c mac.h
hashlist.o: hashlist.c hashlist.h
	gcc -c hashlist.c hashlist.h
hashalgo.o: hashalgo.c hashalgo.h
	gcc -c hashalgo.c hashalgo.h

Install:
	cp sip /bin

Clean:
	rm -f *.gch *.o
	rm -f /bin/sip sip live.html
	rm -f data/*
	rm -f libpy/*.pyc 
