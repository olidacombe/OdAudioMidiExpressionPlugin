SOURCES = TestRunner.cpp # $(wildcard *.cpp)  # $(wildcard ../Source/*.cpp)

OBJECTS    = $(patsubst %.cpp, %.o, $(SOURCES))
CXX = g++
CXXFLAGS  = -DDEBUG=1 -I../Source -I../JUCE/modules -std=c++11

Test: $(OBJECTS)
#Test: $(SOURCES)
	$(CXX) -o $@ $^

test: Test
	./Test

clean:
	rm -f $(OBJECTS) Test
