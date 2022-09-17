TARGET=./main
CC=g++
SRCS=\
	./project/src/main.cpp \
    ./project/include/Mat.h \
    ./project/include/SMatrix.h \
    ./project/include/utils.h \
    ./project/src/utils.cpp \
    ./project/include/correlation.h
LIBS=\
    -lwiringPi \
    -ljpeg \
    -lraspicam
# добавить еще -lmmal -lmmal_core -lmmal_util , если не будет работать
SVN=14  # Версия cpp

all: clean $(TARGET)

$(TARGET): # -Wall -Wextra -Werror -ansi
	$(CC) -std=c++$(SVN) -lm -o $(TARGET) $(addprefix -I,$(HDRS)) $(CFLAGS) $(SRCS) $(LIBS)

build: $(TARGET)

clean:
	rm -rf $(TARGET)
