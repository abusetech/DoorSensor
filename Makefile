CXX = avr-gcc
CXXFLAGS = -Wall -Os -std=c++11 -mmcu=attiny44
BUILD_DIR = build
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c)
OBJS = ${subst $(SRC_DIR),$(BUILD_DIR),$(patsubst %.c,%.o,$(SRCS:.cpp=.o))}
TARGET = $(BUILD_DIR)/main.elf
BIN = $($(TARGET):.elf,.hex)

all: $(TARGET)
	echo $(OBJS)

$(OBJS): $(SRCS)
	echo Building $(OBJS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

hex: $(TARGET)
	avr-objcopy -O ihex $(TARGET) $(BIN)