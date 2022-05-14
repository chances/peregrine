OS := $(shell uname -s)

CC := clang
CFLAGS := -std=c99 -W -Wall -pedantic

ifndef CONFIG
	CONFIG := debug
endif
ifeq ($(CONFIG),debug)
	CFLAGS += -g -DDEBUG
endif

INCLUDE_DIRS := subprojects/glfw/include/GLFW subprojects/wgpu subprojects/wren/src/include
INCLUDES := $(foreach dir, $(INCLUDE_DIRS), -I$(dir))

SOURCES :=

LINK_DIRS := subprojects/wren/lib subprojects/glfw/src subprojects/wgpu
LINKER_FLAGS := $(foreach dir, $(LINK_DIRS), -L$(dir))
LINKER_FLAGS += -lwren -lglfw -lwgpu

# https://www.glfw.org/docs/3.3.6/build_guide.html#build_link_osx
ifeq ($(OS),Darwin)
	LINKER_FLAGS += -framework Cocoa -framework OpenGL -framework IOKit
	SOURCES += src/platform/mac.c
endif

PEREGRINE := bin/peregrine

.DEFAULT_GOAL := peregrine
all: peregrine

peregrine: $(PEREGRINE)
.PHONY: peregrine

# files := $(foreach dir,$(dirs),$(wildcard $(dir)/*))

$(PEREGRINE): configure $(SOURCES) bin/peregrine.c
	$(CC) $(INCLUDES) $(CFLAGS) $(LINKER_FLAGS) $(SOURCES) bin/peregrine.c -o $(PEREGRINE)
ifeq ($(OS),Darwin)
	@echo "Fixing up libwgpu_native dylib path…"
	@install_name_tool -change /Users/runner/work/wgpu-native/wgpu-native/target/x86_64-apple-darwin/debug/deps/libwgpu_native.dylib @executable_path/../subprojects/wgpu/libwgpu.dylib $(PEREGRINE)
	@otool -L $(PEREGRINE) | grep wgpu
endif

configure: subprojects/glfw src/wgpu.i subprojects/wren/lib/libwren.a
.PHONY: configure

# Subprojects
subprojects/glfw: subprojects/glfw.Makefile
	@make -C subprojects -f glfw.Makefile
subprojects/wgpu/wgpu.h: subprojects/wgpu.Makefile
	@make -C subprojects -f wgpu.Makefile
subprojects/wren/lib/libwren.a: subprojects/wren.Makefile
	@make -C subprojects -f wren.Makefile

# TODO: When more testing targets are added, run *all* the tests
TEST_SOURCES := tests/script.c
TEST_TARGETS := $(foreach test, $(TEST_SOURCES), $(notdir $(basename test)))

bin/tests:
	@mkdir -p bin/tests

check: CFLAGS += -fprofile-arcs -ftest-coverage
check: configure bin/tests
	$(CC) $(INCLUDES) $(CFLAGS) $(LINKER_FLAGS) tests/script.c -o bin/tests/script
ifeq ($(OS),Darwin)
	@echo "Fixing up libwgpu_native dylib path…"
	@install_name_tool -change /Users/runner/work/wgpu-native/wgpu-native/target/x86_64-apple-darwin/debug/deps/libwgpu_native.dylib @executable_path/../../subprojects/wgpu/libwgpu.dylib bin/tests/script
	@otool -L bin/tests/script | grep wgpu
endif
	@echo "Running script tests…"
	@bin/tests/script
	@echo "Done."
.PHONY: check
