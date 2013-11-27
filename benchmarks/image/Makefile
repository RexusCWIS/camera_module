.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR  = $(shell pwd)
SRCDIR  = $(TOPDIR)
INCDIR  = $(TOPDIR)
LIBDIR  = ../../libs

CXX = g++ -std=c++0x -O2 
LIBS	 = -L$(LIBDIR) -lcwis-utils -lcwis-camera  -lcwis-pipes -lueye_api -lpng -lpthread
WARNINGS = -g -pedantic -Wextra -Wall -Wundef -Werror=implicit-function-declaration -Wmissing-include-dirs -Wshadow

APP = image-benchmark.out

SRC = $(SRCDIR)/main.cpp			    
OBJ = $(SRC:.cpp=.o)

# Build rules
all: $(APP)

$(APP): $(OBJ)
	g++ -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	@$(CXX) $(ARCH) $(WARNINGS) -c -I$(INCDIR) -I$(LIBDIR) -o $@ $<

clean: 
	rm -rf $(OBJ)

distclean: clean
	rm -rf $(APP) 

