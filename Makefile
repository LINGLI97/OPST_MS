# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -g -DVISUALIZATION
#-msse3 -O3 -fomit-frame-pointer -funroll-loops
 #-DVISUALIZATION
#-DVISUALIZATION
#-msse3 -O3 -fomit-frame-pointer -funroll-loops
#-g -msse3 -O3 -fomit-frame-pointer -funroll-loops

# -msse3 -O3 -fomit-frame-pointer -funroll-loops
#-DVISUALIZATION

#CXXFLAGS = -std=c++11 -O3 -g

LDFLAGS = -L~/lib/
LIBS = -lsdsl -ldivsufsort -ldivsufsort64


# CXXFLAGS += -I/path/to/sdsl/include
# LDFLAGS += -L/path/to/sdsl/lib

SOURCES = main.cpp node.cpp OPST.cpp baseline.cpp utils.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = my_program



all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(TARGET) $(OBJECTS)











maximalOPST_MS_SOURCES = maximalOPST_MS.cpp node.cpp OPST_MS.cpp utils.cpp oracle.cpp DerivedWTInt.cpp
maximalOPST_MS_OBJECTS = $(maximalOPST_MS_SOURCES:.cpp=.o)
maximalOPST_MS_TARGET = OPSTMaximal_MS




$(maximalOPST_MS_TARGET): $(maximalOPST_MS_OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean_OPSTMaximal_MS:
	rm -f $(maximalOPST_MS_TARGET) $(maximalOPST_MS_OBJECTS)





.PHONY: all clean


