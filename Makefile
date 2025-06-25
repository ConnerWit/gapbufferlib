# Detect operating system
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    MKDIR := if not exist build mkdir build
    CD_AND_RUN := cd build &&
    EXECUTABLE := main_proj_file.exe
    RMDIR := if exist build rmdir /s /q build
    NEWLINE := echo.
else
    DETECTED_OS := $(shell uname -s)
    MKDIR := mkdir -p build
    CD_AND_RUN := cd build &&
    EXECUTABLE := main_proj_file
    RMDIR := rm -rf build
    NEWLINE := echo
endif

# Default target
all: build run

# Create build directory and configure
configure:
	$(MKDIR)
	$(CD_AND_RUN) cmake ..

# Build the project
build: configure
	$(CD_AND_RUN) cmake --build .

# Run the program
run: build
	$(CD_AND_RUN) ./$(EXECUTABLE)
	@$(NEWLINE)

# Clean build files
clean:
	$(RMDIR)

# Force rebuild
rebuild: clean all

# Show detected OS (useful for debugging)
show-os:
	@echo "Detected OS: $(DETECTED_OS)"

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Configure, build, and run (default)"
	@echo "  build   - Configure and build only"
	@echo "  run     - Build and run"
	@echo "  clean   - Remove build directory"
	@echo "  rebuild - Clean and rebuild everything"
	@echo "  show-os - Show detected operating system"
	@echo "  help    - Show this help"

# Mark targets as phony (not file names)
.PHONY: all configure build run clean rebuild show-os help
