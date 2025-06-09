# Default target
all: build run

# Create build directory and configure
configure:
	mkdir -p build
	cd build && cmake ..

# Build the project
build: configure
	cd build && cmake --build .

# Run the program
run: build
	cd build && ./main_proj_file

# Clean build files
clean:
	rm -rf build

# Force rebuild
rebuild: clean all

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Configure, build, and run (default)"
	@echo "  build   - Configure and build only"
	@echo "  run     - Build and run"
	@echo "  clean   - Remove build directory"
	@echo "  rebuild - Clean and rebuild everything"
	@echo "  help    - Show this help"

# Mark targets as phony (not file names)
.PHONY: all configure build run clean rebuild help
