CC	:= g++
CPP_FLAGS := -std=c++17 -Wall -Wextra -g

BIN	:= bin
SRC	:= src
INCLUDE	:= include
LIB	:=lib
LIBRARIES	:=

EXECUTABLE	:= main.exe

all: $(BIN)/$(EXECUTABLE)
clean:
	$(RM) $(BIN)/$(EXECUTABLE)

# run: all
# 	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CC) $(CPP_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)


