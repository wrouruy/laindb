CC=g++
CCFLAGS=-std=c++17 -Wall -Wextra
TARGET=darko

SRC=src/main.cpp \
	src/execute.cpp \
	src/resp/parse.cpp \
	src/storage/hashtable.cpp
OBJ=$(SRC:.cpp=.o)

$(TARGET): $(OBJ)
	$(CC) $(CCFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
