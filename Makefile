# Make the `hashtest' binary.
# conditional compilation options
CONFIG =
override CONFIG += -DDEBUG # enable debug mode

# compilation flags and sources
CFLAGS      = -O3 -Wall
LDFLAGS     =

BIN         = hashtest
BIN_DIR     = build
SRC_DIR     = src
SRCS        = $(wildcard $(SRC_DIR)/*.c)
OBJS        = $(patsubst $(SRC_DIR)/%.c, $(SRC_DIR)/%.o, $(SRCS))

.PHONY: all test clean help

all: $(BIN_DIR)/$(BIN)

$(BIN_DIR)/$(BIN): $(OBJS)
	mkdir -p $(BIN_DIR)
	gcc $(LDFLAGS) $^ -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	gcc $(CFLAGS) $(CONFIG) -c $< -o $@

test: $(BIN_DIR)/$(BIN)
	@./$<

clean:
	rm -f $(SRC_DIR)/*.o $(BIN_DIR)/$(BIN)
	@rmdir $(BIN_DIR) 2>/dev/null || echo "no build directory found"

help:
	@echo "Building is as easy as typing 'make'."
