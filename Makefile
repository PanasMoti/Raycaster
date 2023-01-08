# project name
PROJECT = RayCaster

# directories
DEBUG_DIR = debug
RELEASE_DIR = release
BASE_BUILD_DIR = bin

RESOURCES_DIR = res
SOURCES_DIR = src
HEADERS_DIR = include
IMPORTS_DIR = $(HEADERS_DIR)/imports

# c++ compiler
CC = g++
# compiler flags
COMPILER_FLAGS = -std=c++17 -w -I$(HEADERS_DIR)

# libraries

SDL2 = -lSDL2
WIN_FREEGLUT = -lfreeglut -lopengl32 -lglu32
LINUX_FREEGLUT = -lGL -lGLU -lglut -lm 

#linker flags
WIN_LINKER_FLAGS = $(SDL2) $(WIN_FREEGLUT)
LINUX_LINKER_FLAGS = $(SDL2) $(LINUX_FREEGLUT)

# files

SOURCES = $(SOURCES_DIR)/*
IMPORTS = $(IMPORTS_DIR)/sources/*

# executable name

EXE_NAME = $(PROJECT)


lbuild:
	$(CC) $(SOURCES) $(IMPORTS) $(COMPILER_FLAGS) -o3 $(LINUX_LINKER_FLAGS) -o $(BASE_BUILD_DIR)/$(EXE_NAME).o 
lrun:
	$(BASE_BUILD_DIR)/$(EXE_NAME).o

ldebug:
	$(CC) $(SOURCES) $(IMPORTS) $(COMPILER_FLAGS) -o0 $(LINUX_LINKER_FLAGS) -o $(BASE_BUILD_DIR)/$(DEBUG_DIR)/$(EXE_NAME).o
	$(BASE_BUILD_DIR)/$(DEBUG_DIR)/$(EXE_NAME).o

lrelease:
	$(CC) $(SOURCES) $(IMPORTS) $(COMPILER_FLAGS) -o3 $(LINUX_LINKER_FLAGS) -o $(BASE_BUILD_DIR)/$(RELEASE_DIR)/$(EXE_NAME).o
	$(BASE_BUILD_DIR)/$(RELEASE_DIR)/$(EXE_NAME).o




