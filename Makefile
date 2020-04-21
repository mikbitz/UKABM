#############################################################################
# Simple makefile for building: ukmodel
#############################################################################

####### Compiler, tools and options

CXX           = g++
DEFINES       =
CXXFLAGS      = -pipe -std=gnu++11 -O3 -fopenmp -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I.  -I/usr/include 
LINK          = g++
LFLAGS        = -Wl,-O1 
LIBS          = $(SUBLIBS)  -L/usr/lib64 -lboost_date_time -lpthread -lgomp

DEL_FILE      = rm -f
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Directories

OBJDIR   = ./
SRCDIR   = ./

####### Files

SOURCES       = main.cpp \
		model.cpp \
		searchGrid.cpp \
		parameters.cpp \
		timing.cpp \
		layer.cpp \
		places.cpp \
		disease.cpp \
		asciiGrid.cpp \
		movement.cpp \
		populationBuilder.cpp \
		agent.cpp 
 
OBJECTS       = $(SOURCES:.cpp=.o)

TARGET        = ukmodel

first: all
####### Implicit rules

.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

model.o: model.cpp model.h config.h agents.h agentFactory.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
agent.o: agent.cpp timetable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

clean: 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first


