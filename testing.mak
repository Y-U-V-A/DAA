
INCLDIRS = core core/common core/containers core/platform testing testing/common testing/containers

CODEDIRS = core core/common core/containers core/platform testing testing/common testing/containers


#platform detection
ifeq ($(OS),Windows_NT)
	PLATFORM = Windows
else 
	PLATFORM = $(shell uname -s)
endif
#gets name of OS in windows $(OS)
#gets name of OS  in linux $(shell -uname -s)

#/Q dont ask 
#/S recursive
ifeq ($(PLATFORM),Windows)
	STEP = \\$(nullstring)
	RM = del /Q /S
	MKDIR = mkdir
else
	ifeq ($(PLATFORM),Linux)
		STEP = /
		RM = rm
		MKDIR = mkdir -p
	else
		$(error This Makefile is not compatible with the OS)
	endif
endif


#compiler
CC = clang

#optmization level
OP = -O0

#defines
DEFINES = 

#compiler flags
CFLAGS = -Wextra -Wall -fsanitize=address -g -std=gnu17 -MD  $(OP) $(DEFINES) $(foreach dir,$(INCLDIRS),-I$(dir))

#bin dir
BINDIR = bin

#object dir
OBJDIR = bin.int/testing.$(PLATFORM)

#linker flags
LFLAGS = -fsanitize=address

#target file
ifeq ($(PLATFORM),Windows)
	TARGET = $(BINDIR)/DAA.exe
endif
ifeq ($(PLATFORM),Linux)
	TARGET = $(BINDIR)/DAA
endif

#code path/file names
CFILES = $(foreach K,$(CODEDIRS),$(wildcard $(K)/*.c))
#object path/file names
OFILES = $(patsubst %.c,$(OBJDIR)/%.o,$(CFILES))
#dependency path/file names
DFILES = $(patsubst %.c,$(OBJDIR)/%.d,$(CFILES))

#Forces Make to run these targets even if files with these names exist
.PHONY:all clean

all:make_dirs $(TARGET)
	@echo "Done..."

make_dirs:
ifeq ($(PLATFORM),Windows)
	@if not exist $(subst /,$(STEP),$(BINDIR)) $(MKDIR) $(subst /,$(STEP),$(BINDIR))
	@if not exist $(subst /,$(STEP),$(OBJDIR)) $(MKDIR) $(subst /,$(STEP),$(OBJDIR))
endif
ifeq ($(PLATFORM),Linux)
	@$(MKDIR) $(BINDIR)
	@$(MKDIR) $(OBJDIR)
endif

$(TARGET):$(OFILES)
	@echo "linking..."
	@$(CC) $(LFLAGS) $^ -o $@

$(OBJDIR)/%.o:%.c
ifeq ($(PLATFORM),Windows)
	@if not exist $(subst /,$(STEP),$(dir $@)) $(MKDIR) $(subst /,$(STEP),$(dir $@))
endif
ifeq ($(PLATFORM),Linux)
	@$(MKDIR) $(dir $@)
endif
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "compiled $<"

# '-' symbol makes it optional , doesnt give any error if file is not avaliable
-include $(DFILES)

clean:
	@$(RM) $(subst /,$(STEP),$(OFILES))
	@$(RM) $(subst /,$(STEP),$(DFILES))