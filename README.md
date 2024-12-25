# Data Structures and Algorithms in C

A comprehensive collection of data structure and algorithm implementations in C programming language.

## Overview

This project provides practical implementations of various data structures and algorithms without explanatory documentation. For detailed explanations of the concepts, visit [Striver's A2Z DSA Course](https://takeuforward.org/strivers-a2z-dsa-course/strivers-a2z-dsa-course-sheet-2).

## Features

### Data Structures
- Stack
- Queue
- Map
- Set
- List
- Dynamic Array
- Priority Queue
- Unordered Map
- Unordered Set

### Algorithms
- Sorting Algorithms
- Graph Algorithms

## System Requirements

### Supported Platforms
- Linux
- Windows

### Prerequisites
- Clang Compiler
- Make Build Tool

## Build Instructions

Navigate to the project workspace in your terminal and use the following commands:

### Windows Commands
```bash
# Build source files
./build.bat source build <filename>    # Generates obj files in bin_int and executable in bin

# Clean project files
./build.bat source clear              # Remove both bin_int and bin directories
./build.bat source clear_bin          # Remove bin directory only
./build.bat source clear_bin_int      # Remove bin_int directory only

# Build testing files
./build.bat testing build             # Generates obj files in bin_int and executable in bin
./build.bat testing clear             # Remove both bin_int and bin directories
./build.bat testing clear_bin         # Remove bin directory only
./build.bat testing clear_bin_int     # Remove bin_int directory only

# Other commands
./build.bat source info               # Display makefile debugging information
./build.bat testing info              # Display makefile debugging information
./build.bat run                       # Execute the binary in bin/EXE
```

### Linux Commands
Replace `./build.bat` with `./build.sh` in all the above commands.

## Contributing

If you're familiar with C programming and data structures/algorithms, feel free to review the source code. The implementations are designed to be self-explanatory for those with relevant background knowledge.

## Note

This project focuses on implementation rather than explanation. For learning purposes, we recommend following along with the TakeUForward website materials while examining the code.