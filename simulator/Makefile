.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR = $(shell pwd) 
CAMDIR = ../camera

APP = cwis_sim.out

CXX = g++
INCFLAGS = -I$(CAMDIR)/include
LIBS = 

PIPESRC = $(CAMDIR)/src/pipes/tx_pipe.cpp     

SIMSRC  = main.cpp

OBJ = $(PIPESRC:.cpp=.o)    \
	  $(SIMSRC:.cpp=.o)

all: $(APP)

$(APP): $(OBJ)
	g++ -g -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	$(CXX) -c -g $(INCFLAGS) -o $@ $<

clean: 
	rm -rf $(OBJ)

distclean mrproper: clean
	rm -rf $(APP)

