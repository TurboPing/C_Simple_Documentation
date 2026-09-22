CC = gcc
OBJECTS = sockmain.o SvConn.o passiveTCP.o \
           passiveUDP.o passivesock.o AhList.o errexit.o

SocketTest: $(OBJECTS)
	$(CC) -g -o $(OBJECTS) -lsocket -lnsl -L/usr/lib
	         
