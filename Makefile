TARGET=./main
CC=g++
SRCS=\
	./project/src/main.cpp \
    ./project/src/test.cpp \
	./project/MyTest/*.cpp
LIBS=\
    -ljpeg # Добавить  -lmmal -lmmal_core -lmmal_util, если не будет работать

# На mac OS он не подтягивает автоматически библиотеки надо прямо путь указать
JPEGLIB=-I /opt/homebrew/Cellar/jpeg-turbo/2.1.4/include -L /opt/homebrew/Cellar/jpeg-turbo/2.1.4/lib

STD=-std=c++17
WER=-Wall -Wextra -Werror -ansi

all: clean $(TARGET)

apple: clean m 

m:
	$(CC) $(WER) -O3 $(STD) $(JPEGLIB) $(LIBS) -lm -o $(TARGET) $(SRCS)

$(TARGET): 
	$(CC) $(WER) -O3 $(STD) $(LIBS) -lm -o $(TARGET) $(SRCS)

build: $(TARGET)

clean:
	rm -rf $(TARGET)
