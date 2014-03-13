.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR  = $(shell pwd)
DOCDIR  = $(TOPDIR)/documentation
TESTDIR = $(TOPDIR)/tests/unit

CXX = g++
WARNINGS = -g -pedantic -Wextra -Wall -Wundef -Werror=implicit-function-declaration -Wmissing-include-dirs -Wshadow

LIB = ../libcwis-utils.a

SRC = $(TOPDIR)/utilities.cpp 
OBJ = $(SRC:.cpp=.o)

# Build rules
all: $(LIB)

$(LIB): $(OBJ)
	ar -rv $(LIB) $(OBJ) 

%.o: %.cpp
	@$(CXX) $(WARNINGS) -c -o $@ $<

clean: 
	rm -rf $(OBJ)

distclean: clean
	rm -rf $(LIB) 

