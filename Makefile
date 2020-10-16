WASI_SDK_PATH = /opt/wasi-sdk-10.0

CC = $(WASI_SDK_PATH)/bin/clang \
	--target=wasm32-unknown-wasi \
	--sysroot=$(WASI_SDK_PATH)/share/wasi-sysroot \
	-O3 \
	-flto \
	-lm \
	-Wl,--export-all \
	-Wl,--no-entry \
	-nostartfiles \
	-Wl,--lto-O3

OUT_DIR = wasm
SOURCE_DIR = core
OUT_FILES = $(OUT_DIR)/raycast.wasm

$(shell mkdir -p $(OUT_DIR))

build: $(OUT_FILES)

$(OUT_DIR)/%.wasm: $(SOURCE_DIR)/%.c
	$(CC) $< -o $@
