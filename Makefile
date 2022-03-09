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

lab3_queue: src/lab3/queue.c src/lab3/task1_1_queue.c
	@clang $^ -o $(BIN_DIR)$@.bin

lab3_stack: src/lab3/stack.c src/lab3/task2_stack.c
	@clang $^ -o $(BIN_DIR)$@.bin

# --- /TARGETS ---------------------------------------------------------------

run_l11: lab1_1
	@ ./bin/lab1_1.bin

run_l12: lab1_2
	@ ./bin/lab1_2.bin

run_l3_queue: dirs lab3_queue
	@ ./bin/lab3_queue.bin


run_l3_stack: dirs lab3_stack
	@ ./bin/lab3_stack.bin



clean:
	@echo "Cleaning stuff"
	@rm -rf $(BIN_DIR)


.PHONY: dirs 
