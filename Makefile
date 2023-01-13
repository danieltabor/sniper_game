RESOURCE_FILES = notebook.ttf bad.ogg bgm.ogg good.ogg shot.ogg howto.jpg title.jpg title.ogg dead.png map_bg.png map_fg.png map_mask.png notebook.png scope.png scribble.png target_0.png target_1.png target_2.png target_3.png target_4.png target_5.png  target_6.png target_7.png target_8.png target_9.png  target_10.png target_11.png target_12.png target_13.png target_14.png target_15.png target_16.png target_17.png target_18.png target_19.png

CPP=g++
CC=gcc
CFLAGS=-I/usr/local/include -I.
LDFLAGS=-Wl,-rpath,. -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

dist: sniper
	python make_unix_dist.py sniper sniper_lin64
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

resources.o: $(RESOURCE_FILES)
	python pack_resources.py $(RESOURCE_FILES)
	$(CPP) $(CFLAGS) -c resources.c

clean:
	rm -f *.o
	rm -f *~
	rm -f \#*

distclean: clean
	rm -f sniper
	rm -rf sniper_lin64