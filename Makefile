#---------GENERAL SETTINGS--------------
PROGRAMS  = $(subst .cxx,.exe,$(shell ls test*.cxx)) 
FILESGENERATEDBYSCRIPTS = DataItemFactory.h DataItemFactory.cxx
MYLIBCXX  = $(subst .h,.cxx,$(shell ls *.h)) 
MYLIBS    = $(subst .cxx,.o,$(shell ls $(MYLIBCXX) 2>/dev/null))
ROOTSYS   = $(shell echo $ROOTSYS)
#include $(ROOTSYS)/etc/Makefile.arch
# -fPIC option needed only for compilation into a shared library
CXXFLAGS = $(shell root-config --cflags) -fPIC
ROOTLIBS = $(shell root-config --libs --glibs)
LIBS     = $(ROOTLIBS)
OPT      = -ggdb -fsanitize=address -Wshadow # for debug
#OPT      = -O3   # optimized
GCC       = $(shell root-config --cxx)
LD        = $(shell root-config --ld)
SOFLAGS   = -shared 
SHAREDLIB = ./framework.so
#---------DEFAULT ACTION-------------------------
all: $(PROGRAMS)

# compiling the executable 
%.exe: %.o *.h *.cxx $(SHAREDLIB)
	echo creating the executable $@
	$(LD) $(OPT) $(CXXFLAGS) -L. -o $@ $< $(SHAREDLIB) $(LIBS)
# creating the shared library
$(SHAREDLIB): $(MYLIBS) DataItemFactory.o 
	echo "Building shared library ... "
	rm -f $(SHAREDLIB)
	$(LD) -L$(shell root-config --libdir --libs --glibs) $(SOFLAGS) $(MYLIBS) -o $(SHAREDLIB)
	echo "Done"
#
%.o: %.cxx *.h DataItemFactory.cxx
	$(GCC) $(OPT) $(CXXFLAGS) -o $@ -c $<
#
DataItemFactory.cxx: DI*.h scripts/Dictionary.sh
	./scripts/Dictionary.sh
#----------CLEANUP------------------
clean:
	rm -f *.o *.so *.exe *~ $(PROGRAMS) $(FILESGENERATEDBYSCRIPTS) $(SHAREDLIB)
	rm -rf latex html
	make -C unittests clean

fullclean: clean
	rm -f *.root *.dat
	make -C unittests fullclean
	echo "done full clean in DAF"

#----------Doxygen-------------------
doxygen: clean 
	doxygen doxygen_config

#
#--COMPILING if the shared library does not work for your operating system---
# static linking of all object files 
#%.exe: %.o *.h *.cxx $(MYLIBS) DataItemCreator.o
#	$(GCC) $(OPT) $(CXXFLAGS) -L. -o $@ $< $(MYLIBS) $(LIBS) 

# Makefile for the DataAnalysis Example
# Eckhard von Toerne, Bonn U., 2009
#
# the makefiles contains all rules to create the target
# usage: make name_of_target
# the default target is "all"
#
# calling make from the command line starts the target creation pocess 
# defined by the makefile.
# The makefile consists of variable definitions and target declarations.
# The target has dependencies which have to be created before the target 
# the dependencies must also have one or several target rules.
# Syntax: 
# target_name: list of dependencies
# -TAB-target rule
# -TAB-target rule
# -TAB-target rule
#
# the make utility proceeds recursively from the main target trough the 
# chain of dependencies and their target rules to the targets that need 
# to be created first - and then creates all targets in the correct order.
#---------------
# $() constructs
# variables: defined by:  VAR = xxx , addressed via $(VAR)
# shell commands:         $(shell command)
# string substitution:    $(subst string,replacement,targetstring)
#----------------
# placeholders
# %.exe : any file that matches *.exe
# $@ Name of target
# $< the first dependence 
# $+ List of all dependencies
# $^ list of all dependencies with double listings removed
#----------------
# Important:
# always start a target rule with a TAB
