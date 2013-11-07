.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR = $(shell pwd)
SRCDIR = $(TOPDIR)/src
INCDIR = $(TOPDIR)/include
DOCDIR = $(TOPDIR)/documentation

CXX = g++
ARCH     = -m64 
LIBS	 = -lueye_api -lpng 
WARNINGS = -g -pedantic -Wextra -Wall -Wundef -Werror=implicit-function-declaration -Wmissing-include-dirs -Wshadow

APP = cwis_camera.out

SRC = $(SRCDIR)/main.cpp			\
	  $(SRCDIR)/ueye_camera.cpp 	\
	  $(SRCDIR)/image.cpp			\
	  $(SRCDIR)/utilities.cpp		
OBJ = $(SRC:.cpp=.o)

# Build rules
all: $(APP)

doc: $(DOCDIR)

$(DOCDIR):
	doxygen Doxyfile

$(APP): $(OBJ)
	g++ $(ARCH) -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	@$(CXX) $(ARCH) $(WARNINGS) -c -I$(INCDIR) -o $@ $<

clean: 
	rm -rf $(OBJ)

distclean: clean
	rm -rf $(APP)

