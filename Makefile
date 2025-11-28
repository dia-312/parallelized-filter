CXX = g++
CXXFLAGS = -O2 -std=c++17 -pthread

all: apply_filter papply_filter

apply_filter: apply_filter.cpp
	$(CXX) $(CXXFLAGS) -o apply_filter apply_filter.cpp

papply_filter: papply_filter.cpp
	$(CXX) $(CXXFLAGS) -o papply_filter papply_filter.cpp

clean:
	rm -f apply_filter papply_filter *.o

