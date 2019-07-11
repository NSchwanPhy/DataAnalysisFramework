# Makefile for ...
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
#---------GENERAL SETTINGS--------------
PROGRAMS  = uttest.exe 
FILESGENERATEDBYSCRIPTS = 
MYLIBCXX  = $(subst .h,.cxx,$(shell ls *.h)) 
MYLIBS    = $(subst .cxx,.o,$(shell ls $(MYLIBCXX) 2>/dev/null))
OTHERLIBS = 
ROOTSYS   = $(shell echo $ROOTSYS)
# -fPIC option needed only for compilation into a shared library
MYFLAGS  = #-DVERBOSE   # for debug output
CXXFLAGS = $(shell root-config --cflags) -fPIC -I../. 
ROOTLIBS = $(shell root-config --libs) #-lEG -lPythia6 -lEGPythia6
LIBS     = $(ROOTLIBS)
OPT      = -ggdb -fsanitize=address -g3 # for debug
#OPT      = -O3   # optimized
ADDWARNINGS = #-Wold-style-cast  #-Wall 
GCC       = g++ $(ADDWARNINGS)
SOFLAGS   = -shared 
SHAREDLIB = ../framework.so
UTSHAREDLIB = ./unittests.so
#---------DEFAULT ACTION-------------------------
all: $(PROGRAMS)
	./uttest.exe
# compiling the executable 
%.exe: %.o *.h *.cxx $(SHAREDLIB) $(UTSHAREDLIB)
	echo creating the executable $@
	$(GCC) $(OPT) $(CXXFLAGS) -L. -o $@ $< $(SHAREDLIB) $(UTSHAREDLIB) $(LIBS)  
# creating the shared library
%.o: %.cxx *.h
	$(GCC) $(OPT) $(CXXFLAGS) -o $@ -c $<
#
$(SHAREDLIB): ../*.h ../*.cxx
	$(MAKE) -C ..
#
$(UTSHAREDLIB): $(MYLIBS) $(OTHERLIBS)
	echo "Building unit test shared library ... "
	rm -f $(UTSHAREDLIB)
	$(GCC) -L$(shell root-config --libdir) $(SOFLAGS) $(MYLIBS) -o $(UTSHAREDLIB) $(LIBS)
	echo "Done"
#
#----------CLEANUP------------------
clean:
	rm -f *.o *.so *.exe *~ $(PROGRAMS)

fullclean: clean
	rm -f *.root *.dat
	echo "done full clean in unittest"
