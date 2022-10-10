
RELEASE_MACRO:=-D RELEASE
DEBUG_MACRO:=-D DEBUG
OS_MACRO:=-D
BIN     := bin
SRC     := src
INCLUDE := include
LIB     := lib
#Check the OS
ifeq ($(OS),Windows_NT)
# the compiler: gcc for C program, define as g++ for C++
CC	:= g++
# The windows command for delete
DEL := del
# The windows slash for directory
SLASH := "\"
# Indicating that we dont need wine, unlike in linux
WINE := 
OS_MACRO +=__WINDOWS__
# SOURCES := $(wildcard $(SRC)/**/*.cpp) $(wildcard $(SRC)/*.cpp) $(wildcard $(INCLUDE)/**/*.cpp) $(wildcard $(INCLUDE)/*.cpp)
SOURCES := $(dir $(shell dir /A-D /B /S $(YOUR_DIRECTORY)*.cpp ))
uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))

SOURCES:=$(sort $(SOURCES))
SOURCES:=$(call uniq,$(SOURCES))
SOURCES:=$(subst \,/, $(SOURCES))
else
# Linux compiler for windows executables (64 bits)
CC	:= x86_64-w64-mingw32-g++
# Linux command for delete (rm -f)
DEL := $(RM)
# Linux slash for directory
SLASH := '/'
# Wine, program for executing windows apps .exe
WINE := wine
OS_MACRO +=__LINUX__
SOURCES := $(shell dir . -r *.cpp)

endif


# compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
CFLAGS:= -g -Wall


# libraries to link to the project
LIBRARIES   := -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_mixer -lmingw32 -lopengl32 -lglew32 -lglu32 -lSDL2main -lSDL2 -lSDL2_image 
# the build target executable
EXECUTABLE  := game


all: build
# 
showfiles:
	@echo $(SOURCES)
	
# Builds the executable game
build:
	echo "Building..."
	$(CC) $(CFLAGS) -I $(INCLUDE) -L $(LIB)  $(OS_MACRO) $(DEBUG_MACRO) -o $(BIN)/$(EXECUTABLE) $(SOURCES) $(LIBRARIES) 

# Run the game
run:
	@echo "Executing..."
	$(WINE) $(BIN)/$(EXECUTABLE)
# Build then run the game
build-and-run: clean build run
# Remove the executable
clean:
	@echo "Clearing..."
	$(DEL) bin$(SLASH)$(EXECUTABLE).exe
