
INCLUDE_DIRS = common containers source

CODE_DIRS = common containers source source/sorting_algo source/stack_problems


#bin dir
BIN_DIR = bin
#bin_obj dir
OBJ_DIR = bin_int/source

#compiler
CC = clang
#optimization
OP = -O0
#compiler flags
#-std=gnu17: Uses the C17 standard with GNU extensions.
CCFLAGS = -Wall -Wextra -MD -g -std=gnu17 $(OP) $(foreach K,$(INCLUDE_DIRS), -I$(K))

CFILES = $(foreach K,$(CODE_DIRS),$(wildcard $(K)/*.c))
OFILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(CFILES))
DFILES = $(patsubst %.c,$(OBJ_DIR)/%.d,$(CFILES))
TARGET = $(BIN_DIR)/bin.exe

source:$(TARGET)

$(TARGET):$(OFILES)
	@echo "linking..."
	@if not exist $(subst /,\,$(BIN_DIR)) mkdir $(subst /,\,$(BIN_DIR))
	@$(CC) -fsanitize=address $^ -o $@
	@echo "Done..."

#$(dir $@)     # Expands to "src/"
#$(notdir $@)  # Expands to "main.o"

$(OBJ_DIR)/%.o:%.c
	@if not exist $(subst /,\,$(OBJ_DIR)) mkdir $(subst /,\,$(OBJ_DIR))
	@if not exist $(subst /,\,$(dir $@)) mkdir $(subst /,\,$(dir $@))
	@$(CC) $(CCFLAGS) -fsanitize=address -c $< -o $@
	@echo "compiled $(notdir $<)"


-include $(DFILES) #include dependency files to detect change dont forget


clean:
	@del /Q /S $(subst /,\,$(OBJ_DIR))
	@del /Q /S $(subst /,\,$(BIN_DIR)/bin.exe)
	@del /Q /S $(subst /,\,$(BIN_DIR)/bin.pdb)
	@del /Q /S $(subst /,\,$(BIN_DIR)/bin.lib)
	@del /Q /S $(subst /,\,$(BIN_DIR)/bin.exp)

print:
	@echo "$(CFILES)"
	@echo "$(OFILES)"


