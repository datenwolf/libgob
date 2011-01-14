# source files.
SRC = encode.c
TEST_SRC = test_main.c encode_test.c

OBJ = $(SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)

OUT = libgob.a

# include directories
INCLUDES = -I. -I/usr/local/include

# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g

# compiler
CC = gcc

# library paths
LIBS = -L../ -L/usr/local/lib -lm

# compile flags
LDFLAGS = -g

CUNIT_LDFLAGS= -lcunit

.SUFFIXES: .c

default: $(OUT)

.c.o:
	$(CC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(OUT) Makefile.bak 

test: $(OBJ) $(TEST_OBJ)
	$(CC) $^ -o $@ -lm $(CUNIT_LDFLAGS)

exe: $(OUT) main.o
	$(CC) $^ -o $@ -lm -lgob -L. $(LDFLAGS)