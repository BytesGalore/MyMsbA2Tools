TARGET = msba2Server
LIBS = -lpthread -lboost_system -lboost_filesystem -L/usr/local/lib/ -lboost_regex
CC = g++
CPPFLAGS = -D__GXX_EXPERIMENTAL_CXX0X__ -g3 -Wall -O0 -std=c++0x

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS := $(patsubst %.cpp, %.o, $(wildcard *.cpp))
OBJECTS += $(patsubst %.cpp, %.o, $(wildcard ../ttyConnector/*.cpp))
HEADERS := $(wildcard *.hpp) 
HEADERS	+= $(wildcard ../ttyConnector/*.hpp)

%.o: %.cpp $(HEADERS)
	$(CC) $(CPPFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f ../ttyConnector/*.o
	-rm -f $(TARGET)
