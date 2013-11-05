.PHONY: all, clean, mrproper, distclean

# Implicit ryles desactivation

TOPDIR = ../..

APP = unit.out

CXX = g++
INCFLAGS = -I$(TOPDIR)/include -Igtest-svn/include -I/opt/local/include
LIBS = /opt/local/lib/libpng.a gtest-svn/make/libgtest.a

SRC = $(TOPDIR)/src/image.cpp
TESTSRC = image_test.cpp

OBJ = $(SRC:.cpp=.o)    \
      $(TESTSRC:.cpp=.o)



all: $(APP)

$(APP): $(OBJ)
	g++ -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	$(CXX) -c $(INCFLAGS) -o $@ $<

