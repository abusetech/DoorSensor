CXX = avr-gcc

CXXFLAGS = -Wall -Os -std=gnu++11 -g -mmcu=attiny44 -I/usr/include/simavr
#CCFLAGS is only used for compiling the simavr object file as it is the only c file in the project
CCFLAGS = -Wall -Os -std=gnu99 -mmcu=attiny44 -I/usr/include/simavr

OBJCOPY = avr-objcopy
OBJCOPYFLAGS = -O ihex -S

BUILD_DIR=build
SRC_DIR=src

SIMAVR_C = simavr.c
SIMAVR_H = simavr.h
SIMAVR_OBJ = simavr.o

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

#Since C11 does not support designated initializer lists, we must build the simavr header as an
#object file and then link it in separately.
#see https://github.com/buserror/simavr/issues/458
$(SIMAVR_OBJ):
	$(CXX) $(CCFLAGS) -c $(SRC_DIR)/$(SIMAVR_C) -o $(BUILD_DIR)/$(SIMAVR_OBJ)

$(POBJ) : $(PDEPS) 
	$(CXX) $(CXXFLAGS) -c $(patsubst $(BUILD_DIR)/%.o,$(SRC_DIR)/%.cpp,$@) -o $@

$(PTARGET) : $(POBJ) $(SIMAVR_OBJ)
	$(CXX) $(CXXFLAGS) -o $(PTARGET) $(POBJ) $(BUILD_DIR)/$(SIMAVR_OBJ)

$(PHEX) : $(PTARGET)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(PTARGET) $(PHEX)