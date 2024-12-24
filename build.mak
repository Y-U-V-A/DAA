
ifndef CODE_DIRS
	$(error "CODE_DIRS not defined")
endif

ifndef BIN_INT_DIR
	$(error "BIN_INT_DIR not defined")
endif

ifndef BIN_DIR
	$(error "BIN_DIR not defined")
endif

ifndef ARCH
	$(error "ARCH not defined")
endif



#to detect platform
ifeq ($(OS),Windows_NT)
	PLATFORM = WINDOWS
else
	UNAME = $(shell uname)
	ifeq ($(UNAME),Linux)
		PLATFORM = LINUX
	else
		$(error platform not supported : $(UNAME))
	endif
endif


ifeq ($(PLATFORM),WINDOWS)
#platform specific extensions
EXE_EXT = .exe
LIB_EXT = .dll
CFLAGS = -D__WIN32__
LFLAGS = 
#recursive remove directory function $(call RRMDIR,dir)
define RRMDIR
@if exist $(subst /,\$(nullstring),$(1)) rmdir /Q /S $(subst /,\$(nullstring),$(1))
endef

define MKDIR
@if not exist $(subst /,\$(nullstring),$(1)) mkdir $(subst /,\$(nullstring),$(1))
endef

#detect the machine arch
ifeq ($(ARCH),AMD64)
	ARCH_FLAG = -m64
else 
	ifeq ($(ARCH),x86)
		ARCH_FLAG = -m32
	else
		$(error machine arch not supported : $(ARCH))
	endif
endif

endif

ifeq ($(PLATFORM),LINUX)
#platform specific extensions
EXE_EXT = 
LIB_EXT = .so
CFLAGS = -D__linux__
LFLAGS = -lrt
#recursive remove directory function $(call RRMDIR,dir)
define RRMDIR
@rm -r $(1)
endef

define MKDIR
@mkdir -p $(1)
endef

#detect the machine arch
ARCH = $(shell uname -m)
ifeq ($(ARCH),x86_64)
	ARCH_FLAG = -m64
else ifeq ($(ARCH),i386)
	ARCH_FLAG = -m32
else
	$(error machine arch not supported : $(ARCH))
endif

endif

#macro $(call PRINT,info)
define PRINT
@echo "$(1)"
endef

#recursive file macro $(call GET_FILES,dir,pattern) -> pattern example *.c or *.cpp etc
#drawbacks
#should not contain empty directories (may lead to undefined)
#should not contain same file names even in different directories because (which Include flag dir will be first it will consider that)
define GET_FILES
$(foreach VAR,$(wildcard $(1)/*),$(if $(wildcard $(VAR)/*),$(call GET_FILES,$(VAR),$(2)),$(filter $(subst *,%,$(2)),$(VAR))))
endef


#bin dir
EXE_DIR = bin/$(BIN_DIR)
#bin_int dir
OBJ_DIR = bin_int/$(BIN_INT_DIR)

#getting all .c files 
CFILES = $(foreach K,$(CODE_DIRS),$(call GET_FILES,$(K),*.c))
#all .o files
OFILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(CFILES))
#all .d files
DFILES = $(patsubst %.c,$(OBJ_DIR)/%.d,$(CFILES))

#include dirs flags
IFILES = $(foreach K,$(CODE_DIRS),$(call GET_FILES,$(K),*.h))
IDIRS = $(foreach K,$(IFILES),-I$(dir $(K)))
IFLAGS = $(sort $(IDIRS))

#compiler 
CC = clang
#optimization level
OP_FLAGS = -O0
#dependency flags
#-MD will include the external headers
#-MMD will only include the workspace headers
DEP_FLAGS = -MMD 
#defines
DEFINE_FLAGS = -DDAA
#c version
C_VERSION_FLAGS = -std=gnu17

#all compiler time flags
CFLAGS += -Wall -Wextra -g -fsanitize=address -c $(ARCH_FLAG) $(C_VERSION_FLAGS) $(OP_FLAGS) $(DEFINE_FLAGS) $(DEP_FLAGS) $(IFLAGS)
#all linker time flags
LFLAGS += -g -fsanitize=address $(ARCH_FLAG) $(C_VERSION_FLAGS) $(OP_FLAGS)

TARGET = $(EXE_DIR)/EXE$(EXE_EXT)

.PHONY:all,info,clear,create_dirs,clear_bin,clear_bin_int

all:create_dirs $(TARGET)

create_dirs:
	$(call MKDIR,$(EXE_DIR))
	$(call MKDIR,$(OBJ_DIR))

$(TARGET):$(OFILES)
	@echo "linking..."
	@$(CC) $(LFLAGS) $^ -o $@

$(OBJ_DIR)/%.o:%.c
	$(call MKDIR,$(dir $@))
	@$(CC) $(CFLAGS) $< -o  $@
	@echo "compiled $(notdir $<)"

-include $(DFILES)

clear:clear_bin clear_bin_int

clear_bin:
	$(call RRMDIR,$(EXE_DIR))

clear_bin_int:
	$(call RRMDIR,$(OBJ_DIR))

info:
	@echo "PLATFORM = $(PLATFORM)"
	@echo "ARCH = $(ARCH)"
	@echo "COMPILER = $(CC)"
	@echo "COMPILER_FLAGS = $(CFLAGS)"
	@echo "LINKER_FLAGS = $(LFLAGS)"
	@echo "BIN = $(EXE_DIR)"
	@echo "BIN_INT = $(OBJ_DIR)"
	@echo "TARGET = $(TARGET)"
	@echo "CODE_FILES = $(CFILES)"
	@echo "INCLUDE_DIRS = $(subst -I,/,$(IFLAGS))"

