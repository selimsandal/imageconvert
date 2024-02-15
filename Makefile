# Makefile

.PHONY: all clean

all: build_project

build_project:
	@echo "Creating build directory and running qmake..."
	@mkdir -p build && cd build && qmake ../ && make

clean:
	@echo "Cleaning up..."
	@rm -rf build