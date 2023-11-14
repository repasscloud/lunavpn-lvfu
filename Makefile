CXX = g++
CXXFLAGS = -std=c++11
SRC = lvfu.cpp
TARGET_DIR = ./out
TARGET = $(TARGET_DIR)/lvfu
NLOHMANN_PATH = ./nlohmann/json

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) -I$(NLOHMANN_PATH) $(SRC) -lcurl

clean:
	rm -rf $(TARGET_DIR)
