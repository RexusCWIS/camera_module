.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR  = $(shell pwd)
SRCDIR  = $(TOPDIR)
INCDIR  = $(TOPDIR)

CC  = gcc
CXX = g++
#LIBS	 = -L../libs -lcwis-pipes -lpthread
WARNINGS = -g -pedantic -Wextra -Wall -Wundef -Werror=implicit-function-declaration -Wmissing-include-dirs -Wshadow

APP = i2c-com.out

SRC = $(SRCDIR)/main.cpp    \
      $(SRCDIR)/usbiss.c    
OBJ = $(SRC:.cpp=.o)

# Build rules
all: $(APP)

$(APP): $(OBJ)
	g++ -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	@$(CXX) $(ARCH) $(WARNINGS) -c -I$(INCDIR) -I../libs/ -o $@ $<

%.o: %.c
	@$(CC) $(ARCH) $(WARNINGS) -c -I$(INCDIR) $I../libs/ -o $@ $<

clean: 
	rm -rf $(OBJ)

distclean: clean
	rm -rf $(APP) 

