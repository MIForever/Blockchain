TARGET = my_blockchain
SRC = $(wildcard *.c *.h)
OBJ = $(filter %.o,$(patsubst %.c,%.o,$(CPP_SOURCES)) \ $(patsubst %.c,%.o,$(SRC)))

CFLAGS = -Wall -Wextra -Werror

all : $(TARGET)

$(TARGET): $(OBJ)
	gcc $(CFLAGS) -o $(TARGET) $(OBJ)

$(OBJ): $(SRC)
	gcc $(CFLAGS) -c $(SRC)

clean:
	rm -f *.o *.gch

fclean: clean
	rm -f $(TARGET)

re: fclean all