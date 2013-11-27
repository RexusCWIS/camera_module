.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR   = ..
SRCDIR   = $(TOPDIR)/pipes/src
INCDIR   = $(TOPDIR)/pipes/include

CXX = g++
WARNINGS = -g -pedantic -Wextra -Wall -Wundef -Werror=implicit-function-declaration -Wmissing-include-dirs -Wshadow

LIB = $(TOPDIR)/libcwis-pipes.a

SRC = $(SRCDIR)/rx_pipe.cpp			\
	  $(SRCDIR)/tx_pipe.cpp
OBJ = $(SRC:.cpp=.o)

# Build rules
all: $(LIB)

$(LIB): $(OBJ)
	ar -rv $(LIB) $(OBJ) 

%.o: %.cpp
	@$(CXX) $(ARCH) $(WARNINGS) -c -I$(INCDIR) -I$(TOPDIR) -o $@ $<

clean: 
	rm -rf $(OBJ)

distclean: clean
	rm -rf $(LIB) 

