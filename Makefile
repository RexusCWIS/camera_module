.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR = $(shell pwd)
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/include

CXX = g++
CXXFLAGS = -pedantic -Wextra -Wall -Wundef -Werror=implicit-function-declaration -Wmissing-include-dirs -Wshadow

APP = cwis_camera.out

SRC = $(SRCDIR)/main.cpp
OBJ = $(SRC:.cpp=.o)

# Build rules
all: $(APP)

$(APP): $(OBJ)
	@g++ -o $@ $(OBJ)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c -I$(INCDIR) -o $@ $<

clean: 
	rm -rf $(OBJ)

distclean: clean
	rm -rf $(APP)

