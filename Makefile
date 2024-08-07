BUILD_DIR = build
TEST_DIR = test

all: build

.PHONY: configure build build-cli docs release install test clean

build:
	cmake -B $(BUILD_DIR) -S .
	cmake --build $(BUILD_DIR)

build-r:
	cmake -B $(BUILD_DIR) -S . -DBUILD_R_BINDINGS=ON
	cmake --build $(BUILD_DIR)

build-cli:
	cmake -B $(BUILD_DIR) -S . -DBUILD_CLI=ON
	cmake --build $(BUILD_DIR)

docs:
	cmake -B $(BUILD_DIR) -S . -DBUILD_DOCS=ON -DBUILD_TESTING=OFF
	cmake --build $(BUILD_DIR)
	cmake --install $(BUILD_DIR)

release: clean
	cmake -B $(BUILD_DIR) -S . -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
	$(MAKE) build
	$(MAKE) install

install:
	cmake --install $(BUILD_DIR)

install-cli: build-cli
	cmake --install $(BUILD_DIR) --component CLI

test: 
	cmake -B $(BUILD_DIR) -S . -DBUILD_DOCS=OFF -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(BUILD_DIR)
	ctest --test-dir $(BUILD_DIR) --output-on-failure

clean:
	rm -rf $(BUILD_DIR)

