CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRCDIR = src
INCDIR = include
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
EXECUTABLE = $(BINDIR)/LincolnLibrary

# # Debugging output
# $(info SOURCES = $(SOURCES))
# $(info OBJECTS = $(OBJECTS))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(BINDIR)
