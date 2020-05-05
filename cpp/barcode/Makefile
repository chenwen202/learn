CC		:= C:/mingw64/bin/g++
C_FLAGS := -std=c++17 -Wall -Wextra -g 

BIN		:= bin
SRC		:= src
INCLUDE	:= include
INCLUDE += -IC:/cv_build_minggw/install/include -IF:/work/workspace/SynologyDrive/libs/win64/include
LIB		:= lib
LIB		+=  -LC:/cv_build_minggw/install/x64/mingw/lib -LF:/work/workspace/SynologyDrive/libs/win64/lib
LIBRARIES	:= -llibopencv_core410 \
	-llibopencv_highgui410 \
    -llibopencv_imgcodecs410 \
    -llibopencv_imgproc410 \
    -llibopencv_features2d410 \
    -llibopencv_calib3d410	\
	-llibzbar



ifeq ($(OS),Windows_NT)
EXECUTABLE	:= main.exe
else
EXECUTABLE	:= main
endif

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)