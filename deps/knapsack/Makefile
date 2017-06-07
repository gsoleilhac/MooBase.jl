
kp_sources = $(wildcard src/kp/*.cpp)
kp_objects = $(patsubst src/kp/%.cpp, obj/%.o, $(kp_sources))

bikp_sources = $(wildcard src/bikp/*.cpp)
bikp_objects = $(patsubst src/bikp/%.cpp, obj/%.o, $(bikp_sources))

moo_sources = $(wildcard src/moo/*.cpp)
moo_objects = $(patsubst src/moo/%.cpp, obj/%.o, $(moo_sources))

all: tree compile

tree:
	mkdir obj
	make -C src/bikp
	make -C src/kp
	make -C src/moo

ALL_OBJS := $(kp_objects) $(bikp_objects) $(moo_objects)

compile:
	gcc -shared -o ../libKP.so $(ALL_OBJS)

clear:
	@ clear
	
.PHONY: clean

clean: clear
	@ rm -f obj/*.o

