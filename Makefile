CC := clang
CFLAGS := -std=c99 -W -Wall -pedantic

INCLUDE_DIRS := subprojects/glfw/include/GLFW subprojects/wgpu subprojects/wren/src/include
INCLUDES := $(foreach dir, $(INCLUDE_DIRS), -I$(dir))

LINK_DIRS := subprojects/wren/lib subprojects/glfw/src
LINKER_FLAGS := $(foreach dir, $(LINK_DIRS), -L$(dir))
LINKER_FLAGS += -lwren -lglfw

# https://www.glfw.org/docs/3.3.6/build_guide.html#build_link_osx
ifeq ($(shell uname -s),Darwin)
	LINKER_FLAGS += -framework Cocoa -framework OpenGL -framework IOKit
endif

PEREGRINE := bin/peregrine

.DEFAULT_GOAL := peregrine
all: peregrine

peregrine: $(PEREGRINE)
.PHONY: peregrine

# files := $(foreach dir,$(dirs),$(wildcard $(dir)/*))

$(PEREGRINE): configure
	$(CC) $(INCLUDES) $(CFLAGS) $(LINKER_FLAGS) bin/peregrine.c -o $(PEREGRINE)

configure: subprojects/glfw subprojects/wgpu subprojects/wren
.PHONY: configure

# Subprojects
subprojects/glfw: subprojects/glfw.Makefile
	@make -C subprojects -f glfw.Makefile
subprojects/wgpu: subprojects/wgpu.Makefile
	@make -C subprojects -f wgpu.Makefile
subprojects/wren: subprojects/wren.Makefile
	@make -C subprojects -f wren.Makefile
