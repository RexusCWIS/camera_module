.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR  = $(shell pwd)
SRCDIR  = $(TOPDIR)/src
INCDIR  = $(TOPDIR)/include
DOCDIR  = $(TOPDIR)/documentation
TESTDIR = $(TOPDIR)/tests/unit

CXX = g++
LIBS	 = -L../libs -lcwis-utils -lcwis-camera  -lcwis-pipes -lueye_api -lpng -lpthread
WARNINGS = -g -pedantic -Wextra -Wall -Wundef -Werror=implicit-function-declaration -Wmissing-include-dirs -Wshadow

APP = cwis_camera.out

SRC = $(SRCDIR)/main.cpp			    \
	  $(SRCDIR)/serial/i2c_bus.cpp		\
	  $(SRCDIR)/serial/rx_thread.cpp    
OBJ = $(SRC:.cpp=.o)

# Build rules
all: $(APP)

test: 
	cd $(TESTDIR); make run

doc: 
	doxygen Doxyfile


$(APP): $(OBJ)
	g++ -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	@$(CXX) $(ARCH) $(WARNINGS) -c -I$(INCDIR) -I../libs/ -o $@ $<

clean: 
	rm -rf $(OBJ)

docclean: 
	rm -rf $(DOCDIR)

distclean: clean
	rm -rf $(APP) $(DOCDIR)

