CXX = avr-gcc
CXXFLAGS = -Wall -Os -std=gnu11 -mmcu=attiny44
BUILD_DIR = build
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c)
OBJS = $(subst $(SRC_DIR),$(BUILD_DIR),$(patsubst %.c,%.o,$(SRCS:.cpp=.o)))
TARGET = $(BUILD_DIR)/main.elf
BIN = $($(TARGET):.elf,.hex)

all: $(BIN)

$(OBJS): $(SRCS)
	$(CXX) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/$@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(BIN): $(TARGET)
	avr-objcopy -O ihex $(TARGET) $(BIN)