#include <glfw3.h>
#include <wren.h>

int main(int argc, char const *argv[]) {
  WrenConfiguration config;
  wrenInitConfiguration(&config);
  WrenVM* vm = wrenNewVM(&config);

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow* window = glfwCreateWindow(640, 480, "Peregrine", NULL, NULL);

  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  glfwTerminate();
  wrenFreeVM(vm);

  return 0;
}
