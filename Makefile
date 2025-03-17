CXX = avr-gcc
CXXFLAGS = -Wall -Os -std=gnu++11 -mmcu=attiny44 -I/usr/include/simavr -I./include/SfotI2CMaster

OBJCOPY = avr-objcopy
OBJCOPYFLAGS = -O ihex -S

BUILD_DIR=build
SRC_DIR=src

DEPS = main.cpp nrf24.cpp spi.cpp adxl345.cpp softi2c.cpp

HEADERS = $(patsubst %.cpp,%.h,$(DEPS))
OBJ=$(patsubst %.h,%.o,$(HEADERS))

PDEPS = $(patsubst %,$(SRC_DIR)/%,$(DEPS))
POBJ = $(patsubst %,$(BUILD_DIR)/%,$(OBJ))
PHEADERS = $(patsubst %,$(SRC_DIR)/%,$(HEADERS))

TARGET = attiny44.elf
PTARGET = $(BUILD_DIR)/$(TARGET)
PHEX = $(patsubst %.elf,%.hex,$(PTARGET))


all: $(PHEX)
.PHONY: all

vars :
	echo PDEPS: $(PDEPS)
	echo POBJ: $(POBJ)
	echo OBJ: $(OBJ)
	echo HEADERS: $(HEADERS)

hex : $(PHEX)
.PHONY: hex

clean:
	rm $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.hex

$(POBJ) : $(PDEPS)
	$(CXX) $(CXXFLAGS) -c $(patsubst $(BUILD_DIR)/%.o,$(SRC_DIR)/%.cpp,$@) -o $@

$(PTARGET) : $(POBJ)
	$(CXX) $(CXXFLAGS) -o $(PTARGET) $(POBJ)

$(PHEX) : $(PTARGET)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(PTARGET) $(PHEX)