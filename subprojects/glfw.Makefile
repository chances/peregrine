VERSION := 3.3.6

.DEFAULT_GOAL := glfw/src/libglfw3.a

glfw-3.3.6/CMakeLists.txt:
	@echo "Downloading GLFW sources…"
	@curl -L https://github.com/glfw/glfw/releases/download/$(VERSION)/glfw-$(VERSION).zip --output /tmp/glfw-$(VERSION).zip
	@echo "Unzipping GLFW sources…"
	@unzip -q /tmp/glfw-$(VERSION).zip
glfw/src/CMakeLists.txt: glfw-3.3.6/CMakeLists.txt
	@ln -s glfw-3.3.6 glfw
glfw/src/libglfw3.a: glfw/src/CMakeLists.txt
	@echo "Building GLFW…"
	@cd glfw && \
	cmake . -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_DOCS=OFF && \
	make
