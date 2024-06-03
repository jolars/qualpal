BUILD_DIR = build
TEST_DIR = test

all: configure build

.PHONY: configure
configure:
	cmake -B $(BUILD_DIR) -S . -DCMAKE_BUILD_TYPE=Debug

.PHONY: build
build:
	cmake --build $(BUILD_DIR)

.PHONY: build-r
build-r:
	cmake -B $(BUILD_DIR) -S . -DBUILD_R_BINDINGS=ON
	cmake --build $(BUILD_DIR)

.PHONY: docs
docs:
	cmake -B $(BUILD_DIR) -S . -DBUILD_DOCS=ON -DBUILD_TESTING=OFF
	cmake --build $(BUILD_DIR)
	cmake --install $(BUILD_DIR)

.PHONY: release
release: clean
	cmake -B $(BUILD_DIR) -S . -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
	$(MAKE) build
	$(MAKE) install

.PHONY: install
install:
	cmake --install $(BUILD_DIR)

.PHONY: install-r
install-r: build-r
	cmake --install $(BUILD_DIR) --component R_BINDINGS

.PHONY: test
test: 
	cmake -B $(BUILD_DIR) -S . -DBUILD_DOCS=OFF -DBUILD_TESTING=ON
	cmake --build $(BUILD_DIR)
	ctest --test-dir $(BUILD_DIR) --output-on-failure

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

