WASI_SDK_PATH = /opt/wasi-sdk-12.0

CC = $(WASI_SDK_PATH)/bin/clang \
	--target=wasm32-unknown-wasi \
	--sysroot=$(WASI_SDK_PATH)/share/wasi-sysroot \
	-O3 \
	-flto \

BUILD_FLAGS = -lm \
	-Wl,--export-all \
	-Wl,--no-entry \
	-nostartfiles \
	-Wl,--lto-O3

OUT_DIR = dist
OBJECTS_DIR = object

SOURCE_DIR = core

OUTPUT = $(OUT_DIR)/game.wasm

OBJECTS = $(OBJECTS_DIR)/game.o \
	$(OBJECTS_DIR)/map.o \
	$(OBJECTS_DIR)/player.o \
	$(OBJECTS_DIR)/raycast.o

build: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CC) $(BUILD_FLAGS) $(OBJECTS) -o $(OUTPUT)

$(OBJECTS_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJECTS_DIR)/*.o $(OUTPUT)
