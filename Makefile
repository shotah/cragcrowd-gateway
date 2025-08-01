# CragCrowd Gateway Makefile

.PHONY: build upload monitor clean setup install-deps

# Default target
all: build

# Install dependencies
install-deps:
	@echo "Installing PlatformIO dependencies..."
	pio pkg install

# Build the firmware
build:
	@echo "Building CragCrowd gateway..."
	pio run

# Upload firmware to device
upload:
	@echo "Uploading firmware to device..."
	pio run --target upload

# Monitor serial output
monitor:
	@echo "Starting serial monitor..."
	pio device monitor

# Upload and monitor in one command
deploy: upload monitor

# Clean build files
clean:
	@echo "Cleaning build files..."
	pio run --target clean

# Setup development environment
setup:
	@echo "Setting up development environment..."
	@echo "Installing PlatformIO if not present..."
	@which pio > /dev/null || pip install platformio
	$(MAKE) install-deps

# Check device connection
check-device:
	@echo "Checking connected devices..."
	pio device list

# Format code (if clang-format is available)
format:
	@echo "Formatting code..."
	@find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i || echo "clang-format not available"

help:
	@echo "Available commands:"
	@echo "  setup       - Setup development environment"
	@echo "  build       - Build firmware"
	@echo "  upload      - Upload firmware to device"
	@echo "  monitor     - Monitor serial output"
	@echo "  deploy      - Upload and monitor"
	@echo "  clean       - Clean build files"
	@echo "  check-device- List connected devices"
	@echo "  format      - Format source code"