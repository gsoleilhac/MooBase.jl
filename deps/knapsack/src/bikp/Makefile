CC := g++
CFLAGS := -Wall -lm -fPIC
INCLUDES :=  -I../../include

OBJ_DIR = ../../obj

sources = $(wildcard *.cpp)
objects = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(sources))


all: $(objects)
	clear

$(OBJ_DIR)/%.o : %.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ $(GLPK)
