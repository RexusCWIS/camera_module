.PHONY: all, clean, mrproper, distclean

# Implicit ryles desactivation

TOPDIR   = ../..
GTESTDIR = gtest-svn
LIBGTEST = $(GTESTDIR)/make/libgtest.a

APP = unit.out

CXX = g++
INCFLAGS = -I$(TOPDIR)/include -Igtest-svn/include -I/opt/local/include
LIBS = -lpng -lpthread $(LIBGTEST)

SRC = $(TOPDIR)/src/image.cpp		\
	  $(TOPDIR)/src/utilities.cpp
TESTSRC = image_test.cpp			\
		  utilities_test.cpp

OBJ = $(SRC:.cpp=.o)    \
      $(TESTSRC:.cpp=.o)

all: $(APP)


$(GTESTDIR):
	svn checkout http://googletest.googlecode.com/svn/trunk/ $(GTESTDIR)

$(LIBGTEST): $(GTESTDIR)
	cd $(GTESTDIR)/make; make
	ar -rv $(LIBGTEST) $(GTESTDIR)/make/gtest-all.o

$(APP): $(LIBGTEST) $(OBJ)
	g++ -o $@ $(OBJ) $(LIBS)

%.o: %.cpp
	$(CXX) -c $(INCFLAGS) -o $@ $<

clean:
	rm -rf $(OBJ)

distclean:
	rm -rf $(APP) $(GTESTDIR)

