.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR  = ../../..
TESTDIR = $(shell pwd) 

APP = pipes_test.out

CXX = g++
INCFLAGS = -I$(TOPDIR)/include
LIBS = -lpthread

SRC = $(TOPDIR)/src/pipes/tx_pipe.cpp     \
      $(TOPDIR)/src/pipes/rx_pipe.cpp

TESTSRC = main.cpp

OBJ = $(SRC:.cpp=.o)    \
      $(TESTSRC:.cpp=.o)

all: $(APP)

$(APP): $(OBJ)
	g++ -g -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	$(CXX) -c -g $(INCFLAGS) -o $@ $<

