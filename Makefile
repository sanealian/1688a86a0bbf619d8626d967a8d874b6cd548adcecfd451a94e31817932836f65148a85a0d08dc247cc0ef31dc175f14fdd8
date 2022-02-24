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

lab2_1: src/lab2/task1.c dirs
	clang $< -o $(BIN_DIR)$@.bin

# --- /TARGETS ---------------------------------------------------------------

run_l11: lab1_1
	@ ./bin/lab1_1.bin

run_l12: lab1_2
	@ ./bin/lab1_2.bin


clean:
	@echo "Cleaning stuff"
	@rm -rf $(BIN_DIR)


.PHONY: dirs 
