#project name
NAME=PHPClassBuilder
# change application name  (executable output name)
TARGET=build/PHPClassBuilder

CMDTARGET=build/PHPClassBuilder
# compiler
CC=g++
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

#POSIX Thread library
PTHREAD=-lpthread

#winapi
WINAPI=-mwindows
SHLWAPI=-lshlwapi
COMDLG32=-lcomdlg32
#google test suite
GOOGLETEST=-lgtest -lgtest_main
CPPVERSION=-std=c++17

#MONGDB
MONGO=$$(pkg-config --libs --cflags libmongoc-1.0)
#MYSQL
MYSQL=-I"C:\Program Files\MySQL\MySQL Connector C++ 8.0\include\jdbc" -L"C:\Program Files\MySQL\MySQL Connector C+ 8.0\lib64"
#SQLITE
SQLITE=-l sqlite3

CMDCCFLAGS=$(CPPVERSION) $(DEBUG) $(OPT) $(WARN)
# linker
LD=g++

#LDFLAGS= -export-dynamic

GUIOBJS=winmain.o \
		csvParser.o \
		utilities.o \
		jsonParser.o\
		sqlBuilder.o

GUIOBJSDIR = objs/winmain.o \
			 objs/csvParser.o\
			 objs/utilities.o \
			 objs/jsonParser.o\
			 objs/sqlBuilder.o\

CMDOBJS=main.o \
		PHPBuilder.o

CMDOBJSDIR=objs/main.o \
		   objs/PHPBuilder.o

all: $(OBJS)
	make cmd

cmd: $(CMDOBJS)
	$(LD) -o $(CMDTARGET) $(CMDOBJSDIR) $(CMDCCFLAGS)
	
#gui: $(GUIOBJS)
#	$(LD) -o $(TARGET) $(CCFLAGS) $(GUIOBJSDIR)
PHPBuilder.o:
	$(CC) -c $(CMDCCFLAGS) src/PHPBuilder.cpp -o objs/PHPBuilder.o

main.o:
	$(CC) -c $(CMDCCFLAGS) src/main.cpp -o objs/main.o
    
#winmain.o: src/winmain.cpp
#	$(CC) -c $(CCFLAGS) src/winmain.cpp -o objs/winmain.o

run:
	./$(CMDTARGET) -l sampleClass.php

clean:
	rm -f objs/*.o $(TARGET)
