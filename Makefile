CC = /opt/wasi-sdk/bin/clang \
	--target=wasm32 \
	-nostdlib \
	-O3 \
	-flto \
	-Wl,--export-all \
	-Wl,--no-entry \
	-Wl,--lto-O3

OUT_DIR = wasm
SOURCE_DIR = core
OUT_FILES = $(OUT_DIR)/discriminant.wasm

$(shell mkdir -p $(OUT_DIR))

build: $(OUT_FILES)

$(OUT_DIR)/%.wasm: $(SOURCE_DIR)/%.c
	$(CC) $< -o $@
