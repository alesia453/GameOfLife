CC = gcc
CFLAGS = -O2 -Wall
SRC = gameoflife.c
OUT = a.out

INPUT_FILES = $(wildcard InputData/data*.in)
OUTPUT_FILES = $(wildcard OutputData/data*.out)

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT) $(INPUT_FILES) 

clean:
	rm -f $(OUT)
