#define GLFW_EXPOSE_NATIVE_COCOA

#include <glfw3.h>
#include <wgpu.h>
#include <wren.h>

#include "../src/platform.h"
#include "../src/script.h"
#include "../src/gpu.h"

int main(int argc, char const *argv[]) {
  Runtime runtime = runtimeInit();

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(640, 480, "Peregrine", NULL, NULL);

  WGPUSurfaceDescriptorFromMetalLayer* surfaceDescriptor = NULL;
  // TODO: Platform window integration with Linux & Windows
#ifdef __OSX__
  surfaceDescriptor = &(WGPUSurfaceDescriptorFromMetalLayer){
    .chain = (WGPUChainedStruct){.next = NULL, .sType = WGPUSType_SurfaceDescriptorFromMetalLayer},
    .layer = peregrine_platform_GetMetalLayer((void*) glfwGetCocoaWindow(window)),
  };
#endif
  assert(surfaceDescriptor);

  WGPUSurface surface = wgpuInstanceCreateSurface(NULL, &(WGPUSurfaceDescriptor){
    .label = NULL,
    .nextInChain = (const WGPUChainedStruct*) surfaceDescriptor,
  });
  gpuInit(&runtime, surface);

  const char* module = "main";
  const char* script = "System.print(\"I am running in a VM!\")";

  if (wrenInterpret(runtime.vm, module, script) != WREN_RESULT_SUCCESS) {
    // TODO: case WREN_RESULT_COMPILE_ERROR: { printf("Compile Error!\n"); } break;
    // TODO: case WREN_RESULT_RUNTIME_ERROR: { printf("Runtime Error!\n"); } break;
    return 1;
  }

  #pragma region Main Loop
  glfwShowWindow(window);
  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
  #pragma endregion

  glfwTerminate();
  wrenFreeVM(runtime.vm);

  return 0;
}
