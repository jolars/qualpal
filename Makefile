BUILD_DIR = build
TEST_DIR = test

all: configure build

.PHONY: configure
configure:
	mkdir -p $(BUILD_DIR)
	cmake -B $(BUILD_DIR) -S .

.PHONY: build
build:
	cmake --build build

.PHONY: install
install:
	cmake --install $(BUILD_DIR)

.PHONY: test
test:
	ctest --test-dir $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*
