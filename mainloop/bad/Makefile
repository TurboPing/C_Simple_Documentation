PROG=mainloop_bad
all: $(PROG)

COPTS_CPU=-mcpu=ultrasparc -mvis
COPTS_DEBUG=-O0 -g
COPTS_STD=-O2
COPTS_OPT=-O3 -fforce-addr -frename-registers -ftracer -funit-at-a-time -funroll-loops -fpeel-loops
COPTS_WALL=-Wall -Werror
COPTS_PEDANTIC=-Wall -Werror -pedantic -std=c99 -Wformat -Wreturn-type -Wshadow -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes -Wpadded -Wredundant-decls

COPTS=$(COPTS_OPT) $(COPTS_CPU) $(COPTS_WALL)
#COPTS=$(COPTS_STD) $(COPTS_CPU) $(COPTS_WALL)
#COPTS=$(COPTS_DEBUG) $(COPTS_PEDANTIC)
#COPTS=$(COPTS_DEBUG) $(COPTS_WALL)

CC=gcc
#CC=quantify -record-child-process-data gcc
#CC=/sharedapps/Pure/releases/purify.sol.2003.06.00/purify gcc

LINK=$(CC)
LDFLAGS=
OBJECTS=$(PROG).o
LIBS=-lsocket -lnsl

$(PROG): $(OBJECTS)
	$(LINK) $(LDFLAGS) -o $(PROG) $(OBJECTS) $(LIBS)

$(PROG).o: $(PROG).c
	$(CC) $(COPTS) -c $(PROG).c

clean:
	rm -f $(OBJECTS) *_pure*.o $(PROG) *.BAK *.*~ .pure
