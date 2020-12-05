CPP = g++
## optimization options: O0 - no optimization, O3 - max optimization
## O0 is fastest to compile and easiest to debug
## O3 gives fastest code
OPTIMIZE = -O3  
FINAL = raymarch  # final file name
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
# sfml libraries to link

SOURCES = $(wildcard *.cpp)  # all .cpp files
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

# make rules in the form
# target: prerequisites
# <tab>   rule -- how to make target from the prerequisites
all: $(FINAL)
	echo "all done :-)"

%.o: %.cpp color.hpp
	$(CPP) -c $(OPTIMIZE) -Wall $<
# $< matches the first prerequisite

$(FINAL): $(OBJECTS)
	$(CPP) $^ -o $@ $(SFML_LIBS)
# apparently objects must be before libraries in the attribute list
# $^ means _all objects_
# $@ matches the target $(FINAL)

clean:
	rm -vf $(OBJECTS) *~
