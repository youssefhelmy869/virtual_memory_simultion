# Memory Management Project

## Overview

This project simulates a basic operating system's memory management subsystem, including physical memory, virtual memory, process management, and swap file operations. It is designed for educational purposes to demonstrate concepts such as paging, frame allocation, process creation/deletion, and swapping.

## Features

- Physical memory management with frame allocation
- Virtual memory management with paging
- Process creation and deletion with unique IDs
- Frame table tracking usage of memory frames
- Swap file mechanism for handling memory overflows
- Read and write operations to process memory

## File Structure

- `operating_system.hpp`: Main class for OS simulation, manages processes and memory.
- `physical_memory.hpp`: Defines physical memory and frame structures.
- `virtual_memory.hpp`: Defines virtual memory and page structures.
- `process.hpp`: Defines process structure and attributes.
- `swap_file.hpp`: Implements swap file logic for paging out processes.
- `helper_funcs.hpp`: Utility functions for conversions and random ID generation.

## Usage

1. **Compile the project** using your preferred C++ compiler.
2. **Create processes** using the `create_process` method.
3. **Read and write memory** using the `read` and `write` methods.
4. **Delete processes** using the `delete_process` method.
5. **Monitor memory usage** with `print_frame_table`.

## Example

```cpp
operating_system os;
int pid = os.create_process("ProcessA");
os.write(pid, "00000001", "A");
std::string value = os.read(pid, "00000001");
os.delete_process(pid);
```

## Requirements

- C++17 or later
- Standard library only (no external dependencies)

## License

This project is provided for educational purposes.

