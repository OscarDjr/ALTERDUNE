CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC      = src/main.cpp \
           src/Entity.cpp \
           src/ActAction.cpp \
           src/Monster.cpp \
           src/Inventory.cpp \
           src/Player.cpp \
           src/FileLoader.cpp \
           src/Game.cpp
TARGET   = alterdune

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
