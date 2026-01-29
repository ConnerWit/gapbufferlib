# Gap Buffer Library (C)
A simple C library implementing a gap buffer, the core data structure behind text editors.
Supports insertion, deletion, cursor movement and dynamic resizing with strict invariants for correctness.

## Features
- Efficient insert/delete operations
- Cursor movement with gap tracking
- Automatic gap growth
- Safe memory management
- Debug invariants

### Next Steps
- Split into `.h / .c` for library use
- Configurable gap size & improved growth/shrink strategies

Designed as a library core for building editors or text-processing tools.
