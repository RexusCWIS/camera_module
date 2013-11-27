.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR   = $(shell pwd)
CAMDIR   = $(TOPDIR)/camera
UTILSDIR = $(TOPDIR)/utils
PIPESDIR = $(TOPDIR)/pipes

TESTDIR  = $(TOPDIR)/tests/unit


# Build rules
all: libs

libs: 
	cd $(CAMDIR); make
	cd $(UTILSDIR); make
	cd $(PIPESDIR); make

test: libs
	cd $(TESTDIR); make run

doc: 
	doxygen Doxyfile

clean: 
	cd $(CAMDIR); make clean
	cd $(UTILSDIR); make clean
	cd $(PIPESDIR); make clean

distclean:
	cd $(CAMDIR); make distclean
	cd $(UTILSDIR); make distclean
	cd $(PIPESDIR); make distclean

