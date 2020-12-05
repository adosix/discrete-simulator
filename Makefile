example         = example
SRC_F           = usage_example/

CFLAGS          = -c -Wall -g
CC              = g++
INCLUDES        = #-I.
LIBDIRS         = lib/sim.o 
LDFLAGS         = $(LIBDIRS) $(LIBRARIES)



.cpp.o:
		$(CC) $(CFLAGS) -c $<

all:		$(example) 

rebuild:	clean all

$(example):	sim.o main.o 
		$(CC) $(SRC_F)main.o lib/sim.o -g -o $(example)

sim.o: lib/sim.cpp
		$(CC) -c -o $(LIBDIRS) lib/sim.cpp

main.o: $(SRC_F)main.cpp
		$(CC) $(CFLAGS) -o $(SRC_F)main.o $(SRC_F)main.cpp

clean:
	rm -fr *.o lib/*.o $(SRC_F)*.o


