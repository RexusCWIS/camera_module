.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR  = $(shell pwd)
SRCDIR  = $(TOPDIR)
INCDIR  = $(TOPDIR)

CC  = g++
CXX = g++
LIBS	 = -L../libs -lcwis-pipes -lpthread
WARNINGS = -g -pedantic -Wextra -Wall -Wundef -Werror=implicit-function-declaration -Wmissing-include-dirs -Wshadow

APP = i2c-com.out

CSRC   = $(SRCDIR)/usbiss.c    
CPPSRC = $(SRCDIR)/main.cpp    

OBJ = $(CSRC:.c=.o) $(CPPSRC:.cpp=.o)

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

