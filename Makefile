CXX = avr-gcc
CXXFLAGS = -Wall -Os -std=c++11 -mmcu=attiny44

BUILD_DIR=build
SRC_DIR=src

DEPS = main.cpp nrf24.cpp spi.cpp

HEADERS = $(patsubst %.cpp,%.h,$(DEPS))
OBJ=$(patsubst %.h,%.o,$(HEADERS))

PDEPS = $(patsubst %,$(SRC_DIR)/%,$(DEPS))
POBJ = $(patsubst %,$(BUILD_DIR)/%,$(OBJ))
PHEADERS = $(patsubst %,$(SRC_DIR)/%,$(HEADERS))

TARGET = main

vars :
	echo PDEPS: $(PDEPS)
	echo POBJ: $(POBJ)
	echo OBJ: $(OBJ)
	echo HEADERS: $(HEADERS)
all: $(TARGET)
	.PHONY: all
$(POBJ) : $(PDEPS)
	$(CXX) $(CXXFLAGS) -c $(patsubst $(BUILD_DIR)/%.o,$(SRC_DIR)/%.cpp,$@) -o $@
$(TARGET) : $(POBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(POBJ)
