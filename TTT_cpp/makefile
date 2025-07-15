PROJECTNAME = game
COMPILER = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
CXXOPTIMIZED = -std=c++17 -O2
FILES = $(wildcard  ./src/*.cpp) $(wildcard ./src/*.h)
BUILDFOLDER = ./build

# I suppose I could make object files to save time on compiling, but this is such a small project.

default: compile

compile:
	$(COMPILER) ${CXXFLAGS} $(FILES) -o $(BUILDFOLDER)/$(PROJECTNAME)

optimized:
	$(COMPILER) ${CXXOPTIMIZED} $(FILES) -o $(BUILDFOLDER)/$(PROJECTNAME)

clean:
	rm -f $(BUILDFOLDER)/$(PROJECTNAME)