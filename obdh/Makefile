.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR  = $(shell pwd)
SRCDIR  = $(TOPDIR)/src
INCDIR  = $(TOPDIR)/include
DOCDIR  = $(TOPDIR)/documentation
TESTDIR = $(TOPDIR)/tests/unit

CC  = g++
CXX = g++
LIBS	 = -L../libs -lcwis-utils -lueye_api -lpthread
WARNINGS = -g -pedantic -Wextra -Wall -Wundef -Werror=implicit-function-declaration -Wmissing-include-dirs -Wshadow

APP = cwis_camera_test.out

CSRC = $(SRCDIR)/usbiss.c

SRC = $(SRCDIR)/camera.cpp				\
      $(SRCDIR)/ueye_event_thread.cpp	\
      $(SRCDIR)/image_buffer.cpp		\
      $(SRCDIR)/i2c_manager.cpp         \
      $(SRCDIR)/main.cpp
OBJ = $(CSRC:.c=.o) $(SRC:.cpp=.o)

# Build rules
all: $(APP)

doc: 
	doxygen Doxyfile


$(APP): $(OBJ)
	g++ -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	@$(CXX) $(ARCH) $(WARNINGS) -c -I$(INCDIR) -I../libs/ -o $@ $<

%.o: %.c
	@$(CC) $(ARCH) $(WARNINGS) -c -I$(INCDIR) -I../libs/ -o $@ $<

clean: 
	rm -rf $(OBJ)

docclean: 
	rm -rf $(DOCDIR)

distclean: clean
	rm -rf $(APP) $(DOCDIR)

