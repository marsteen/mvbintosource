#************************************************************
#
#  makefile for GNU C++
#
#************************************************************

#************************************************************
#
# CC: name of C++ compiler
#
#************************************************************

CC = g++

#************************************************************
#
# library and include path names
#
#************************************************************

#
LIB        = ./lib
INCLUDE    = ./include

#************************************************************
#
# c++ source files
#
#************************************************************

CCFILES  = main.cpp \
           $(LIB)/CFileIO.cpp \
           $(LIB)/NStringTool.cpp \

#************************************************************
#
# library files
#
#************************************************************

LINKLIBS    = 

#************************************************************
#
# C++ flags
#
# -c  = compile only
# -O2 = optimize level 2
# -s  = remove symboltables
#
#************************************************************

CCFLAGS  = -O2 -I$(INCLUDE)

#************************************************************
#
# Rule for making .o-file from .cpp-file
#
# $@ = Name of the target (.out)
# $< = Name of dependency (.cpp)
#
#************************************************************

.cpp.o:
	$(CC) -c $(CCFLAGS) -o$@ $<

#************************************************************
#
# List of object-files
#
#************************************************************

CCOBJECTS  = $(CCFILES:.cpp=.o)

#************************************************************
#
# Name of target
#
#************************************************************

TARGET  = bin2source

#************************************************************
#
# Rule for default-target
#
#************************************************************

default: $(TARGET)

#************************************************************
#
# Rule for clean-up
#
#************************************************************

clean:
	rm $(CCOBJECTS)

#************************************************************
#
# Rule for creating target from objects
#
#************************************************************

$(TARGET): $(CCOBJECTS) makefile
	$(CC) -o $(TARGET) $(CCFLAGS) $(CCOBJECTS)
	strip $(TARGET)	

