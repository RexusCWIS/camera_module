.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR = $(shell pwd) 
LIBDIR = ../libs

APP = cwis_sim.out

CXX = g++
INCFLAGS = -I$(LIBDIR)
LIBS = -L$(LIBDIR) -lcwis-pipes 

SIMSRC  = main.cpp

OBJ = $(SIMSRC:.cpp=.o)

all: $(APP)

$(APP): $(OBJ)
	g++ -g -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	$(CXX) -c -g $(INCFLAGS) -o $@ $<

clean: 
	rm -rf $(OBJ)

distclean mrproper: clean
	rm -rf $(APP)

