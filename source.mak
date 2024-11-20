
INCLDIRS = core core/common core/containers core/platform source/algorithms source/problems

CODEDIRS = core core/common core/containers core/platform\
source\
source/algorithms/sorting\
source/algorithms/graph\
source/problems/stack\
source/problems/stack/conversions\
source/problems/heap\


#compiler
CC = clang
#optmization level
OP = -O0
#defines
DEFINES = 
#linker flags
LFLAGS = -fsanitize=address 
#compiler flags
CFLAGS = -Wextra -Wall -fsanitize=address -g -std=gnu17 -MD  $(OP) $(DEFINES) $(foreach dir,$(INCLDIRS),-I$(dir))


ifeq ($(OS),Windows_NT)
	PLATFORM = Windows
else 
	PLATFORM = $(shell uname -s)
endif
#gets name of os $(shell -uname -s)

#bin dir
BINDIR = bin

#object dir
OBJDIR = bin.int/source.$(PLATFORM)

#/Q dont ask 
#/S recursive
ifeq ($(PLATFORM),Windows)
	STEP = \\$(nullstring)
	RM = del /Q /S
	MKDIR = mkdir
	EXE = .exe
else
	ifeq ($(PLATFORM),Linux)
		STEP = /
		RM = rm
		MKDIR = mkdir -p
		EXE =
	else
		$(error This Makefile is not compatible with the OS)
	endif
endif



CFILES = $(foreach K,$(CODEDIRS),$(wildcard $(K)/*.c))
OFILES = $(patsubst %.c,$(OBJDIR)/%.o,$(CFILES))
DFILES = $(patsubst %.c,$(OBJDIR)/%.d,$(CFILES))

TARGET = $(BINDIR)/DAA$(EXE)

.PHONY:all clean

all:make_dirs $(TARGET)
	@echo "Done..."

make_dirs:
ifeq ($(PLATFORM),Windows)
	@if not exist $(subst /,$(STEP),$(BINDIR)) $(MKDIR) $(subst /,$(STEP),$(BINDIR))
	@if not exist $(subst /,$(STEP),$(OBJDIR)) $(MKDIR) $(subst /,$(STEP),$(OBJDIR))
else
	@$(MKDIR) $(BINDIR)
	@$(MKDIR) $(OBJDIR)
endif

$(TARGET):$(OFILES)
	@echo "linking..."
	@$(CC) $(LFLAGS) $^ -o $@

$(OBJDIR)/%.o:%.c
ifeq ($(PLATFORM),Windows)
	@if not exist $(subst /,$(STEP),$(dir $@)) $(MKDIR) $(subst /,$(STEP),$(dir $@))
else
	@$(MKDIR) $(dir $@)
endif
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "compiled $<"

# '-' symbol makes it optional , doesnt give any error if file is not avaliable
-include $(DFILES)

clean:
	@$(RM) $(subst /,$(STEP),$(OFILES))
	@$(RM) $(subst /,$(STEP),$(DFILES))
	@$(RM) $(subst /,$(STEP),$(BINDIR))