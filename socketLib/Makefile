# ------------------------------------------------------------------------------
# Makefile -- Makefile for a8620 socket library
# ------------------------------------------------------------------------------
#
# (c) 1995 Alcatel STR, Zuerich, Switzerland.
#
# ----------------------------------------------------------------------------
#
# $Id: Makefile,v 1.1 1995/10/24 09:47:46 roelli Exp $
#
# Status : Release 1.0
#
# ------------------------------------------------------------------------------
# Description
#
# ------------------------------------------------------------------------------
#
include ../../../MakeVars.Common
#
# ------------------------------------------------------ PROGRAMS & LIBRARIES --
#
# Programs & Libraries
#
                    PROGRAMS = 
                   LIBRARIES = libSocket.a libSocket.so
               TEST_PROGRAMS = SocketTest
#
# Socket
#
             SOCKET_INCLUDES = 
            SOCKET_LIBRARIES = $(NETWORK_LIB) -lLog -lAh
#
# libSocket
#
               SOCKET_A_OBJECTS = SocketLib.o
#
# SocketTest
#
        SOCKET_TEST_INCLUDES =
       SOCKET_TEST_LIBRARIES = $(NETWORK_LIB) -lSocket -lLog -lAh

# ------------------------------------------------------------------- Targets --
#
#
all: $(LIBRARIES) $(PROGRAMS) $(TEST_PROGRAMS)

#
# Socket
#
SocketLib.o: SocketLib.c
	$(CC) $(CFLAGS) $(SOCKET_INCLUDES) -c SocketLib.c

libSocket.a: $(SOCKET_A_OBJECTS)
	-$(RM) -f libSocket.a
	$(AR) $(ARFLAGS) libSocket.a $(SOCKET_A_OBJECTS)
	$(RANLIB) libSocket.a

libSocket.so: $(SOCKET_A_OBJECTS)
	$(LD) $(LDFLAGS) -o libSocket.so $(SOCKET_A_OBJECTS) $(SOCKET_LIBRARIES)

SocketTest.o: SocketTest.c
	$(CC) $(CFLAGS) $(SOCKET_TEST_INCLUDES) -c SocketTest.c

SocketTest: SocketTest.o libSocket.a
	$(LD) $(LDFLAGS) -o SocketTest SocketTest.o $(SOCKET_TEST_LIBRARIES)

# -----------------------------------------------------------------------------
# General Targets
#
clean:
	-$(RM) *.o $(LIBRARIES) $(PROGRAMS)
	-$(RM) *%

checkin:
	-$(RM) *%
	-$(CI) *.h *.c

checkout:
	-$(RM) *%
	-$(CO) *.h *.c

install: all
	-$(RM) -f $(SYSTEMDIR)/distrib/$(ARCH)/$(OS)/include/SocketLib.h
	-$(CP) SocketLib.h $(SYSTEMDIR)/distrib/$(ARCH)/$(OS)/include
	-$(CP) libSocket.a $(SYSTEMDIR)/distrib/$(ARCH)/$(OS)/lib
	-$(CP) libSocket.so $(SYSTEMDIR)/distrib/$(ARCH)/$(OS)/lib
	-$(RANLIB) $(SYSTEMDIR)/distrib/$(ARCH)/$(OS)/lib/libSocket.a

