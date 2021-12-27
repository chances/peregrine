VERSION := 0.4.0

MAKE_DIR := wren/projects/make
ifeq ($(shell uname -s),Darwin)
	MAKE_DIR = wren/projects/make.mac
endif

.DEFAULT_GOAL := wren/lib/libwren.a

wren/README.md:
	@echo "Downloading Wren…"
	git clone -b $(VERSION) https://github.com/wren-lang/wren.git --depth 1

wren/lib/libwren.a: wren/README.md
	@echo "Building Wren…"
	@make -C $(MAKE_DIR) wren
