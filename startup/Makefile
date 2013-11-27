.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR = $(shell pwd) 
CAMDIR = ../camera

APP = cwis_startup.out

CXX = g++
INCFLAGS = -I$(CAMDIR)/include
LIBS = 

SRC  = main.cpp

OBJ = $(SRC:.cpp=.o)

all: $(APP)

$(APP): $(OBJ)
	g++ -g -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	$(CXX) -c -g $(INCFLAGS) -o $@ $<

clean: 
	rm -rf $(OBJ)

distclean mrproper: clean
	rm -rf $(APP)

