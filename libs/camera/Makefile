.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR   = ..
SRCDIR   = $(TOPDIR)/camera
INCDIR   = $(TOPDIR)/camera

CXX = g++ -O2
WARNINGS = -g -pedantic -Wextra -Wall -Wundef -Werror=implicit-function-declaration -Wmissing-include-dirs -Wshadow

LIB = $(TOPDIR)/libcwis-camera.a

SRC = $(SRCDIR)/ueye_camera.cpp 	    \
	  $(SRCDIR)/ueye_event_thread.cpp 	\
	  $(SRCDIR)/image.cpp			    \
	  $(SRCDIR)/ring_buffer.cpp		    
OBJ = $(SRC:.cpp=.o)

# Build rules
all: $(LIB)

test: 
	cd $(TESTDIR); make run

doc: 
	doxygen Doxyfile


$(LIB): $(OBJ)
	ar -rv $(LIB) $(OBJ) 

%.o: %.cpp
	$(CXX) $(ARCH) $(WARNINGS) -c -I$(INCDIR) -I$(TOPDIR) -o $@ $< 

clean: 
	rm -rf $(OBJ)

distclean: clean
	rm -rf $(LIB) 

