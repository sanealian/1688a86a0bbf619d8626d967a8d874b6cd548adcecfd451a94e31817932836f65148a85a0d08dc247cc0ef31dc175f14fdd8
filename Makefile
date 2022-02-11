# Declare some default executable commands
# 
CC=clang
MKDIR=mkdir -p


# Declare default locations
#
BIN_DIR=bin/
BUILD_DIR=build/
SRC_DIR=src/
EXC_NAME=$(BIN_DIR)application

PRE_DIR=$(BUILD_DIR)preprocessed/
IRS_DIR=$(BUILD_DIR)irs/
ASM_DIR=$(BUILD_DIR)assembled/
OBJ_DIR=$(BUILD_DIR)translations/



# DECLARE TARGETS
sources=main.c

# preprocessed
pre_targets=$(addprefix $(PRE_DIR),$(sources:.c=.i))

# itermediate representation
irs_targets=$(addprefix $(IRS_DIR),$(sources:.c=.ll))

# assembly
asm_targets=$(addprefix $(ASM_DIR),$(sources:.c=.s))

# object files (translation units)
obj_targets=$(addprefix $(OBJ_DIR),$(sources:.c=.o))


# Pretty print
define pprint
	@echo "\n\n$(1)"
	@echo "------------------"
endef


# BUILD TARGET
# ____________________________________________________________________
build: dirs $(pre_targets) $(irs_targets) $(asm_targets) $(obj_targets) $(EXC_NAME)


# BUILD PROJECT
# ____________________________________________________________________
#
# FEED PREPROCESSOR
# -----------------------
desc_pre:
	$(call pprint,"Preprocessing...")
$(pre_targets): $(PRE_DIR)%.i: $(SRC_DIR)%.c desc_pre
	@echo "Source: " $< "|| Target: "  $@
	@$(CC) -E $< -o $@


# COMPILE TO IR
# -----------------------
desc_irs:
	$(call pprint,"Making IRs...")
$(irs_targets): $(IRS_DIR)%.ll: $(SRC_DIR)%.c desc_irs
	@echo "Source: " $< "|| Target: "  $@
	@$(CC) -S -emit-llvm $< -o $@


# COMPILE TO ASSEMBLY CODE
# -----------------------
desc_asm:
	$(call pprint,"Assembling...")
$(asm_targets): $(ASM_DIR)%.s: $(IRS_DIR)%.ll desc_asm
	@echo "Source: " $< "|| Target: "  $@
	@$(CC) -S -mllvm --x86-asm-syntax=intel $< -o $@


# CREATE TRANSLATION UNITS
# ------------------------
desc_obj:
	$(call pprint,"Making translation units...")
$(obj_targets): $(OBJ_DIR)%.o: $(SRC_DIR)%.c desc_obj
	@echo "Source: " $< "|| Target: "  $@
	@$(CC) -c $< -o $@


# LINKING
# -----------------------
$(EXC_NAME): $(obj_targets)
	$(call pprint,"Linking...")
	@echo "Source: " $< "|| Target: "  $@
	@$(CC) -Wall $^ -o $(EXC_NAME)
#
# END: BUILDING
# ____________________________________________________________________



# PREPARE ENVIRONMENT
# ____________________________________________________________________
#
dirs:
	@echo "Creating directories..."
	@$(MKDIR) $(BIN_DIR)
	@$(MKDIR) $(BUILD_DIR)
	@$(MKDIR) $(PRE_DIR)
	@$(MKDIR) $(ASM_DIR)
	@$(MKDIR) $(OBJ_DIR)
	@$(MKDIR) $(IRS_DIR)

 
# CLEAN UP
# ____________________________________________________________________
sweep:
	@echo "Cleaning stuff"
	@rm -rf build bin


.PHONY: clean build desc_lnk
