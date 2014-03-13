.PHONY: all, clean, mrproper, distclean

# Implicit rules desactivation
.SUFFIXES: 

TOPDIR  = $(shell pwd)

all:
	cd libs; make
	cd i2c_com; make
	cd camera; make

clean: 
	cd libs; make clean
	cd i2c_com; make clean
	cd camera; make clean

distclean: clean
	cd libs; make distclean
	cd i2c_com; make distclean
	cd camera; make distclean

