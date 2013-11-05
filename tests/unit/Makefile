.PHONY: all, clean, mrproper, distclean

# Implicit ryles desactivation

TOPDIR = ../..

APP = unit.out

CXX = g++
INCFLAGS = -I$(TOPDIR)/include -Igtest-svn/include -I/opt/local/include
LIBS = -lpng -lpthread gtest-svn/make/libgtest.a

SRC = $(TOPDIR)/src/image.cpp
TESTSRC = image_test.cpp

OBJ = $(SRC:.cpp=.o)    \
      $(TESTSRC:.cpp=.o)

all: $(APP)


download: 
	svn checkout http://googletest.googlecode.com/svn/trunk/ gtest-svn

$(APP): download $(OBJ)
	g++ -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	$(CXX) -c $(INCFLAGS) -o $@ $<

