CC=clang
MKDIR=mkdir -p

BIN_DIR=bin/

all: dirs lab1


# --- PREREQUISITES ----------------------------------------------------------

dirs:
	@echo "Creating directories..."
	@$(MKDIR) $(BIN_DIR)

# --- /PREREQUISITES ---------------------------------------------------------


# --- TARGETS ----------------------------------------------------------------

lab1_1: src/lab1/task1.c dirs
	@clang $< -o $(BIN_DIR)$@.bin

lab1_2: src/lab1/task2.c dirs
	@clang $< -o $(BIN_DIR)$@.bin

# --- /TARGETS ---------------------------------------------------------------

run: lab1_1
	@ ./bin/lab1_1.bin

run: lab1_2
	@ ./bin/lab1_2.bin

.PHONY: dirs