CPP=g++
CC=gcc
CFLAGS=-I/usr/local/include -I.
LDFLAGS=-Wl,-rpath,. -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
RESOURCE_FILES = $(wildcard resource_files/*)

dist: sniper
	./make_unix_dist.py sniper sniper_lin64
	rm -f sniper
	tar -czvf sniper_lin64.tgz sniper_lin64

sniper: resources.o res.o Target.o Map.o main.o
	$(CPP) $(CFLAGS) -o sniper main.o resources.o res.o Target.o Map.o $(LDFLAGS)

main.o: main.cpp
	$(CPP) $(CFLAGS) -c main.cpp

Map.o: Map.cpp Map.h 
	$(CPP) $(CFLAGS) -c Map.cpp

Target.o: Target.cpp Target.h 
	$(CPP) $(CFLAGS) -c Target.cpp

res.o: resources.o
	$(CPP) $(CFLAGS) -c res.c

resources.o: pack_resources.py $(RESOURCE_FILES)
	./pack_resources.py resource_files
	$(CPP) $(CFLAGS) -c resources.c

clean:
	rm -f *.o
	rm -f *~
	rm -f \#*
	rm -rf sniper_lin64

distclean: clean
	rm -f resources.h
	rm -f resources.c
	rm -f sniper
	
	rm -rf sniper_lin64.tgz
	